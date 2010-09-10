@echo off

SET TARGET=bin\

echo [post-build] Starting ...

echo Copying SDL.dll ...
copy /Y libsdl\lib\SDL.dll %TARGET%
copy /Y libsdl\README-SDL.txt %TARGET%
copy /Y HISTORY %TARGET%

echo Copying fmod.dll ...
copy /Y fmod\win32\api\fmod.dll %TARGET%

echo Copying executable(s) ...
copy /Y %1\*.exe %TARGET%
copy /Y %1\*.map %TARGET%

echo [post-build] Done.

@echo on