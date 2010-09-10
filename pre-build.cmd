@echo off

SET TARGET=bin

echo [post-build] Starting ...

del /Q %TARGET%\debug.html

del /Q %TARGET%\SDL.dll
del /Q %TARGET%\README-SDL.txt 

del /Q %TARGET%\fmod.dll

del /Q %1
del /Q %TARGET%\s*.exe

echo [post-build] Done.

@echo on