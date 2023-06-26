@echo off
setlocal enabledelayedexpansion

set CLEAN_BUILD=NO
set BUILD_TEST=false
set EXAMPLE_NAME=

REM Parse command line args as a single string
for /f "delims=" %%a in ("%*") do (
  set "params=%%a"
)
echo !params!

REM Split string by spaces manually
for %%a in (!params!) do (
  set "para=%%a"
  if "%%a"=="--clean" (
    set CLEAN_BUILD=YES
  ) else if "%%a"=="--test" (
    set BUILD_TEST=true
  ) else if "!para:~0,9!"=="--example" (
    REM Assuming that the next parameter is the example name
    set nextIsExampleName=true
  ) else (
    if defined nextIsExampleName (
      set EXAMPLE_NAME=%%a
      set nextIsExampleName=
    ) else (
      echo Unknown option %%a
      exit /b 1
    )
  )
)

if "%CLEAN_BUILD%"=="YES" (
  rmdir /S /Q build
  echo Removed build directory
  exit /b 0
)

if not exist build mkdir build
cd build
cmake .. -DBUILD_TEST=%BUILD_TEST% -DEXAMPLE_NAME=%EXAMPLE_NAME% -DPLATFORM_NAME="windows"
cmake --build . --config Release -j 7

REM Define the source and target directories
set TARGET_DIR=./Release

REM Check if *.dll exists in the target directory
if not exist "%TARGET_DIR%/taichi_c_api.dll" (
  copy "..\dependencies\taichi_c_api\bin\Windows\x86_64_vc22\taichi_c_api.dll" "%TARGET_DIR%"
)
if not exist "%TARGET_DIR%/soft2d.dll" (
  copy "..\soft2d\bin\Windows\x86_64_vc22\soft2d.dll" "%TARGET_DIR%"
)

if "%errorlevel%"=="0" (
  if "%BUILD_TEST%"=="true" (
    echo Running tests
    .\Release\tests
  ) else (
    if "%EXAMPLE_NAME%"=="" (
      set EXAMPLE_NAME="body_minimal"
    )
    echo .\Release\!EXAMPLE_NAME!
    .\Release\!EXAMPLE_NAME!
  )
) else (
  echo Building failed!
  exit /b 1
)