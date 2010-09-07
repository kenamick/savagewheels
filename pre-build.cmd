@echo off

SET TARGET=bin

echo [post-build] Starting ...

echo Copying SDL.dll ...
del %TARGET%\SDL.dll
del %TARGET%\README-SDL.txt 

echo Copying fmod.dll ...
del %TARGET%\fmod.dll

echo [post-build] Done.

@echo on