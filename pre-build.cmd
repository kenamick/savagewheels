@echo off

echo [post-build] Starting ...

echo Copying SDL.dll ...
del .\bin\libsdl\lib\SDL.dll

echo Copying fmod.dll ...
del .\bin\fmod\win32\api\fmod.dll

echo [post-build] Done.

@echo on