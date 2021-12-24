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

#if !defined(AFX_CONSOLE_H__6A765343_B24B_496B_A7F8_1DAB81997BB2__INCLUDED_)
#define AFX_CONSOLE_H__6A765343_B24B_496B_A7F8_1DAB81997BB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Console.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConsole window

class CConsole : public CWnd
{
// Construction
public:
	CConsole();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsole)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConsole();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConsole)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLE_H__6A765343_B24B_496B_A7F8_1DAB81997BB2__INCLUDED_)
