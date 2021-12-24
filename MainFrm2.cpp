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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "eaxTerm.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()
	ON_WM_DROPFILES()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_Font.CreatePointFont(88, "Arial");
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//CPaintDC dc(this);
	//SetFont(&m_Font);

	m_wndView.m_pBufferLength = &m_BufferLength;

	// create a view to occupy the client area of the frame
	m_wndView.m_pFont = &m_Font;
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	CenterWindow();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	WNDCLASS wc;
	wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpszMenuName = NULL;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpszClassName = _T("eaxMainWndv1.x");
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = AfxWndProc;
	AfxRegisterClass(&wc);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = _T("eaxMainWndv1.x"); //AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
	CClientDC dc(this);
	TEXTMETRIC tm;
	CFont *pOldFont = dc.SelectObject(&m_Font);
	dc.GetTextMetrics(&tm);
	dc.SelectObject(pOldFont);
	lpMMI->ptMaxTrackSize = CPoint(tm.tmAveCharWidth * 80 +88, tm.tmHeight * m_BufferLength+43 +16);
	lpMMI->ptMinTrackSize = CPoint(124,103);
}

//DEL UINT CMainFrame::OnNcHitTest(CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	return HTCAPTION;
//DEL 	return CFrameWnd::OnNcHitTest(point);
//DEL }

void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnDropFiles(hDropInfo);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}
