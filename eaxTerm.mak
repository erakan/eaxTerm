# Microsoft Developer Studio Generated NMAKE File, Based on eaxTerm.dsp
!IF "$(CFG)" == ""
CFG=eaxTerm - Win32 Debug
!MESSAGE No configuration specified. Defaulting to eaxTerm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "eaxTerm - Win32 Release" && "$(CFG)" != "eaxTerm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eaxTerm.mak" CFG="eaxTerm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eaxTerm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "eaxTerm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eaxTerm - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\eaxTerm.exe"


CLEAN :
	-@erase "$(INTDIR)\eaxTerm.obj"
	-@erase "$(INTDIR)\eaxTerm.pch"
	-@erase "$(INTDIR)\eaxTerm.res"
	-@erase "$(INTDIR)\EaxTermClock.obj"
	-@erase "$(INTDIR)\ExecuteLink.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\eaxTerm.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\eaxTerm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x41f /fo"$(INTDIR)\eaxTerm.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\eaxTerm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\eaxTerm.pdb" /machine:I386 /out:"$(OUTDIR)\eaxTerm.exe" 
LINK32_OBJS= \
	"$(INTDIR)\eaxTerm.obj" \
	"$(INTDIR)\EaxTermClock.obj" \
	"$(INTDIR)\ExecuteLink.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\eaxTerm.res"

"$(OUTDIR)\eaxTerm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "eaxTerm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\eaxTerm.exe"


CLEAN :
	-@erase "$(INTDIR)\eaxTerm.obj"
	-@erase "$(INTDIR)\eaxTerm.pch"
	-@erase "$(INTDIR)\eaxTerm.res"
	-@erase "$(INTDIR)\EaxTermClock.obj"
	-@erase "$(INTDIR)\ExecuteLink.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\eaxTerm.exe"
	-@erase "$(OUTDIR)\eaxTerm.ilk"
	-@erase "$(OUTDIR)\eaxTerm.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\eaxTerm.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x41f /fo"$(INTDIR)\eaxTerm.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\eaxTerm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\eaxTerm.pdb" /debug /machine:I386 /out:"$(OUTDIR)\eaxTerm.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\eaxTerm.obj" \
	"$(INTDIR)\EaxTermClock.obj" \
	"$(INTDIR)\ExecuteLink.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\eaxTerm.res"

"$(OUTDIR)\eaxTerm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("eaxTerm.dep")
!INCLUDE "eaxTerm.dep"
!ELSE 
!MESSAGE Warning: cannot find "eaxTerm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "eaxTerm - Win32 Release" || "$(CFG)" == "eaxTerm - Win32 Debug"
SOURCE=.\eaxTerm.cpp

"$(INTDIR)\eaxTerm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\eaxTerm.pch"


SOURCE=.\eaxTerm.rc

"$(INTDIR)\eaxTerm.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\EaxTermClock.cpp

"$(INTDIR)\EaxTermClock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\eaxTerm.pch"


SOURCE=.\ExecuteLink.cpp

"$(INTDIR)\ExecuteLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\eaxTerm.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\eaxTerm.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "eaxTerm - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\eaxTerm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\eaxTerm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "eaxTerm - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\eaxTerm.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\eaxTerm.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

