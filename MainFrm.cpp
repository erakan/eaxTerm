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

// ChildView.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "eaxTerm.h"
#include "MainFrm.h"
#include "ExecuteLink.h"
#include "commands.h"


#include <mmsystem.h>
#pragma comment(lib, "winmm")

#define DTO(X) Printf(X)//DrawTextOutput(X)
#define DTOP(X,Y) Printf(X, TRUE, FALSE, TRUE, TRUE);// PrintAndWait(X)//DrawTextOutput(X,Y)
#define DEFAULT_TEXT_COLOR RGB(0,255,0)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WS_EX_LAYERED	0x00080000   // Layered Transperacy için kullanılıyor...
#define LWA_ALPHA		0x00000002	// buda aynen...




HMODULE		hUserDll;

BOOL OpenBitmap(LPCTSTR pszFileName, CBitmap *pBitmap)
{
	HANDLE Image;
	Image = ::LoadImage(NULL, pszFileName,IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | 
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(Image)
	{
		pBitmap->DeleteObject();
		pBitmap->Attach(Image);
		return TRUE;
	}
	return FALSE;
	CFile cf;
	BITMAPFILEHEADER bmh;
	BITMAPINFOHEADER bih;
	BYTE	*pData = NULL;


	cf.Open(pszFileName, CFile::modeRead);
	cf.Read(&bmh, sizeof(BITMAPFILEHEADER));
	if(bmh.bfType != 0x4D42) // check to see if this is not a BMP file...
	{
		cf.Close();
		return FALSE;
	}
	cf.Read(&bih, sizeof(BITMAPINFOHEADER));
	int size = (((bih.biWidth*24) + 31) / 32 * 4)*bih.biHeight;
	pData = new BYTE[size];
	if( !pData)
	{
		cf.Close();
		return FALSE;
	}
	cf.Seek(bmh.bfOffBits, CFile::begin);
	cf.Read(pData, size);
	cf.Close();
	pBitmap->SetBitmapBits(size, pData);
	return TRUE;
}

BOOL SetTransparent(CWnd* pWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	BOOL	bRet = TRUE;
	typedef BOOL (WINAPI* lpfnSetTransparent)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	
	// Check that "USER32.dll" library has been loaded successfully...
	if ( hUserDll )
	{
		lpfnSetTransparent pFnSetTransparent  = NULL;
		pFnSetTransparent  = (lpfnSetTransparent)GetProcAddress(hUserDll, "SetLayeredWindowAttributes");
		if (pFnSetTransparent )
			bRet = pFnSetTransparent(pWnd->m_hWnd, crKey, bAlpha, dwFlags);
		else 
			bRet = FALSE;
	} //if( hUserDll )

	return bRet;
} // End of SetTransparent function

UINT IniYazOku(LPCTSTR Anahtar, LPCTSTR DegerAdi, int DefDeger, BOOL bOku = TRUE)
{
	UINT DonenDeger =0;
	char buff[MAX_PATH];
	char fName[MAX_PATH];
	::GetModuleFileName(NULL,fName, sizeof(fName));
	fName[strlen(fName)- 4] = '\0';
	strcat(fName, ".ini");
	if(bOku)
		DonenDeger = ::GetPrivateProfileInt(Anahtar, DegerAdi, DefDeger, fName);
	else
		::WritePrivateProfileString(Anahtar, DegerAdi, itoa(DefDeger, buff, 10),fName);
	return DonenDeger;
}

IniYazOku(LPCTSTR Anahtar, LPCTSTR DegerAdi, LPCTSTR DefDeger, LPSTR DonenDeger, BOOL bOku = TRUE)
{
	char fName[MAX_PATH];
	::GetModuleFileName(NULL,fName, sizeof(fName));
	fName[strlen(fName)- 4] = '\0';
	strcat(fName, ".ini");
	if(bOku)
		::GetPrivateProfileString(Anahtar, DegerAdi, DefDeger, DonenDeger, MAX_PATH, fName);
	else
		::WritePrivateProfileString(Anahtar, DegerAdi, DefDeger,fName);
	return TRUE;

}

void systray_add(HWND hwnd, HICON hMaIcon)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = 3131;
  tnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  tnid.uCallbackMessage = WM_USER+3131;
  tnid.hIcon = hMaIcon;
  strcpy(tnid.szTip,"eaxTerm v0.1 © Erdinc AKAN");
  Shell_NotifyIcon(NIM_ADD, &tnid);
}

void systray_notify(HWND hwnd, HICON hMaIcon)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = 3131;
  tnid.uFlags = NIF_ICON;
  tnid.hIcon = hMaIcon;
  Shell_NotifyIcon(NIM_MODIFY, &tnid);
}

void systray_del(HWND hwnd)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = 3131;
  Shell_NotifyIcon(NIM_DELETE, &tnid);
}

void runstartup_set(LPCTSTR pszKeyName)
{
	char fName[MAX_PATH];
	::GetModuleFileName(NULL,fName, sizeof(fName));
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey );
	RegSetValueEx(hKey, pszKeyName, 0, REG_SZ,(const unsigned char*)fName, strlen(fName));
	RegCloseKey(hKey);
}

void runstartup_remove(LPCTSTR pszKeyName)
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey );
	RegDeleteValue(hKey, pszKeyName);
	RegCloseKey(hKey);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

CMainFrame::CMainFrame()
{
	m_clrTextColor = DEFAULT_TEXT_COLOR;
	m_bMoving = FALSE;
	Settings(TRUE);
	ResetFont();
	//m_BackGroundBrush.CreateSolidBrush(RGB(0,0,0));
	m_BackGroundBrush.CreatePatternBrush(&m_BackBitmap);
	m_PressLock = FALSE;
	m_nShutDownRemain = -1;
	m_bShutDownReBoot = FALSE;
}

CMainFrame::~CMainFrame()
{
}


BEGIN_MESSAGE_MAP(CMainFrame,CWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_NCHITTEST()
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+3131, OnTrayNotify)
	ON_MESSAGE(WM_POWERBROADCAST,OnPowerBroadCast)
	ON_MESSAGE(WM_HOTKEY ,OnHotKey)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
#ifndef _DEBUG	
	cs.dwExStyle |= WS_EX_TOOLWINDOW;
