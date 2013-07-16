set OutDir=%1
set Configuration=%2
set Platform=%3

if "%Platform%" == "Win32" (
    set BinDir=%~dp0bin\Win32
) else (
    set BinDir=%~dp0bin\Win64
)

xcopy %BinDir% %OutDir% /Y/D/V
