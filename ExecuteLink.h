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

// ExecuteLink.h: interface for the CExecuteLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXECUTELINK_H__1096B5EA_7201_48EF_B8D3_430C12F3AE44__INCLUDED_)
#define AFX_EXECUTELINK_H__1096B5EA_7201_48EF_B8D3_430C12F3AE44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExecuteLink : public CObject  
{
	DECLARE_SERIAL(CExecuteLink)
public:
	void Serialize(CArchive &ar);
	CExecuteLink(CString &ExecLink, CString &FileName);
	CString m_FileName;
	CString m_ExecLink;
	CExecuteLink();
	virtual ~CExecuteLink();

};

#endif // !defined(AFX_EXECUTELINK_H__1096B5EA_7201_48EF_B8D3_430C12F3AE44__INCLUDED_)