#endif
	cs.style &= ~WS_BORDER;

	return TRUE;
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Transperacy için user DLL'i...
	hUserDll = ::LoadLibrary(_T("USER32.dll"));
	//Transperacy için WS_EX_LAYERED stili atama...
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) |  WS_EX_LAYERED );
	SetTransparent(this, 0, IniYazOku("eaxMain", "Transperacy", 255), LWA_ALPHA );
	systray_add(GetSafeHwnd(), AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	ReSetWindowPos();
	if(IniYazOku("eaxMain", "XY", 255) == 255)
		CenterWindow();
	else
		SetWindowPos((IniYazOku("eaxMain", "OnTop", 0)?&wndTopMost:&wndNoTopMost), 
		IniYazOku("eaxMain", "X", 0),
		IniYazOku("eaxMain", "Y", 0),
		NULL, NULL, SWP_NOSIZE);
	m_CurrentPos = 0;
	m_ComBuffPos = 0;
	Version();
	DTO("Welcome!");
	DragAcceptFiles();
	if(SaveList(TRUE))
	{
		CString s;
		s.Format("Your Execute Library loaded! You have %d links in your library!", m_ExecList.GetSize());
		DTO(s);
	}
#ifndef _DEBUG
	if(!RegisterHotKey( GetSafeHwnd(), 1, MOD_WIN,(UINT) 'X'))
		DTO("Can't register hot key as \"Win + S\"!");
	else
		DTO("Hot-Key Added to the system, whenever you need eaxTerm, press \"Win + S\"!");
#endif
	DTO("For help please type \"help\" or \"?\" and hit enter.");
	SetTimer(1, 900, NULL);
	//SetTimer(5, 1000, NULL);
	CRect r;
	r = m_rConsole;
	r.top = r.bottom + 7;
	r.bottom = r.top + ::GetSystemMetrics(SM_CYSMCAPTION);
	r.left++;
	m_Clock.Create(r, this);
	CString s;
	IniYazOku("eaxClock", "ClockFormat", "", s.GetBufferSetLength(1024));
	s.ReleaseBuffer();
	if(s.GetLength())
		m_Clock.SetClockStr(s);
	s.Empty();
	IniYazOku("eaxClock", "TipFormat", "", s.GetBufferSetLength(1024));
	s.ReleaseBuffer();
	if(s.GetLength())
		m_Clock.SetTipStr(s);
	s.Empty();
	return 0;
}

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_PressLock)
	{
		m_PressLock = FALSE;
		if(nChar != VK_ESCAPE)
		{
			m_CurrentPos = 0;
			m_strConsole.Empty();
			DTOP(m_strContText, TRUE);
			if(!m_PressLock)
			{
				InvalidateRect(&m_rConsole);
				//DrawTextLine();
			}
			return;
		}
	}
	m_ComBuffPos = 0;
	switch (nChar) 
	{
		case VK_ESCAPE:
			Cursor(TRUE);
			m_Command.Empty();
			break;

		case VK_RETURN:
			Cursor(TRUE);
			//m_CurrentPos += m_CurrentCurrPos?m_CurrentCurrPos+1:1;
			Printf("eaxTerm." + m_Command, TRUE, FALSE, TRUE);
			/*if(m_CurrentPos == m_BufferLength)
				FlipScreen();*/
			if(!m_Command.IsEmpty())
			{
				//Önce buffer'a atalım...
				if(m_CommandBuffer.GetSize())
				{
					if(m_Command != m_CommandBuffer[m_CommandBuffer.GetSize()-1])
						m_CommandBuffer.Add(m_Command);
				}
				else
					m_CommandBuffer.Add(m_Command);
				ProcessCommand();
				m_Command.Empty();
			}
			if(m_PressLock)
				return;
			break;

		case VK_BACK:
			if(!m_Command.IsEmpty())
			{
				m_Command = m_Command.Left(m_Command.GetLength()-1);
			}
			else
				return;
			break;

		default:
			if ((nChar >= 0) && (nChar <= 31))
				return;

			if(m_GetInput)
				m_Command += nChar;
			break;
	}
	DrawTextLine();
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CMainFrame::DrawTextLine(LPCTSTR pszLine)
{
	CString line;
	
	if(pszLine)
		line = pszLine;
	else
		line = "eaxTerm." + m_Command;
	Printf(line, FALSE, FALSE, TRUE);
	if(m_Command.GetLength() >= MAX_PATH)
		m_GetInput = FALSE;
	else
		m_GetInput = TRUE;
	return;

}

