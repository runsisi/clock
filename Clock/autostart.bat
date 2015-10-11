:: runsisi AT hust.edu.cn

@echo off

set keyName=restclock
set programName=Clock.exe
set disabled=1

REG QUERY HKCU\Software\Microsoft\Windows\CurrentVersion\Run /v restclock > nul 2>&1

if %errorlevel% == 0 (
    echo 已设置自启动，取消自启动?
    set disabled=0
) else (
    echo 未设置自启动，设置自启动?
)

echo=
CHOICE /C YC /M "确认请按 Y，取消请按 C。"

if %errorlevel% == 1 (
    if %disabled% == 0 (
        REG DELETE HKCU\Software\Microsoft\Windows\CurrentVersion\Run /f /v %keyName%
    ) else (
        REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Run /f /v %keyName% /t REG_SZ /d "%cd%\%programName%"
    )
)

pause