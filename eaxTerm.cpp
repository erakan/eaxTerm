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

// eaxTerm.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "eaxTerm.h"

#include "MainFrm.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEaxTermApp

BEGIN_MESSAGE_MAP(CEaxTermApp, CWinApp)
	//{{AFX_MSG_MAP(CEaxTermApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEaxTermApp construction

CEaxTermApp::CEaxTermApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEaxTermApp object

CEaxTermApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEaxTermApp initialization

BOOL CEaxTermApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
#ifndef _DEBUG
	HWND hWnd_ = ::FindWindow(_T("eaxMainWndv1.x"), NULL);
	if( hWnd_ != NULL)
	{
		//Çalışanı adamın gözüne getirelim..
		::SetForegroundWindow(hWnd_);
		return FALSE;
	}
	HKEY key;
	if (::RegCreateKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",&key) == ERROR_SUCCESS)
	{
		char fName[MAX_PATH];
		::GetModuleFileName(NULL,fName, sizeof(fName));
		::RegSetValueEx(key, "eaxTerm",0,REG_SZ,(unsigned char *)fName,strlen(fName)+1);
		::RegCloseKey(key);
	}
#endif
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// 
	//pFrame->m_BufferLength = 20; // 50 satırlık bir buffer

	// create and load the frame with its resources

//	pFrame->LoadFrame(IDR_MAINFRAME,
//		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
//		NULL);
	WNDCLASS wc;
	wc.hbrBackground = NULL;//(HBRUSH) pFrame->m_BackGroundBrush.GetSafeHandle();
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpszMenuName = NULL;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon =  NULL; //myAppLoadIcon(IDI_ZUNA);
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpszClassName = _T("eaxMainWndv1.x");
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = AfxWndProc;
	AfxRegisterClass(&wc);

	// The one and only window has been initialized, so show and update it.
	CRect r(0,0,593,375);
	pFrame->CreateEx(0, _T("eaxMainWndv1.x"), _T("eaxTerm"), WS_POPUP /*0| WS_DLGFRAME*/, r.left,
		r.top, r.Width(), r.Height(), NULL, NULL);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	setlocale( LC_ALL, "" );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEaxTermApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEaxTermApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CEaxTermApp message handlers

