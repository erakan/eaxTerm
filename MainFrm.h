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

// ChildView.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__BD0D0BD7_5A76_46F9_BA7D_EE73CD5A1A09__INCLUDED_)
#define AFX_CHILDVIEW_H__BD0D0BD7_5A76_46F9_BA7D_EE73CD5A1A09__INCLUDED_

#include "EaxTermClock.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMainFrame window

class CMainFrame : public CWnd
{
// Construction
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL m_StretchMode;
	void ResetBitmap();
	CBitmap m_BackBitmap;
	CBrush m_BackGroundBrush;
	virtual ~CMainFrame();

	// Generated message map functions
protected:
	COLORREF m_clrTextColor;
	void PrintAndWait(CString Str);
	void Settings(BOOL Read = FALSE);
	BOOL m_OnTop;
	void ResetFont();
	int m_FontSize;
	CString m_FontName;
	int DosyadanListe(CObArray &mListe, CString filename, BOOL bOku = TRUE);
	BOOL _mu(LPCTSTR Command);
	CFont m_Font;
//	UINT m_BufferLength; // I've put it on to the struct.
	CString m_strConsole;
	void Printf(CString Text, BOOL Count = TRUE, BOOL Paint = FALSE, BOOL CountForInput = FALSE, BOOL PauseAfter = FALSE, int *DoCalculate = NULL);
	void DrawMemoryBar(CDC *pDC);
	CEaxTermClock m_Clock;
	BOOL m_bShutDownReBoot;
	int m_nShutDownRemain;
	void ShutDownAll();
	UINT m_CurrentPos;
	int m_cxChar;
	BOOL m_bCursorVis;
	BOOL m_bMoving;
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPowerBroadCast(WPARAM dwPowerEvent, LPARAM dwData);
	afx_msg LRESULT OnTrayNotify(WPARAM uID, LPARAM lEvent);
	void ReSetWindowPos();
	int SaveList(BOOL bOku = FALSE);
	int FindInList(LPCTSTR String, BOOL FileName = FALSE);
	int ParseCommand(CString &String);
	void Execute(CString File, CString Parameters);
	CObArray m_ExecList;
	CString m_strContText;
	BOOL m_PressLock;
	void Help();
	int m_ComBuffPos;
	BOOL _mup(LPCTSTR Command);
	BOOL m_GetInput;
	void Cursor(BOOL Clear = FALSE);
	CPoint m_CursorPos;
	void Version();
	void ProcessCommand();
	void DrawTextLine(LPCTSTR pszLine = NULL);
	CString m_Command;
	CRect m_rConsole;
	int m_LineHeight;
	void FlipScreen();
	CStringArray m_CommandBuffer;
	CSize m_SatirWidthHeight;
	struct ROWCOLS
	{
		UINT Width;
		UINT Height;
	}m_RowCols;
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__BD0D0BD7_5A76_46F9_BA7D_EE73CD5A1A09__INCLUDED_)
