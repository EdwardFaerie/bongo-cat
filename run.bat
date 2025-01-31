@echo off
setlocal enabledelayedexpansion

set "default_bin_folder=D:\libraries\w64devkit\bin"
:: Change above to the right path ONLY IF you are lazy like me xd 

:Bin
set "bin_folder="
echo The default bin folder is: %default_bin_folder%
echo Press Enter to accept the default, or type a new path and press Enter to change it.
echo Enter bin folder path (or press Enter to accept default):

set /p bin_folder=

if "%bin_folder%"=="" (
    set "bin_folder=%default_bin_folder%"
)

echo checking bin folder: %bin_folder%
if not exist "%bin_folder%" (
    echo Bin Folder Does not exist
    goto Bin
)

echo Using bin folder: %bin_folder%

set "make_path=%bin_folder%\make.exe"
if not exist "%make_path%" (
    echo make.exe not found in the bin folder: %bin_folder%
    goto Bin
)

echo make.exe found in: %bin_folder%

:OSS
set choiceOS=
echo Please enter a number for OS Build:
echo [1] Windows
echo [2] Linux

set /p choiceOS=
echo You entered: %choiceOS%

if "%choiceOS%"=="1" (
    echo You selected Windows.
    goto BUILD1
)
if "%choiceOS%"=="2" (
    echo You selected Linux.
    goto BUILD2
)
if not "%choiceOS%"=="1" if not "%choiceOS%"=="2" (
    echo Invalid OS selection. Please enter either 1 (Windows) or 2 (Linux).
    goto OSS
)

:BUILD1
echo Please enter a number:
echo [1] Make build
echo [2] Run Build already Built (Need to build first!)
set /p choiceBUILD=""
echo You entered: %choiceBUILD%
if "%choiceBUILD%"=="1" (
    if exist "%~dp0\bin" (
        echo Removed bin folder
        RMDIR /S /Q "%~dp0\bin"
    )

    if exist "%~dp0\obj" (
        echo Removed obj folder
        RMDIR /S /Q "%~dp0\obj"
    )

    echo Running make build for Windows...
    "%bin_folder%\make.exe" -f "%~dp0Makefile.windows"
    pause
    echo Removed obj folder
    RMDIR /S /Q "%~dp0\obj"
    pause
    exit
)
if "%choiceBUILD%"=="2" (
    echo Running already built Windows project...
    "%bin_folder%\make.exe" -f "%~dp0Makefile.windows" test
    pause
)
if not "%choiceBUILD%"=="1" if not "%choiceBUILD%"=="2" (
    echo Invalid choice. Please enter a correct number.
    pause
    goto BUILD1
)

:BUILD2
echo Please enter a number:
echo [1] Make build
echo [2] Run Build already Built (Need to build first!)
set /p choiceBUILD2=""
echo You entered: %choiceBUILD2%
if "%choiceBUILD2%"=="1" (
    if exist "%~dp0\bin" (
        echo Removed bin folder
        RMDIR /S /Q "%~dp0\bin"
    )

    if exist "%~dp0\obj" (
        echo Removed obj folder
        RMDIR /S /Q "%~dp0\obj"
    )

    echo Running make build for Linux...
    "%bin_folder%\make.exe" -f "%~dp0Makefile.linux"
    pause
    echo Removed obj folder
    RMDIR /S /Q "%~dp0\obj"
    pause
    exit
)
if "%choiceBUILD2%"=="2" (
    echo Running already built Linux project...
    "%bin_folder%\make.exe" -f "%~dp0Makefile.linux" test
    pause
)
if not "%choiceBUILD2%"=="1" if not "%choiceBUILD2%"=="2" (
    echo Invalid choice. Please enter a correct number.
    pause
    goto BUILD2
)