void CMainFrame::ProcessCommand()
{
	if( _mu(_CA[CA_EXIT_1]) || _mu(_CA[CA_EXIT_2]) ||
		_mu(_CA[CA_EXIT_3]) || _mu(_CA[CA_EXIT_4]) ||
		_mu(_CA[CA_EXIT_5]) || _mu(_CA[CA_EXIT_6]))
	{
		DTO("Byez..");
		Sleep(350);
		PostMessage(WM_CLOSE);
		return;
	}
	if( _mu(_CA[CA_HELP_1]) || _mu(_CA[CA_HELP_2]))
	{
		Help();
		return;
	}
	if( _mu(_CA[CA_VER_1]) || _mu(_CA[CA_VER_2]))
	{
		Version();
		return;
	}
	if( _mu(_CA[CA_TIME]))
	{
		CTime t = CTime::GetCurrentTime();
		DTO(t.Format("%#c"));
		return;
	}
	if(_mup(_CA[CA_CD]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s, s1;
			s1 = s = m_Command.Right(m_Command.GetLength()-x-1);
			s1 = s1.Right(s1.GetLength() - ParseCommand(s));
			ParseCommand(s1);
			if((s.GetLength() && s1.GetLength()) && (s.GetLength()<=2 && s1.GetLength() == 1))
			{
				BOOL bOpenDrive;
				MCI_OPEN_PARMS op;
				DWORD flags;

				::ZeroMemory(&op, sizeof(MCI_OPEN_PARMS));
				op.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_CD_AUDIO;
				s.MakeUpper();
				if(s.Find(":") == -1)
					s += ":";
				bOpenDrive = s1.Compare("o");
				op.lpstrElementName = s;
				flags = MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT | MCI_OPEN_SHAREABLE;

				if (!mciSendCommand(0,MCI_OPEN,flags,(unsigned long)&op)) 
				{

					if(!bOpenDrive)
					{
						DTO("Opening drive \""+s+"\"...");
						mciSendCommand(op.wDeviceID,MCI_SET,MCI_SET_DOOR_OPEN,0);
					}
					else
					{
						DTO("Closing drive \""+s+"\"...");
						mciSendCommand(op.wDeviceID,MCI_SET,MCI_SET_DOOR_CLOSED,0);
					}
					mciSendCommand(op.wDeviceID,MCI_CLOSE,MCI_WAIT,0);
					DTO("Done!");
				}
				else
					DTO("Can't access drive \"" + s + "\"! Its locked or not a CD-ROM drive!");
				return;
			}
		}
		DTO("cd:\n\tOpens or closes the given CD-ROM's tray. For opening use \"o\", for closing use \"c\" as parameter.\nUsage:\n\tcd Dirive o/c\n\tcd d o\n\tcd d c\n");
		return;
	}
	if(_mu(_CA[CA_CLS_1]) || _mu(_CA[CA_CLS_2]))
	{
		m_strConsole.Empty();
		m_CurrentPos = 0;
		InvalidateRect(&m_rConsole);
		return;
	}
	if( _mu(_CA[CA_SETONTOP]))
	{
		if(!m_OnTop)
			SetWindowPos(&wndTopMost, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(&wndNoTopMost, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
		m_OnTop = !m_OnTop;
		DTO(m_OnTop?"eaxTerm set to top of all windows!":"eaxTerm return to normal situation.");
		return;
	}
	if( _mup(_CA[CA_SHUTDOWN]))
	{
		int x = m_Command.Find(" ");
		if ( x != -1)
		{
			CString s,s1,s2;
			s1 = s = m_Command.Right(m_Command.GetLength()-x-1);
			ParseCommand(s);s1.Replace(s, "");
			s2 = s1;
			ParseCommand(s1);s2.Replace(s1, "");
			ParseCommand(s2);
			if(s.CompareNoCase("-now") == 0)
			{
				if(s1.CompareNoCase("-r") == 0)
					m_bShutDownReBoot = TRUE;
				s = (m_bShutDownReBoot?" and rebooting":"");
				DTO("Shutting down" + s + " system! Have a nice day...");
				ShutDownAll();
				return;
			}
			if(s.CompareNoCase("-seconds") == 0)
			{
				if(s1.GetLength())
				{
					int sTime = (int) strtoul(s1, NULL, 10);
					if(sTime)
					{
						if(s2.CompareNoCase("-r") == 0)
						{
							m_bShutDownReBoot = TRUE;
							s2 = " and then reboot";
						}
						else
						{
							m_bShutDownReBoot = FALSE;
							s2.Empty();
						}
						m_nShutDownRemain = sTime;
						s1.Format("%d", sTime);
						SetTimer(5, 1000, NULL);
						DTO("System is going to shutdown in " + s1 + " seconds" + s2 + "...\n");
						return;
					}
				}
			}
			if(s.CompareNoCase("-time") == 0)
			{
				if(s1.GetLength())
				{
					COleDateTime dt, dt1;
					if(dt.ParseDateTime(s1,VAR_TIMEVALUEONLY))
					{
						dt1.ParseDateTime(CTime::GetCurrentTime().Format("%H:%M"), VAR_TIMEVALUEONLY);
						COleDateTimeSpan ts;
						ts = dt - dt1;
						if(ts.GetStatus() == 0 && ts.GetTotalSeconds() >0)
						{
							if(s2.CompareNoCase("-r") == 0)
							{
								m_bShutDownReBoot = TRUE;
								s2 = " and then reboot";
							}
							else
							{
								m_bShutDownReBoot = FALSE;
								s2.Empty();
							}
							m_nShutDownRemain = (int) ts.GetTotalSeconds();
							s1.Format("%d hours, %d minutes, %d", int(ts.GetHours()), 
											int(ts.GetMinutes()), int(ts.GetSeconds()));
							DTO("System is going to shutdown in " + s1 + " seconds" + s2 + "...\n");
						}
						else
							DTO("Wrong time value!");
						return;
					}
				}
			}
			if(s.CompareNoCase("-cancel") == 0)
			{
				if(m_nShutDownRemain >0)
				{
					CString s;
					s.Format("Shutdown operation cancelled at %d!", m_nShutDownRemain);
					m_nShutDownRemain = -1;
					m_bShutDownReBoot = FALSE;
					KillTimer(5);
					DTO(s);
				}
				else
					DTO("There is no shutdown event in the stack!");
				return;
			}
		}
		DTOP("shutdown:\n\tForces all process to terminate and shutdowns the system."
			" If you like to shutdown in a specific time, call with \"-time\" paremeter (In 24 Hour format!),"
			" you can also set specific seconds to close, call with \"-seconds\" parameter. For cancelling "
			" shutdown operation in time or seconds mode call with \"-cancel\". For rebooting add \"-r\" paremeter. Please look at the Usage information for examples."
			"\nUsage:\n\tshutdown [-now -time -seconds -cancel] [TIME or SECONDS] [-r]\n\tshutdown -now\n"
			"\tshutdown -now -r\n"
			"\tshutdown -time 23:59\n"
			"\tshutdown -time 23:59 -r\n"
			"\tshutdown -seconds 20\n"
			"\tshutdown -seconds 20 -r\n"
			"\tshutdown -cancel\n", TRUE);
		return;
	}
	if(_mup(_CA[CA_SETROWS]) || _mup(_CA[CA_SETCOLS]))
	{
		BOOL SetCols = _mup(_CA[CA_SETCOLS]);
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s;
			s =m_Command.Right(m_Command.GetLength()-x-1);
			s.TrimLeft();
			s.TrimRight();
			if(s.GetLength())
			{
				x = atoi(s);
				if(x>1)
				{
					if(SetCols)
						m_RowCols.Width = x;
					else
						m_RowCols.Height = x;
					m_CurrentPos = 0;
					DTO("Now on display will show \"" + s + (SetCols?"\" columns!":"\" rows!"));
					InvalidateRect(&m_rConsole);
					ReSetWindowPos();
				}
				else
					DTO("Error:\nMinimum row/column size is 2!");
				return;
			}
		}
		DTO("SetRows:\n\tChanges the consoles row size.\nUsage:\n\tSetRows Size\n");
		return;
	}
	if(_mup(_CA[CA_SETTRANSPARENT]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			s.TrimLeft();
			s.TrimRight();
			if(s.GetLength())
			{
				x = atoi(s);
				if( x >= 20 && x <= 100)
				{
					x = x * 255 /100;
					SetTransparent(this, 0, x, LWA_ALPHA );
					IniYazOku("eaxMain", "Transperacy", x, FALSE);
					DTO("Transparacy set to \"" + s + "\"!");
					return;
				}
			}
		}
		DTO("SetTransparent:\n\tChanges the consoles transparency value in percent. You can set this value between 20% and 100%. Windows 2000/XP and beyond only!\nUsage:\n\tSetTransparent TransparencyPercent\n");
		return;
	}
	if(_mup(_CA[CA_SETFONTSIZE]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			s.TrimLeft();
			s.TrimRight();
			if(s.GetLength())
			{
				m_FontSize = atoi(s);
				ResetFont();
				InvalidateRect(&m_rConsole);
				ReSetWindowPos();
				DTO("Now on font size will be \"" + s + "\"!");
				return;
			}
		}
		DTO("SetFontSize:\n\tChanges the consoles font size.\nUsage:\n\tSetFontSize Size\n");
		return;
	}
	if(_mup(_CA[CA_SETFONT]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			s.TrimLeft();
			s.TrimRight();
			if(s.GetLength())
			{
				m_FontName = s;
				ResetFont();
				Invalidate();
				ReSetWindowPos();
				DTO("Now on font will be \"" + s + "\"!");
				return;
			}
		}
		DTO("SetFont:\n\tChanges the consoles font.\nUsage:\n\tSetFont Font Name\n");
		return;
	}
	if(_mu(_CA[CA_LIST_1]) || _mu(_CA[CA_LIST_2])) //Büyük liste
	{
		if(m_ExecList.GetSize())
		{
			CString Buff;
			CExecuteLink * pLink;
			Buff = "Execute List:\n";
			for(int i= 0; i < m_ExecList.GetSize(); i++)
			{
				pLink = (CExecuteLink *) m_ExecList[i];
				Buff += "\"" + pLink->m_ExecLink + "\" points to: \"" + pLink->m_FileName + "\"\n";
			}
			DTOP(Buff, TRUE);
		}
		else
			DTO("Your Execute Library is empty!");
		return;
	}
	if(_mu(_CA[CA_DIR_1]) || _mu(_CA[CA_DIR_2])) //Küçük "more readable" liste
	{
		if(m_ExecList.GetSize())
		{
			CString Buff, bf;
			CExecuteLink * pLink;
			Buff = "Small Execute List - type \"list\" for more info:\n";
			for(int i= 0; i < m_ExecList.GetSize(); i++)
			{
				pLink = (CExecuteLink *) m_ExecList[i];
				bf =  pLink->m_ExecLink + "      ->      \"" + pLink->m_FileName + "\"";
				if(bf.GetLength() > (int) m_RowCols.Width)
				{
					bf = bf.Left(m_RowCols.Width +(m_RowCols.Width>3?-3:0))
					+ "...";
				}
				Buff += bf + "\n";
			}
			DTOP(Buff, TRUE);
		}
		else
			DTO("Your Execute Library is empty!");
		return;
	}
	if(_mup(_CA[CA_CMD_1]) || _mup(_CA[CA_CMD_2]) ||
		_mup(_CA[CA_WEB]) || _mup(_CA[CA_EMAIL]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s,s1;
			s1 = s = m_Command.Right(m_Command.GetLength()-x-1);
			s1 = s1.Right(s1.GetLength() - ParseCommand(s));
			ParseCommand(s1);
			DTO("Executing command \"" + s + "\"...");
			if (_mup("email"))
				s = "mailto:" + s;
			if(_mup("web") && s.Find("www") == -1)
				s = "www." + s;
			Execute( s, s1);
			return;
		}
		DTO(m_Command + ":\n\tExecutes writed command.\nUsage:\n\t" + m_Command + 
			" CommandToExecute\n\t" + m_Command + " erakan@firma.bursa.gen.tr\n\t" 
			+ m_Command + " winamp mp3s.pls\n\t" + m_Command + " www.yahoo.com");
		return;
	}
	if(_mup(_CA[CA_RENAME_1]) || _mup(_CA[CA_RENAME_2]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s,s1;
			s1 = s = m_Command.Right(m_Command.GetLength()-x-1);
			s1 = s1.Right(s.GetLength() - ParseCommand(s));
			ParseCommand(s1);
			if(s.GetLength() && s1.GetLength())
			{
				if( (x = FindInList(s)) != -1)
				{
					if(FindInList(s1) == -1)
					{
						for(int o=0; o< sizeof(_CA) / sizeof(LPTSTR); o++)
							if(s1.CompareNoCase(_CA[o]) == 0)
							{
								DTO("Can't rename, \"" + s1 + "\" is an internal command!");
								return;
							}
						CExecuteLink *pLink = (CExecuteLink *) m_ExecList[x];
						pLink->m_ExecLink = s1;
						DTO("\"" + s + "\" link changed to \"" + s1 + "\"");
						SaveList();
						return;
					}
					DTO("You already have a link named \"" + s1 + "\".");
					return;
				}
				DTO("Can't find link \"" + s + "\".");
				return;
			}
			m_Command = m_Command.Left(x);
		}
		DTO(m_Command + ":\n\tRenames link.\nUsage:\n\t" + m_Command + 
			" OldLinkName NewLinkName\n\t" + m_Command + " winamp wa\n");
		return;
	}
	if(_mup(_CA[CA_DELETEALL]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{

			CString s;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			ParseCommand(s);
			if(s == "YES")
			{
				if(m_ExecList.GetSize())
				{
					m_ExecList.RemoveAll();
					DTO("All links deleted! Don't forget to save database!");
					return;
				}
				else
				{
					DTO("Your Execute Library is empty!");
					return;
				}
			}
		}
		DTO(m_Command + ":\n\tDeletes all links from execute library, if you sure you want to delete all links call with YES parameter. To delete one link use \"delete\" command.\nUsage:\n\t" 
			+ m_Command + " YES");
		return;
	}
	if(_mup(_CA[CA_DELETE_1]) || _mup(_CA[CA_DELETE_2]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s,s1;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			ParseCommand(s);
			if(s.GetLength())
			{
				int z = FindInList(s);
				if(z != -1)
				{
					m_ExecList.RemoveAt(z);
					DTO("Link \"" + s + "\" deleted.");
					return;
				}
				DTO("Can't find link \"" + s + "\".");
				return;
			}
			m_Command = m_Command.Left(x);
		}
		DTO(m_Command + ":\n\tDeletes one link from execute library. To delete all links use \"deleteall\" command.\nUsage:\n\t" + m_Command + 
			" LinkToDelete\n\t" + m_Command + " winamp");
		return;
	}
	if(_mu(_CA[CA_SAVE]))
	{
		if(SaveList())
			DTO("Execute Library saved!");
		else
			DTO("An error occoured when tyring to save file!\nIf you are saving an empty library then everything is OK!\nIf you not, please try again!");
		return;
	}
	if(_mu(_CA[CA_RESETEAX]))
	{
		if(SaveList(TRUE))
			DTO("Execute Library re-loaded!");
		else
			DTO("An error occoured when tyring to load file, please try again!");
		return;
	}
	if(_mup(_CA[CA_SETTIMEFORMAT]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			if(s.GetLength())
			{
				m_Clock.SetClockStr(s);
				IniYazOku("eaxClock", "ClockFormat", s, NULL, FALSE);
				return;
			}
			m_Command = m_Command.Left(x);
		}
		DTO(m_Command + ":\n\tSets time format for the clock. For format specifications reference look at the readme.txt. You can use strftime format specifications.\nUsage:\n\t" + m_Command + 
			" TimeFormatString\n\t" + m_Command + " %#c");
		return;
	}
	if(_mup(_CA[CA_SETTIMETIP]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			CString s;
			s = m_Command.Right(m_Command.GetLength()-x-1);
			if(s.GetLength())
			{
				m_Clock.SetTipStr(s);
				IniYazOku("eaxClock", "TipFormat", s, NULL, FALSE);
				return;
			}
			m_Command = m_Command.Left(x);
		}
		DTO(m_Command + ":\n\tSets time format for the clock tip. For format specifications reference look at the readme.txt. You can use strftime format specifications.\nUsage:\n\t" + m_Command + 
			" TimeFormatString\n\t" + m_Command + " %#c");
		return;
	}
	if(_mu(_CA[CA_SETBITMAP]))
	{
		CFileDialog fd(TRUE);
		CString filename;
		fd.m_ofn.lpstrFile = filename.GetBuffer(MAX_PATH);
		fd.m_ofn.lpstrFilter = "Bitmap files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
		fd.m_ofn.lpstrTitle = "Please choose background bitmap.";
		fd.m_ofn.Flags |= OFN_EXPLORER;
		if(fd.DoModal() == IDOK)
		{
			if(OpenBitmap(filename, &m_BackBitmap))
			{
				IniYazOku("eaxMain", "BackBitmap", filename, NULL, FALSE);
				m_BackGroundBrush.DeleteObject();
				ResetBitmap();
				m_BackGroundBrush.CreatePatternBrush(&m_BackBitmap);
				InvalidateRect(&m_rConsole);
			}
		}
		filename.ReleaseBuffer();
		return;
	}
	if(_mup(_CA[CA_STARTUP]))
	{
		int x = m_Command.Find(" ");
		if(x != -1)
		{
			int cmd = atoi(m_Command.Right(m_Command.GetLength()-x-1));
			if(cmd == 1 || cmd == 0)
			{
				if(cmd == 1)
					runstartup_set("eaxTerm");
				else
					runstartup_remove("eaxTerm");
				return;
			}
			m_Command = m_Command.Left(x);
		}
		DTO(m_Command + ":\n\tStarts the eaxTerm when Windows starts.\n\t\tTo set use: " + m_Command + " 1\n\t\tTo remove use: " + m_Command + " 0");
		return;
	}

	if(_mu(_CA[CA_SETTEXTCOLOR]))
	{
		CColorDialog ccd(m_clrTextColor, CC_FULLOPEN, this);
		if(ccd.DoModal() == IDOK)
		{
			m_clrTextColor = ccd.GetColor();
			IniYazOku("eaxMain", "TextColor", m_clrTextColor, FALSE);
			InvalidateRect(&m_rConsole);
		}
		return;
	}
	// Search for the keyword...
	int z = m_Command.Find(" ");
	for(int x=0; x < m_ExecList.GetSize(); x++)
	{
		CExecuteLink * pLink = (CExecuteLink *) m_ExecList[x];
		if(z == -1)
		{
			if(_mu(pLink->m_ExecLink))
			{
				DTO("Executing link \"" + pLink->m_ExecLink + "\"...");
				Execute(pLink->m_FileName, "");
				return;
			}
		}
		else
		{
			if(_mup(pLink->m_ExecLink))
			{
				CString s;
				s = m_Command.Right(m_Command.GetLength()-z-1);
				DTO("Executing link \"" + pLink->m_ExecLink + "\"...");
				Execute(pLink->m_FileName, s);
				return;
			}
		}
	}
	// Bu noktada komut bulunamadı oluyor!
	DTO("\"" + m_Command + "\" is not recognized as an internal or external command, operable program or batch file link.");
}

void CMainFrame::Version()
{
				DTO("erdincakan Xtreme Terminal - Version 0.09 FFN FreeWare (BETA2)\n"
				   "Copyright © 2003 Erdinc AKAN - erakan@softhome.net\n"
				   "All rights reserved.\n"
				   "You can freely distribute this software unless you alter code or setup package!\n\n"
				   "         \"The Power you need is in the noble blood, flowing in your veins.\"\n"
				   "                                                    October 20, 1927\n"
				   "                                                 Mustafa Kemal Ataturk\n"
				   "                                           Founder of The Turkish Republic\n\n"
				   );
}

BOOL CMainFrame::_mu(LPCTSTR Command)
{
	if(m_Command.CompareNoCase(Command) == 0)
		return TRUE;
	return FALSE;
}

void CMainFrame::Cursor(BOOL Clear)
{
	CClientDC dc(this);
	CRect r(m_CursorPos.y, m_CursorPos.x,
			m_CursorPos.y + (m_rConsole.Width() / m_RowCols.Width),
			m_CursorPos.x + (m_rConsole.Height() / m_RowCols.Height)), r3d;
	r.DeflateRect(1,2);
	if(r.bottom > m_rConsole.Height())
		return;
	r3d =r;
#define SHADOW_PXX 1
		r3d.top+=SHADOW_PXX; r3d.right+=SHADOW_PXX; r3d.bottom+=SHADOW_PXX; r3d.left+=SHADOW_PXX;
	if(Clear)
	{
		dc.FillRect(&r3d, &m_BackGroundBrush);
		dc.FillRect(&r, &m_BackGroundBrush);
	}
	else
	{
		dc.FillSolidRect(&r3d,RGB(0,0,0));
		dc.FillSolidRect(&r,m_clrTextColor);
	}
	m_bCursorVis = Clear;
}

BOOL CMainFrame::_mup(LPCTSTR Command)
{
	CString s;
	int z = m_Command.Find(" ");
	if(z==-1)
		s = m_Command;
	else
		s = m_Command.Left(z);
	if(s.CompareNoCase(Command) == 0)
		return TRUE;
	return FALSE;
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) 
	{
	case VK_UP:
		if(m_CommandBuffer.GetSize())
		{
			if(m_CommandBuffer.GetSize() > abs(m_ComBuffPos))
			{
				m_ComBuffPos--;
				m_Command = m_CommandBuffer[m_CommandBuffer.GetSize() + m_ComBuffPos];
				DrawTextLine();
			}
		}
		break;

	case VK_DOWN:
		if(m_CommandBuffer.GetSize())
		{
			if(m_ComBuffPos < -1)
			{
				m_ComBuffPos++;
				m_Command = m_CommandBuffer[m_CommandBuffer.GetSize() + m_ComBuffPos];
				DrawTextLine();
			}
		}
		break;

	case VK_LEFT:
		break;

	case VK_RIGHT:
		break;

	case VK_HOME:
		break;

	case VK_END:
		break;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::Help()
{
	DTOP(
		"\tThis program associates folders, programs and many more with keywords. "
		"It is designed for removing all shortcuts from your desktop and start menu. "
		"Because you will never need them again. You can set a keyword like \"ie\" for "
		"Internet Explorer and you can run it just typing ie! With eaxTerm you will never "
		"search icons that are hidden who know where!\n"
		"\tWith eaxTerm, you can run dos and system commands, open documents, surf the web and send e-mails easy as writing e-mail addresses! "
		"And you can costumize eaxTerm in the way you like, please take a look at available commands and try to find your best!\n"
		"\tWhenever, whereever you are when you need eaxTerm, just press the key that has Winindows logo on it and S (\"Win+S\"). "
		"eaxTerm will come along and save the day!\n"
		"\tBars that at the end of the window are memory bars, the left presents your total memory limit and usage and the right "
		"presents your physcal memory.\n"
		"\tTo add your links, just select and drop them to the eaxTerm window, they will be added to the library and ready for you to execute "
		"or modify them.\n"
		"\tTo move eaxTerm window, just click on it, and move!\n"
		"\tIf you like this program or not, please send an e-mail to me, including your thoughts and questions.\n\n"
		"\t\tErdinc AKAN\n\t\tProgrammer\n"
		"Available commads are:\n"
		"help or ?:\n\tBrings this info.\n"
		"cls or clrscr:\n\tClears the screen.\n"
		"ver or version:\n\tDisplays the eaxTerm version.\n"
		"command or cmd:\n\tExecutes given command.\n"
		"web:\n\tOpens given web address in default browser.\n"
		"email:\n\tOpens default e-mail composer to send e-mail for given address.\n"
		"time:\n\tShows the current time.\n"
		"list:\n\tLists commands that you added.\n"
		"dir:\n\tLists commands that you added, more readable than list!\n"
		"rename or ren:\n\tRenames given link from library.\n"
		"delete or del:\n\tDeletes given link from library.\n"
		"deleteall:\n\tDeletes all links from library.\n"
		"save:\n\tSaves link library.\n"
		"reset_eax:\n\tRe-loads link library.\n"
		"setfont:\n\tSets the terminal font.\n"
		"setfontsize:\n\tSets the terminals font size.\n"
		"setrows:\n\tSets the terminals row.\n"
		"setcols:\n\tSets the terminals cols.\n"
		"settimeformat:\n\tSets time format for time control.\n"
		"settimetip:\n\tSets time tip for the time control. Time tip is what you see when you point your mouse on time control for a second or two.\n"
		"setbitmap:\n\tSets consoles background bitmap.\n"
		"settransparent:\n\tChanges the terminals transparency value. (Windows 200/XP and beyond only.)\n"
		"setontop:\n\tSets the terminal always on top of other windows.\n"
		"cd:\n\tOpens or closes the given CD-ROM's tray.\n"
		"exit, bye, byz, close or quit:\n\tExits from the eaxTerm.\n"
		"To get more help about a command, type its name and press enter."
		,TRUE);
}

UINT CMainFrame::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
     CString FileName, s, output;
     UINT numFiles;
	 int x;
	 numFiles = DragQueryFile(hDropInfo, (UINT) -1, NULL, (UINT) 0);
	 Cursor(TRUE);
	 m_CurrentPos++;
	 output.Empty();
	 for(UINT i=0; i<numFiles;i++)
	 {
		 UINT BuffSize;
		 BuffSize = ::DragQueryFile(hDropInfo, i, NULL, 0) + 1;
		 ::DragQueryFile(hDropInfo, i, FileName.GetBufferSetLength(BuffSize * sizeof(TCHAR)), BuffSize);
		 if(FileName.IsEmpty())
			 continue;
		 FileName.Delete(FileName.GetLength()-1);
		 s = FileName.Right(FileName.GetLength() - FileName.ReverseFind('\\') -1);
		 if((x = s.ReverseFind('.')) !=-1 )
			 s= s.Left(x);
		 output += "Gotcha!: \"" + FileName + "\"\n";
				   
		 if(FindInList(FileName, TRUE) == -1)
		 {
			 if(FindInList(s) != -1)
			 {
				 CString Num, sFind;
				 int p = 0;
				 sFind = s;
				 while(FindInList(sFind) != -1)
				 {
					 p++;
					 sFind = s;
					 Num.Format("_%d", p);
					 sFind += Num;
				 }
				 output += "You already have a link named \"" + s + 
						"\", new link renamed to \"" + sFind + "\" and added to the execute library!";
				 s = sFind;
			 }
			 else
				output += "Adding as \"" + s + "\" to the execute library.";
			 CExecuteLink *pNewLink = new CExecuteLink(s, FileName);
			 m_ExecList.Add(pNewLink);
			 SaveList();
		 }
		 else
			 output += "You already have a link for this file, skipping!";
		 s.Empty();
		 FileName.Empty();
	 }
	 GetFocus();
	 ::DragFinish(hDropInfo);
	 DTO(output);
    if(!m_PressLock)
		DrawTextLine();
	CWnd ::OnDropFiles(hDropInfo);
}

void CMainFrame::Execute(CString File, CString Parameters)
{
	long  Durum = (long)::ShellExecute(NULL, NULL, File, Parameters , NULL, SW_SHOWNORMAL);
	switch(Durum)
	{
	case 0:
		DTO("Error executing command!\nSystem returned:\"Operating system, out of memory or resources!\"\n");
		break;
	case ERROR_FILE_NOT_FOUND:
		DTO("Error executing command!\nSystem returned:\"File not found!\"\n");
		break;
	case ERROR_PATH_NOT_FOUND:
		DTO("Error executing command!\nSystem returned:\"Path not found!\"\n");
		break;
	case ERROR_BAD_FORMAT:
		DTO("Error executing command!\nSystem returned:\"Not a Win32 executable!\"\n");
		break;
	case SE_ERR_ACCESSDENIED:
		DTO("Error executing command!\nSystem returned:\"Access denied!\"\n");
		break;
	case SE_ERR_ASSOCINCOMPLETE :
		DTO("Error executing command!\nSystem returned:\"File name association incomplete or invalid!\"\n");
		break;
	case SE_ERR_DDEBUSY:
		DTO("Error executing command!\nSystem returned:\"DDE busy!\"\n");
		break;
	case SE_ERR_DDEFAIL:
		DTO("Error executing command!\nSystem returned:\"DDE fail!\"\n");
		break;
	case SE_ERR_DDETIMEOUT:
		DTO("Error executing command!\nSystem returned:\"DDE timeout!\"\n");
		break;
	case SE_ERR_DLLNOTFOUND:
		DTO("Error executing command!\nSystem returned:\"Specified DLL not found!\"\n");
		break;
	case SE_ERR_NOASSOC:
		DTO("Error executing command!\nSystem returned:\"No application found that suitable with this document!\"\n");
		break;
	case SE_ERR_OOM:
		DTO("Error executing command!\nSystem returned:\"Not enough memory to complete your request!\"\n");
		break;
	case SE_ERR_SHARE:
		DTO("Error executing command!\nSystem returned:\"Share voilation occured!\"\n");
		break;
	default:
		DTO("Success!");
	}
}

int CMainFrame::ParseCommand(CString &String)
{
	BOOL Qs = FALSE;
	int SelStart = 0, i = 0;

	String.TrimLeft();
	String.TrimRight();

	for(; i< String.GetLength(); i++)
	{
		if(String[i] == '"' && Qs) //" için Bitiş..
		{
			break;
		}
		if(String[i] == '"') // Eğer " başlamış ise sonuna kadar arayalım
		{
			Qs = TRUE;
			SelStart = i+1;
		}
		if(String[i] == ' ' && !Qs) // " başlamamış ise iş bitti...
			break;
	}
	String = String.Mid(SelStart, i - SelStart );
	if(Qs)
		i++; // sondaki " için uzama
	return i;
}

int CMainFrame::FindInList(LPCTSTR String, BOOL FileName)
{
	for(int x=0; x < m_ExecList.GetSize(); x++)
	{
		CExecuteLink * pLink = (CExecuteLink *) m_ExecList[x];
		if(FileName)
		{
			if(pLink->m_FileName.CompareNoCase(String) == 0)
				return x;
		}
		else
		{
			if(pLink->m_ExecLink.CompareNoCase(String) == 0)
				return x;
		}
	}
	return -1;
}

int CMainFrame::SaveList(BOOL bOku)
{
	CString fName;
	::GetModuleFileName(NULL,fName.GetBufferSetLength(MAX_PATH), MAX_PATH * sizeof(TCHAR));
	fName.FreeExtra();
	fName = fName.Left(fName.ReverseFind('\\'));
	fName += "\\eaxTerm.eav";
	if(DosyadanListe(m_ExecList, fName, bOku) == 1)
		return TRUE;
	return FALSE;
}
/*
	DosyadanListe Fonksiyonu:

	Return Değerleri:
	0 Hiç bir zaman 0 dönmez! Dönerse hata var...
	1 Herşey Tamam!
	2 Dosya açılamıyor, iç hata oluştu.
	3 DosyaKontrol hatası.
	4 Dosya boş hatası (empty file, filesize=0) 
	5 Dosya yok!
*/
int CMainFrame::DosyadanListe(CObArray &mListe, CString filename, BOOL bOku)
{
	CFileStatus cfstatus;
	CFile cf;
	if(bOku)
	{
		if(CFile::GetStatus(filename, cfstatus))
		{
			if(cfstatus.m_size>0)
			{
				if(!cf.Open(filename, CFile::modeRead | CFile::modeNoTruncate))
					return 2;
					CArchive ar(&cf, CArchive::load);
					mListe.Serialize(ar);
					ar.Close();
				cf.Close();
			}
			else
				return 4;
		}
		else
			return 5;
	}
	else
	{
		if(!cf.Open(filename, CFile::modeWrite| CFile::modeNoTruncate | CFile::modeCreate))
			return 2;
		if(mListe.GetSize())
		{
			CArchive ar(&cf, CArchive::store);
			mListe.Serialize(ar);
			ar.Close();
		}
		else //Liste boş, var olan dosyayı silelim..
		{
			cf.Close();
			CFile::Remove(filename);
		}
	}
	return 1;
}

void CMainFrame::ReSetWindowPos()
{
	CClientDC dc(this);
	CFont *pOldFont;
	ResetBitmap();
	pOldFont = dc.SelectObject(&m_Font);
	m_rConsole.SetRectEmpty();
	dc.DrawText("A", m_rConsole, DT_SINGLELINE | DT_CALCRECT);
	dc.SelectObject(pOldFont);
	m_rConsole.right *= m_RowCols.Width;
	m_rConsole.bottom *= m_RowCols.Height;
	SetWindowPos(NULL,NULL,NULL, 
		m_rConsole.Width() + (2*::GetSystemMetrics(SM_CYFIXEDFRAME)),
		m_rConsole.Height() + (2*::GetSystemMetrics(SM_CYFIXEDFRAME)) 
		+ 5 + ::GetSystemMetrics(SM_CYSMCAPTION) + 2,
		SWP_NOMOVE | SWP_NOZORDER);
	m_rConsole.left++;
	m_rConsole.right++;
#ifdef _DEBUG
	ShowWindow(SW_SHOW);
#endif
}

void CMainFrame::PrintAndWait(CString Str)
{
	int CutPos = 0;
	//CString buffback = m_strConsole;
	Printf(Str, TRUE, FALSE, FALSE, FALSE, &CutPos);
	if(CutPos == Str.GetLength())
		Printf(Str, TRUE);
	else
	{
		Printf(Str, TRUE, TRUE, FALSE, FALSE, &CutPos);
		Str.Insert(CutPos, "Press any key to continue...\n");
		Printf(Str, TRUE, TRUE, FALSE, TRUE);
		InvalidateRect(&m_rConsole, TRUE);
		/*
				CString StrBk;
		Printf(Str, TRUE, TRUE, FALSE, FALSE, &CutPos);
		StrBk = Str.Left(CutPos);
		Str.Delete(0, CutPos);
		m_strConsole += StrBk + "Press any key to continue...\n";
		m_strContText = Str;
		m_PressLock = TRUE;
		//Printf(Str, TRUE, TRUE, FALSE, TRUE);
		InvalidateRect(&m_rConsole, TRUE);
		*/
	}
}

void CMainFrame::Printf(CString Text, BOOL Count, BOOL Paint, BOOL CountForInput, BOOL PauseAfter, int *DoCalculate)
{
	CClientDC dc(this);
	CFont *pOldFont;
	CRect rChar;
	UINT nFormat;
	CString str;
	int i, PauseCount, SavedBuffPos;
	BOOL CalculateMode = DoCalculate!=NULL;
	BOOL CallInvalidate = FALSE;
	PauseCount = 0;
	nFormat = DT_NOPREFIX | DT_EXPANDTABS | DT_SINGLELINE;
	pOldFont = dc.SelectObject(&m_Font);
	dc.SetBkMode(TRANSPARENT);

	if(Paint || (PauseAfter && (m_CurrentPos == (m_RowCols.Height))))
	{
		m_strConsole.Empty();
		m_CurrentPos = 0;
	}

	if(CalculateMode)
	{
		SavedBuffPos = m_CurrentPos;
	}
	rChar.SetRectEmpty();
	rChar = m_rConsole;
	rChar.top = m_CurrentPos * (m_rConsole.bottom / m_RowCols.Height);
	rChar.right = m_rConsole.right;
	for(i = 0; i < Text.GetLength(); i++)
	{
		str += Text[i];
		dc.DrawText(str, &rChar, nFormat | DT_CALCRECT);
		if(rChar.right > m_rConsole.right && str[str.GetLength()-1] != '\n')
		{
			str.Delete(str.GetLength() -1);
			str += "\n";
			i--;
		}
		if(i == Text.GetLength() -1 && str[str.GetLength()-1] != '\n')
			str += "\n";
		if(str[str.GetLength() - 1] == '\n')
		{
			CRect r3d;
			r3d = rChar;
			if(CalculateMode && m_CurrentPos == (m_RowCols.Height - 2))
			{
				m_CurrentPos = SavedBuffPos;
				*DoCalculate = i+1;
				break;
			}
			if(m_CurrentPos == m_RowCols.Height)
			{
				{
					if(Count)
					{
						Text.Delete(0, i+1);
						m_strConsole += str + Text;
					}
					CallInvalidate = TRUE;
				}
				break;
			}
			if(Count)
				m_strConsole += str;
			if(!CalculateMode)
			{
				if(CountForInput)
				{
					CRect rErase;
					rErase = rChar;
					rErase.right = m_rConsole.right;
					dc.FillRect(&rErase, &m_BackGroundBrush);
				}
				str.Remove('\n');
#define SHADOW_PXX 1
				
#if 1
				r3d.top+=SHADOW_PXX; r3d.right+=SHADOW_PXX; r3d.bottom+=SHADOW_PXX; r3d.left+=SHADOW_PXX;
				dc.SetTextColor(RGB(0,0,0));
				dc.DrawText(str, &r3d, nFormat);
				dc.SetTextColor(m_clrTextColor);
				dc.DrawText(str, &rChar, nFormat);
#else
				dc.BeginPath();
				dc.DrawText(str, &rChar, nFormat);
				dc.EndPath();
				int oldRop = dc.SetROP2(R2_XORPEN);
				CBrush brush;
				brush.CreateSolidBrush(TEXT_COLOR);
				CBrush *oldBrush = dc.SelectObject(&brush);
				dc.FillPath();
				dc.SelectObject(oldBrush);
				dc.SetROP2(oldRop);
#endif
			}
			if(Count)
				m_CurrentPos++;
			rChar.top = m_CurrentPos * (m_rConsole.bottom / m_RowCols.Height);
			rChar.bottom = rChar.top + (m_rConsole.bottom / m_RowCols.Height);
			rChar.left = m_rConsole.left;
			str.Empty();

			if(PauseAfter && m_CurrentPos == (m_RowCols.Height -1))
			{
				Text.Delete(0, i+1);
				if(Text.GetLength())
				{
					m_strContText = Text;
					m_PressLock = TRUE;
					DrawTextLine("Press any key to continue...");
				}
				CountForInput = FALSE;
				break;
			}
		}
	}
	if(CountForInput)
	{
		m_CursorPos.x = rChar.top;
		m_CursorPos.y = rChar.right;
	}
	dc.SelectObject(pOldFont);
	if(CallInvalidate)
		FlipScreen();
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect r;
	GetWindowRect(&r);
	dc.FillSolidRect(&r, RGB(0,0,0));
//	if(!m_StretchMode)
	dc.FillRect(&m_rConsole, &m_BackGroundBrush);
/*	else
	{
		CDC dcMem;
		BITMAP bmp;
		CBitmap *pOldBitmap;
		m_BackBitmap.GetBitmap(&bmp);
		dcMem.CreateCompatibleDC(&dc);
		pOldBitmap = dcMem.SelectObject(&m_BackBitmap);
		dc.StretchBlt(0,0, m_rConsole.Width(), m_rConsole.Height(), &dcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		dcMem.SelectObject(pOldBitmap);
	}*/
	Printf(m_strConsole, TRUE, TRUE);
	if(!m_PressLock)
		DrawTextLine();
	DrawMemoryBar(&dc);
	r = m_rConsole;
	r.top = r.bottom + 7;
	r.bottom = r.top + ::GetSystemMetrics(SM_CYSMCAPTION);
	r.left++;
	m_Clock.MoveWindow(&r);
}

void CMainFrame::FlipScreen()
{
	while( m_strConsole[0] != '\n')
		m_strConsole.Delete(0);
	m_strConsole.Delete(0);
	InvalidateRect(&m_rConsole, TRUE);
	/*CPaintDC dc(this);
	dc.FillRect(&m_rConsole, &m_BackGroundBrush);
	Printf(m_strConsole, TRUE, TRUE);
	if(!m_PressLock)
		DrawTextLine();*/
	return;
	/*ScrollWindow(0, -m_LineHeight, &m_rScroll);
	ValidateRect(&m_rScroll);
	CClientDC dc(this);
	CRect	r(0, (m_SatirWidthHeight.cy * m_CurrentPos), m_SatirWidthHeight.cx,
							(m_SatirWidthHeight.cy * m_CurrentPos) + m_SatirWidthHeight.cy);
	dc.FillRect(&r, &m_BackGroundBrush);*/
}


void CMainFrame::ResetFont()
{
	if ((HFONT) m_Font != NULL)
            m_Font.DeleteObject();
	m_Font.CreateFont(m_FontSize, 0, 0, 0, FW_BOLD,
            0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
            CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
            FF_DONTCARE, m_FontName);
}

LRESULT CMainFrame::OnTrayNotify(WPARAM uID, LPARAM lEvent)
{
	if(lEvent == WM_LBUTTONUP)
		SetForegroundWindow();
	return 1;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		CClientDC dc(this);
		DrawMemoryBar(&dc);
	}
	if(nIDEvent == 2)
	{
		Cursor(!m_bCursorVis);
	}
	if(nIDEvent == 5)
	{
		if(m_nShutDownRemain == 0)
		{
			::KillTimer(this->GetSafeHwnd(), nIDEvent);
			DTO("Shutting down system! Have a nice day...");
			ShutDownAll();
		}
		if(m_nShutDownRemain >0)
		{
			if((m_nShutDownRemain < 11 || ((m_nShutDownRemain % 5)==0 && m_Command.IsEmpty()))
				&& !m_PressLock)
			{
				CString s;
				Cursor(TRUE);
				if(m_bShutDownReBoot)
					s = " and reboot";
				s.Format("peaxTerm->Message: %d seconds left to shutdown" + s + "!" ,m_nShutDownRemain);
				DTO(s);
				DrawTextLine();
				Cursor(FALSE);

			}
			m_nShutDownRemain--;
		}
		
	}
}

void CMainFrame::Settings(BOOL Read)
{
	if(!Read)
	{
		IniYazOku("eaxMain", "Rows", m_RowCols.Height, FALSE);
		IniYazOku("eaxMain", "Cols", m_RowCols.Width, FALSE);
		IniYazOku("eaxMain", "OnTop", m_OnTop, FALSE);
		IniYazOku("eaxMain", "FontSize", m_FontSize, FALSE);
		IniYazOku("eaxMain", "FontName", m_FontName, NULL, FALSE);
	}
	else
	{
		CString filename;
		IniYazOku("eaxMain", "BackBitmap", "", filename.GetBuffer(MAX_PATH));
		filename.ReleaseBuffer();
		if(!filename.IsEmpty())
		{
			if(OpenBitmap(filename, &m_BackBitmap))
			{
				ResetBitmap();
			}
			else
				m_BackBitmap.LoadBitmap(IDB_BITMAP1);
		}
		else
			m_BackBitmap.LoadBitmap(IDB_BITMAP1);
		m_RowCols.Height = IniYazOku("eaxMain", "Rows", 20);
		m_RowCols.Width = IniYazOku("eaxMain", "Cols", 80);
		m_OnTop = IniYazOku("eaxMain", "OnTop", FALSE);
		m_FontSize = IniYazOku("eaxMain", "FontSize", 12);
		m_clrTextColor = IniYazOku("eaxMain", "TextColor", m_clrTextColor);
		IniYazOku("eaxMain", "FontName", "Verdana", m_FontName.GetBufferSetLength(1024));
		m_FontName.FreeExtra();
	}
}

LRESULT CMainFrame::OnPowerBroadCast(WPARAM dwPowerEvent, LPARAM dwData)
{
	if((long)dwPowerEvent == PBT_APMSUSPEND)
		Settings();
	return TRUE;
}

LRESULT CMainFrame::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	SetForegroundWindow();
	return TRUE;
}

