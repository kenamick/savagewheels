@echo off

SET CMD="%1"
SET CUR_DIR=%CHDIR%
SET BIN_PATH=%CUR_DIR%\bin
SET TMP_PATH=%CUR_DIR%\dist-build.tmp


:clean

del /Q %TMP_PATH%\*.*

:build

mkdir %TMP_PATH%
xcopy /L /Y /C %BIN_PATH%\*.* %TMP_PATH%

@echo on