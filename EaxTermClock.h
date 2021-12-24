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

#if !defined(AFX_EAXTERMCLOCK_H__C7631CF4_08AE_4155_890F_082B44FD6821__INCLUDED_)
#define AFX_EAXTERMCLOCK_H__C7631CF4_08AE_4155_890F_082B44FD6821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EaxTermClock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEaxTermClock window

class CEaxTermClock : public CWnd
{
// Construction
public:
	CEaxTermClock();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEaxTermClock)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTipStr(CString strSet);
	void SetClockStr(CString strSet);
	BOOL Create(CRect rWnd, CWnd *pOwner);
	virtual ~CEaxTermClock();

	// Generated message map functions
protected:
	void ClearBg(CDC *pDC);
	int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	void RelayToolTipEvent(const MSG* pMsg);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strToolTip;
	CString GetFormatted(BOOL Tip = FALSE);
	int m_ClockSize;
	CPoint m_MousePoint;
	BOOL m_bLButtonDown;
	int m_nCharPos;
	void UpdateClock();
	UINT m_nTimer;
	CString m_strTimeFormat;
	CFont m_Font;
	//{{AFX_MSG(CEaxTermClock)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EAXTERMCLOCK_H__C7631CF4_08AE_4155_890F_082B44FD6821__INCLUDED_)