void CMainFrame::OnClose() 
{
	systray_del(GetSafeHwnd());
	UnregisterHotKey(GetSafeHwnd(), 1);
	Settings();
	CWnd ::OnClose();
}

void CMainFrame::OnMove(int x, int y) 
{
	CWnd ::OnMove(x, y);
	if(m_bMoving)
	{
		IniYazOku("eaxMain", "XY", 0, FALSE);
		IniYazOku("eaxMain", "Y", y, FALSE);
		IniYazOku("eaxMain", "X", x, FALSE);
		m_bMoving = FALSE;
	}
}

void CMainFrame::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd ::OnMoving(fwSide, pRect);
	m_bMoving = TRUE;
}

BOOL CMainFrame::OnNcActivate(BOOL bActive) 
{
	if(bActive)
	{
		SetTimer(2, 500, NULL);
		if(!m_PressLock)
			Cursor();
	}
	else
	{
		KillTimer(2);
		Cursor(TRUE);
	}
	return CWnd ::OnNcActivate(bActive);
}

void CMainFrame::ShutDownAll()
{
	DTO("Shutdown Process Started!");
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	OpenProcessToken(GetCurrentProcess(), 
	TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
	&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
	(PTOKEN_PRIVILEGES)NULL, 0); 

	if (!ExitWindowsEx(EWX_SHUTDOWN | (m_bShutDownReBoot?EWX_REBOOT:0) | EWX_FORCE, 0)) 
		DTO("Shutdown process terminated by Windows!");
}

