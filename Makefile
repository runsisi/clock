All: GlobalHook.dll

GlobalHook.dll: GlobalHook.obj GlobalHook.res
	link /DLL /DYNAMICBASE /DEF:"GlobalHook.def" /MACHINE:X86 "user32.lib" $**

.cpp.obj:
	cl /c /O2 /MD /Gd /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" $<

.rc.res:
	rc $<

clean:
	del /f /q *.exe
	del /f /q *.dll
	del /f /q *.obj
	del /f /q *.res
	del /f /q *.exp
	del /f /q *.lib