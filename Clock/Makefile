# runsisi AT hust.edu.cn

all: Clock.exe

Clock.exe: Xml.lib ClockWnd.obj Main.obj Clock.res
	link  /OUT:"$@" /DYNAMICBASE /MACHINE:X86 /SUBSYSTEM:WINDOWS /NOLOGO \
	"kernel32.lib" "user32.lib" "gdi32.lib" $**

Xml.lib: CXml\Xml.obj CXml\XmlNode.obj CXml\XmlNodes.obj CXml\Xsl.obj 
	lib /OUT:"$@" /NOLOGO $**

.cpp.obj:
	cl /Fo"$@" /c /O2 /MT /Gd /EHsc /nologo /D "GDIPLUS_NO_AUTO_INIT" \
	/D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" \
	/I"Include" /I"." $<

.rc.res:
	rc /nologo $<

clean:
	@del /f /q *.exe 2> nul
	@del /f /q *.lib 2> nul
	@del /f /q *.obj 2> nul
	@del /f /q *.res 2> nul
	@del /f /q *.tlh 2> nul
	@del /f /q *.tli 2> nul
	@del /f /q CXml\*.obj 2> nul
	@del /f /q CXml\*.tlh 2> nul
	@del /f /q CXml\*.tli 2> nul
	@echo OK!