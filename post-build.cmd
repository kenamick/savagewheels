@echo off

echo [post-build] Starting ...

echo Copying SDL.dll ...
copy .\libsdl\lib\SDL.dll bin\

echo Copying fmod.dll ...
copy .\fmod\win32\api\fmod.dll bin\

echo [post-build] Done.

@echo on