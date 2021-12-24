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

// ExecuteLink.cpp: implementation of the CExecuteLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eaxTerm.h"
#include "ExecuteLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CExecuteLink, CObject, 1 | VERSIONABLE_SCHEMA)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExecuteLink::CExecuteLink()
{

}

CExecuteLink::~CExecuteLink()
{

}

CExecuteLink::CExecuteLink(CString &ExecLink, CString &FileName)
{
	m_ExecLink = ExecLink;
	m_FileName = FileName;
}

void CExecuteLink::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
		ar << m_FileName << m_ExecLink;
	else
		ar >> m_FileName >> m_ExecLink;
}
