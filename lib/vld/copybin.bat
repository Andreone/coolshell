set OutDir=%1
set BuildConfig=%2
set BuildPlatform=%3

if "%BuildPlatform%" == "Win32" (
    set BinDir=%~dp0bin\Win32
) else (
    set BinDir=%~dp0bin\Win64
)

xcopy %BinDir% %OutDir% /Y/D/V
