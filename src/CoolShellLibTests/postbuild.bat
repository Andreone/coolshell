set OutDir=%1
set BuildConfig=%2
set BuildPlatform=%3
set BuildToolsetVersion=%4

if "%BuildConfig%" == "Debug" (
	echo Copying Visual Leak Detector files...
	call ..\..\lib\vld\copybin.bat %* > NUL
)

echo Running unit tests...
%OutDir%\CoolShellLibTests.exe