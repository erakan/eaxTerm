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

// stdafx.cpp : source file that includes just the standard includes
//	eaxTerm.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CString XFormatNumber(LPCTSTR lpszNumber, int nFracDigits)
{
	ASSERT(lpszNumber);
	ASSERT(lpszNumber[0] != 0);
	
	CString str = lpszNumber;

	NUMBERFMT nf;
	memset(&nf, 0, sizeof(nf));


	_TCHAR szBuffer[10];
	_TCHAR szDecSep[10];
	_TCHAR szThousandsSep[10];

	// set decimal separator string from locale default
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,
		szDecSep, sizeof(szDecSep)-1);
	nf.lpDecimalSep = szDecSep;

	// set thousand separator string from locale default
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND,
		szThousandsSep, sizeof(szThousandsSep)-1);
	nf.lpThousandSep = szThousandsSep;

	// set leading zeroes from locale default
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO,
		szBuffer, sizeof(szBuffer)-1);
	nf.LeadingZero = _ttoi(szBuffer);

	// set grouping of digits from locale default
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING,
		szBuffer, sizeof(szBuffer)-1);
	nf.Grouping = _ttoi(szBuffer);

	// set negative number mode from locale default
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER,
		szBuffer, sizeof(szBuffer)-1);
	nf.NegativeOrder = _ttoi(szBuffer);

	int dp = nFracDigits;

	if (dp == -1)
	{
		// set number of fractional digits from input string
		CString strDecSep = szDecSep;
		dp = str.Find(_T('.'));
		dp = (dp == -1) ? 0 : str.GetLength() - dp - strDecSep.GetLength();
	}
	nf.NumDigits = dp;

	int nSize = (_tcslen(lpszNumber) * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];
	ASSERT(pszFormattedNumber);

	if (::GetNumberFormat(LOCALE_USER_DEFAULT, 
						0, 
						lpszNumber, 
						&nf, 
						pszFormattedNumber, 
						nSize-1))
	{
		str = pszFormattedNumber;
	}
	else
	{
		if (ERROR_INVALID_PARAMETER == ::GetLastError())
			TRACE("INVALID PARAM");
	}

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;

	return str;
}


