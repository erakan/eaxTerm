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

#define CA_EXIT_1			0
#define CA_EXIT_2			1
#define CA_EXIT_3			2
#define CA_EXIT_4			3
#define CA_EXIT_5			4
#define CA_EXIT_6			5
#define CA_HELP_1			6
#define CA_HELP_2			7
#define CA_VER_1			8
#define CA_VER_2			9
#define CA_TIME				10
#define CA_CD				11
#define CA_CLS_1			12
#define CA_CLS_2			13
#define CA_SETONTOP			14
#define CA_SHUTDOWN			15
#define CA_SETROWS			16
#define CA_SETTRANSPARENT	17
#define CA_SETFONTSIZE		18
#define CA_SETFONT			19
#define CA_LIST_1			20
#define CA_LIST_2			21
#define CA_DIR_1			22
#define CA_DIR_2			23
#define CA_CMD_1			24
#define CA_CMD_2			25
#define CA_WEB				26
#define CA_EMAIL			27
#define CA_RENAME_1			28
#define CA_RENAME_2			29
#define CA_DELETEALL		30
#define CA_DELETE_1			31
#define CA_DELETE_2			32
#define CA_SAVE				33
#define CA_RESETEAX			34
#define CA_SETTIMEFORMAT	35
#define CA_SETTIMETIP		36
#define CA_SETCOLS			37
#define CA_SETBITMAP		38
#define CA_STARTUP			39
#define CA_SETTEXTCOLOR		40

LPTSTR _CA[] = {
	"EXIT", "BYE", "BYZ", "CLOSE", "exıt", "quit",
	"help", "?", 
	"ver", "version",
	"time",
	"cd",
	"cls", "clrscr",
	"setontop",
	"shutdown",
	"setrows",
	"settransparent",
	"setfontsize",
	"setfont",
	"list", "lıst",
	"dır", "dir",
	"command", "cmd", "web", "email",
	"rename", "ren",
	"deleteall",
	"delete", "del",
	"save",
	"reset_eax",
	"settimeformat",
	"settimetip",
	"setcols", // Added 04 -> 05 Ekim 2003 Pazar
	"setbitmap", // Added 04 -> 05 Ekim 2003 Pazar
	"setstartup", // Added 18 Mart 2008 Salı
	"settextcolor", // Added 18 Mart 2008 Salı
};