void CMainFrame::DrawMemoryBar(CDC *pDC)
{
	MEMORYSTATUS ms;
	ms.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&ms);
	CRect r;
	r = m_rConsole;
	r.top = r.bottom;
	r.bottom += 5;
	r.right = r.Width() / 2 - 1;
	pDC->FillSolidRect(&r,  RGB(0,0,255));
	pDC->Draw3dRect(&r,  RGB(180,180,180), RGB(255,255,255));
	r.DeflateRect(1,1);
	r.right  = r.left + (r.Width() * ms.dwMemoryLoad / 100);
	pDC->FillSolidRect(&r, RGB(255,0,0));
	r = m_rConsole;
	r.top = r.bottom;
	r.bottom +=5;
	r.left = r.Width() / 2;
	pDC->FillSolidRect(&r,  RGB(0,255,0));
	pDC->Draw3dRect(&r,  RGB(180,180,180), RGB(255,255,255));
	r.DeflateRect(1,1);
	r.right  = r.left + (r.Width() * ( 100 -(100 * ms.dwAvailPhys / ms.dwTotalPhys)) / 100);
	pDC->FillSolidRect(&r, RGB(255,0,0));
}

void CMainFrame::ResetBitmap()
{
/*	BITMAP bmp;
	m_BackBitmap.GetBitmap(&bmp);
	if( bmp.bmWidth > m_rConsole.Width() || bmp.bmHeight > m_rConsole.Height())
		m_StretchMode = TRUE;
	else
		m_StretchMode = FALSE;*/
}
