DEBUG_BREAKPOINT:=1
DEBUG_RAS:=1

include config.mak

all: com2com.sys

COM2COM_FILES_ASM:=segments.obj
COM2COM_FILES_ASM2:=wclib/i4d.obj wclib/i4m.obj
COM2COM_FILES_C:=header.obj init.obj strategy_d.obj base/devhelp.obj base/stdio.obj base/stdlib.obj base/string.obj
COM2COM_FILES_CC:=resident.obj CircularBuffer.obj ComDevice.obj
COM2COM_FILES:=$(COM2COM_FILES_ASM) $(COM2COM_FILES_C) $(COM2COM_FILES_CC) $(COM2COM_FILES_ASM2)

com2com.sys: $(COM2COM_FILES)
	$(LDDRV_CMD)

$(COM2COM_FILES_ASM): %.obj: %.asm
	$(AS_CMD)

$(COM2COM_FILES_ASM2): %.obj: %.asm
	$(AS16_CMD)

$(COM2COM_FILES_C): %.obj: %.c
	$(CC16_CMD)

$(COM2COM_FILES_CC): %.obj: %.cc
	$(CCC16_CMD)

clean:
	-del /S *.obj
	-del com2com.sys
	-del com2com.map

full:
	$(MAKE) clean
	$(MAKE) all

logout.exe: logout.obj
	$(LD_CMD)

testout.exe: testout.obj
	$(LD_CMD)

testin.exe: testin.obj
	$(LD_CMD)
