# runsisi AT hust.edu.cn

all: GlobalHook.dll

GlobalHook.dll: GlobalHook.obj GlobalHook.res
	link /OUT:"$@" /DLL /DYNAMICBASE /DEF:"GlobalHook.def" /MACHINE:X86 \
	/NOLOGO /SUBSYSTEM:WINDOWS "user32.lib" $**

.cpp.obj:
	cl /Fo"$@" /c /O2 /MT /Gd /EHsc /nologo \
	/D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" $<

.rc.res:
	rc /nologo $<

clean:
	@del /f /q *.dll 2> nul
	@del /f /q *.obj 2> nul
	@del /f /q *.res 2> nul
	@del /f /q *.exp 2> nul
	@del /f /q *.lib 2> nul
	@echo OK!