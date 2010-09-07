@echo off

SET TARGET=bin

echo [post-build] Starting ...

del %TARGET%\debug.html

del %TARGET%\SDL.dll
del %TARGET%\README-SDL.txt 

del %TARGET%\fmod.dll

echo [post-build] Done.

@echo on