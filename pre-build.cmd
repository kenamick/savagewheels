@echo off

SET TARGET=bin

echo [post-build] Starting ...

del /Q %TARGET%\debug.html
del /Q %TARGET%\pref

del /Q %TARGET%\SDL.dll
del /Q %TARGET%\README-SDL.txt 

del /Q %TARGET%\fmod.dll

del /Q %TARGET%\HISTORY
del /Q %TARGET%\LICENSE

del /Q %TARGET%\sw*.exe
del /Q %TARGET%\sw*.map

echo [post-build] Done.

@echo on