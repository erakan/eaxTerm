/**
 * eaxTerm ~ erdincakan Xtreme Terminal
 * Copyright (c) 2003 Erdinc AKAN (erakan@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **/

// EaxTermClock.cpp : implementation file
//

#include "stdafx.h"
#include "eaxTerm.h"
#include "EaxTermClock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEaxTermClock

CEaxTermClock::CEaxTermClock()
{
	m_nCharPos = -2;
	m_bLButtonDown = FALSE;
	m_MousePoint = NULL;
}

CEaxTermClock::~CEaxTermClock()
{
}


BEGIN_MESSAGE_MAP(CEaxTermClock, CWnd)
	//{{AFX_MSG_MAP(CEaxTermClock)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEaxTermClock message handlers

BOOL CEaxTermClock::Create(CRect rWnd, CWnd *pOwner)
{
	WNDCLASS wndcls;
	wndcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndcls.lpfnWndProc = AfxWndProc; // DefWindowProc
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = AfxGetInstanceHandle();;
	wndcls.hIcon = NULL;
	wndcls.hCursor = NULL;
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = _T("eaxTerm Clock Class v1.x");
	
	if (!AfxRegisterClass(&wndcls))
		return FALSE;
	if(CWnd::CreateEx(NULL, _T("eaxTerm Clock Class v1.x"), _T("eaxTermClock"), WS_CHILD | WS_VISIBLE , 
		rWnd.left, rWnd.top, rWnd.Width(), rWnd.Height(), pOwner->GetSafeHwnd(), NULL))
		return TRUE;
	else
		return FALSE;
}

void CEaxTermClock::OnPaint() 
{
	CPaintDC dc(this);
	ClearBg(&dc);
	UpdateClock();
}

void CEaxTermClock::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nTimer)
		UpdateClock();
	CWnd::OnTimer(nIDEvent);
}

void CEaxTermClock::UpdateClock()
{
	CClientDC dc(this); 
	CRect r, r1;
	CString ToDisplayStr;
	UINT nFormat;
	nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX;
	//Select Font
	CFont *pOldFont = dc.SelectObject(&m_Font);
	//Get String	
	ToDisplayStr = GetFormatted();
	//Clear the back ground
	ClearBg(&dc);
	//Get Rect, Deflate (because we have borders)
	GetClientRect(&r);
	r.DeflateRect(1,1);
	r1 = r;
	//Calculate Required space for string
	dc.DrawText(ToDisplayStr, &r1, nFormat | DT_CALCRECT);
	if(r1.Width() > r.Width())
	{
		m_ClockSize = r1.Width();

		if(m_nCharPos == -2)
		{
			KillTimer(m_nTimer);
			m_nTimer = SetTimer(1, 300, NULL);
		}

		ToDisplayStr += "  ***  ";

		if (!m_bLButtonDown)
			m_nCharPos++;
		
		if(m_nCharPos > ToDisplayStr.GetLength())
			m_nCharPos = 0;

		if(m_nCharPos > -1)
		{
			CString strBackup;
			strBackup = ToDisplayStr.Left(m_nCharPos);
			ToDisplayStr.Delete(0, m_nCharPos);
			ToDisplayStr += strBackup;
		}
		
	}
	else
	{
		m_nCharPos = -2;
		nFormat |= DT_CENTER;
	}

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,255,0));
	dc.DrawText(ToDisplayStr, &r, nFormat);
	dc.SelectObject(pOldFont);
}

int CEaxTermClock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateFont(8, 0, 0, 0, FW_NORMAL,
            0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
            CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
            FF_DONTCARE, "Ms Sans Serif");
	
	m_strTimeFormat = "Memory Usage %L%%, %r Kb empty, %d %B %Y %A, %I/%H:%M:%S, %Y. year,  %U. week, %j. day.";
	m_strToolTip = "%#c";
	m_nTimer = SetTimer(1, 1000, NULL);

	EnableToolTips(TRUE);

	return 0;
}

void CEaxTermClock::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bLButtonDown = TRUE;
	m_MousePoint = point;
	RelayToolTipEvent(GetCurrentMessage());
	CWnd::OnLButtonDown(nFlags, point);
}

void CEaxTermClock::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(GetCapture() == this)
		ReleaseCapture();
	m_bLButtonDown = FALSE;
	m_MousePoint = NULL;
	RelayToolTipEvent(GetCurrentMessage());
	CWnd::OnLButtonUp(nFlags, point);
}

void CEaxTermClock::OnMouseMove(UINT nFlags, CPoint p) 
{
	if(m_bLButtonDown)
	{
		if(m_nCharPos != -2)
		{
			int t,y;
			t = p.x;
			y = t / m_ClockSize;
			t = t * 100/ m_ClockSize;
			t = (GetFormatted().GetLength() *t / 100) * y == 0 ? 1: y;
			if(p.x > m_MousePoint.x)
				m_nCharPos -= t;
			else if (p.x < m_MousePoint.x)
				m_nCharPos += t;
			if(m_nCharPos < 0)
			{
				m_nCharPos = GetFormatted().GetLength() - m_nCharPos;
			}
			m_MousePoint = p;
		}
	}
	RelayToolTipEvent(GetCurrentMessage());
	CWnd::OnMouseMove(nFlags, p);
}

CString CEaxTermClock::GetFormatted(BOOL Tip /* = FALSE*/)
{
	CString StrParsed, s;
	StrParsed = Tip ? m_strToolTip:m_strTimeFormat;
	MEMORYSTATUS ms;
	ms.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&ms);
	s.Format("%d", ms.dwMemoryLoad);
	StrParsed.Replace("%L", s);
	s.Format("%d", ms.dwAvailPhys / 1024); // KB!
	StrParsed.Replace("%r", XFormatNumber(s));
	return CTime::GetCurrentTime().Format(StrParsed);
}

BOOL CEaxTermClock::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	::strcpy(pTTT->szText, (LPCTSTR) GetFormatted(TRUE));
	return TRUE;
}

void CEaxTermClock::RelayToolTipEvent(const MSG *pMsg)
{
	MSG MsgCopy;
	::memcpy(&MsgCopy, pMsg, sizeof(MSG));
	FilterToolTipMessage(&MsgCopy);
}

int CEaxTermClock::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
    if (pTI != NULL)
    {
        pTI->hwnd = m_hWnd;
        pTI->uId = 0;
        pTI->uFlags = TTF_ALWAYSTIP;
        GetClientRect(&(pTI->rect));
        pTI->lpszText = LPSTR_TEXTCALLBACK;
    }
    return 1;
}

void CEaxTermClock::ClearBg(CDC *pDC)
{
	CRect r;
	GetClientRect(&r);
	pDC->Draw3dRect(r.left,r.top, r.right, r.bottom, RGB(120,120,120), RGB(255,255,255));
	r.DeflateRect(1,1);
	pDC->FillSolidRect(&r, RGB(0,0,0));
}

void CEaxTermClock::OnClose() 
{
	KillTimer(m_nTimer);
	CWnd::OnClose();
}

void CEaxTermClock::SetClockStr(CString strSet)
{
	m_strTimeFormat = strSet;
	UpdateClock();
}

void CEaxTermClock::SetTipStr(CString strSet)
{
	m_strToolTip = strSet;
	UpdateClock();
}
