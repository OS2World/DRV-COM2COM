export WATCOM:=e:\dev\wc11
COMP_INCLUDE=$(WATCOM)\h

C16_INCLUDE:=E:\DEV\ddk\ddk\base\h

EMPTY:=
SPACE:=$(EMPTY) $(EMPTY)
COMMA:=, 

CC16:=cmd /c wcc16.cmd
CC16OPT:=-bt=os2 -bd -bm -ms -6 -omi -s -zdp -zff -zgf -zu -zl -zp=1
ifdef DEBUG
CC16OPT+= -d3 -hc
endif
ifdef DEBUG_BREAKPOINT
CC16OPT+= -dDEBUG_BREAKPOINT
endif
ifdef DEBUG_RAS
CC16OPT+= -dDEBUG_RAS
endif
CC16_FILE_SRC=$^
CC16_FILE_OUT=$@
CC16_INCLUDE=$(C16_INCLUDE)
CC16_CMD=$(CC16) $(CC16OPT)
ifneq ($(COMP_INCLUDE),$(EMPTY))
CC16_CMD+= -i=$(subst /,\,$(COMP_INCLUDE))
endif
ifneq ($(CC16_INCLUDE),$(EMPTY))
CC16_CMD+= -i=$(subst /,\,$(CC16_INCLUDE))
endif
CC16_CMD+= -fo=$(subst /,\,$(CC16_FILE_OUT)) $(subst /,\,$(CC16_FILE_SRC))

CCC16:=cmd /c wpp16.cmd
CCC16OPT:=-bt=os2 -ms -6 -omi -s -zdp -zff -zgf -zu -zl -zp=1
ifdef DEBUG
CCC16OPT+= -d3 -hc
endif
ifdef DEBUG_BREAKPOINT
CCC16OPT+= -dDEBUG_BREAKPOINT
endif
ifdef DEBUG_RAS
CCC16OPT+= -dDEBUG_RAS
endif
CCC16_FILE_SRC=$^
CCC16_FILE_OUT=$@
CCC16_INCLUDE=$(C16_INCLUDE)
CCC16_CMD=$(CCC16) $(CCC16OPT)
ifneq ($(COMP_INCLUDE),$(EMPTY))
CCC16_CMD+= -i=$(subst /,\,$(COMP_INCLUDE))
endif
ifneq ($(CCC16_INCLUDE),$(EMPTY))
CCC16_CMD+= -i=$(subst /,\,$(CCC16_INCLUDE))
endif
CCC16_CMD+= -fo=$(subst /,\,$(CCC16_FILE_OUT)) $(subst /,\,$(CCC16_FILE_SRC))

CC:=cmd /c wcc.cmd
CCOPT:=-bt=os2 -6 -omi -zp=1
ifdef DEBUG
CCOPT+= -d3 -hc
endif
ifdef DEBUG_BREAKPOINT
CCOPT+= -dDEBUG_BREAKPOINT
endif
ifdef DEBUG_RAS
CCOPT+= -dDEBUG_RAS
endif
CC_FILE_SRC=$^
CC_FILE_OUT=$@
CC_INCLUDE=$(C_INCLUDE)
CC_CMD=$(CC) $(CCOPT)
ifneq ($(COMP_INCLUDE),$(EMPTY))
CC_CMD+= -i=$(subst /,\,$(COMP_INCLUDE))
endif
ifneq ($(CC_INCLUDE),$(EMPTY))
CC_CMD+= -i=$(subst /,\,$(CC_INCLUDE))
endif
CC_CMD+= -fo=$(subst /,\,$(CC_FILE_OUT)) $(subst /,\,$(CC_FILE_SRC))

CCC=cmd /c wpp.cmd
CCCOPT:=-bt=os2 -6 -omi -zp=1
ifdef DEBUG
CCCOPT+= -d3 -hc
endif
ifdef DEBUG_BREAKPOINT
CCCOPT+= -dDEBUG_BREAKPOINT
endif
ifdef DEBUG_RAS
CCCOPT+= -dDEBUG_RAS
endif
CCC_FILE_SRC=$^
CCC_FILE_OUT=$@
CCC_INCLUDE=$(C_INCLUDE)
CCC_CMD=$(CCC) $(CCCOPT)
ifneq ($(COMP_INCLUDE),$(EMPTY))
CCC_CMD+= -i=$(subst /,\,$(COMP_INCLUDE))
endif
ifneq ($(CCC_INCLUDE),$(EMPTY))
CCC_CMD+= -i=$(subst /,\,$(CCC_INCLUDE))
endif
CCC_CMD+= -fo=$(subst /,\,$(CCC_FILE_OUT)) $(subst /,\,$(CCC_FILE_SRC))

LDDRV:=cmd /c wlink.cmd
LDDRVOPT:=sys os2 dll initglobal option map
ifdef DEBUG
LDDRVOPT+= debug codeview all option cvpack
endif
# LDDRVOPT+= lib clibs
LDDRV_FILE_SRC=$^
LDDRV_FILE_OUT=$@
LDDRV_CMD=$(LDDRV) $(LDDRVOPT) NAME $(LDDRV_FILE_OUT) FILE $(subst $(SPACE),$(COMMA),$(LDDRV_FILE_SRC))
#LDDRV_CMD+=LIBRARY E:\DEV\ddk\ddk\base\lib\os2286.lib LIBRARY $(WATCOM)\lib286\plibs.lib

LD:=cmd /c wlink.cmd
LDOPT:=SYS os2v2 DEBUG DWARF
LD_FILE_SRC=$^
LD_FILE_OUT=$@
LD_CMD=$(LD) $(LDOPT) NAME $(LD_FILE_OUT) FILE $(subst $(SPACE),$(COMMA),$(LD_FILE_SRC))

AS16:=$(WATCOM)\BINP\wasm.exe
AS16OPT:=-ms
ifdef DEBUG
AS16OPT+= -d1
endif
AS16_FILE_SRC=$^
AS16_FILE_OUT=$@
AS16_CMD=$(AS16) $(AS16OPT) -fo=$(subst /,\,$(AS16_FILE_OUT)) $(subst /,\,$(AS16_FILE_SRC))

AS:=$(WATCOM)\BINP\wasm.exe
ASOPT:=$(EMPTY)
ifdef DEBUG
ASOPT+= -d1
endif
AS_FILE_SRC=$^
AS_FILE_OUT=$@
AS_CMD=$(AS) $(ASOPT) -fo=$(subst /,\,$(AS_FILE_OUT)) $(subst /,\,$(AS_FILE_SRC))

%.obj: %.c
	$(CC_CMD)

%.obj: %.cc
	$(CCC_CMD)

%.obj: %.asm
	$(AS_CMD)
