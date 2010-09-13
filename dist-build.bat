@echo off

SET CMD="%1"
SET CUR_DIR=%CD%
SET BIN_PATH=%CUR_DIR%\bin
SET TMP_PATH=%CUR_DIR%\dist-build.tmp

echo Building distributable package folder ...

:clean

echo Cleaning up ...
del /Q /S %TMP_PATH%\*.*
rmdir /Q /S %TMP_PATH%\

:build
echo Building ...

echo Created %TMP_PATH% ...
mkdir %TMP_PATH%

echo Copying files ...
xcopy /Y /C /S /EXCLUDE:dist-build.bat.excl %BIN_PATH%\*.* %TMP_PATH%\ 

:end

echo Done.

@echo on