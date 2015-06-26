@setlocal enableextensions enabledelayedexpansion

set outdir=%1
set configuration=%2
set platform=%3

rem if /I "%configuration:debug=%" == "%configuration%" exit /b

if /I "%platform%" == "x64" (
	set libdir=%~dp0bin\Win64
	set vldfile=vld_x64.dll
) else (
	set libdir=%~dp0bin\Win32
	set vldfile=vld_x86.dll
)

call :copyfile %vldfile%, "%libdir%", "%outdir%"
call :copyfile dbghelp.dll, "%libdir%", "%outdir%"
call :copyfile Microsoft.DTfW.DHL.manifest, "%libdir%", "%outdir%"

exit /b
@endlocal

rem ------
:copyfile
setlocal
set file=%~1
set srcdir=%~2
set destdir=%~3

echo Copying %file% to %destdir%
xcopy /Y /V /D "%srcdir%\%file%" "%destdir%"

endlocal
goto :eof

