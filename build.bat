@echo off
setlocal EnableDelayedExpansion

echo ========================================
echo        SIMPLE THINK BUILD
echo ========================================
echo.

REM ========================================
REM Create build directories
REM ========================================

if not exist build mkdir build
if not exist build\obj mkdir build\obj

echo Checking for C compiler...
echo.

REM ========================================
REM Detect compiler
REM ========================================

set "COMPILER="

where gcc >nul 2>nul
if not errorlevel 1 (
    set "COMPILER=GCC"
    goto COMPILER_FOUND
)

where clang >nul 2>nul
if not errorlevel 1 (
    set "COMPILER=CLANG"
    goto COMPILER_FOUND
)

where cl >nul 2>nul
if not errorlevel 1 (
    set "COMPILER=MSVC"
    goto COMPILER_FOUND
)

echo ERROR: No supported C compiler was found.
echo.
echo Supported compilers:
echo   GCC
echo   Clang
echo   MSVC
echo.
goto BUILD_FAILED


:COMPILER_FOUND

echo Compiler: %COMPILER%
echo.

REM ========================================
REM Remove old build files
REM ========================================

echo Cleaning old object files...

del /Q "build\obj\*.o" >nul 2>nul
del /Q "build\obj\*.obj" >nul 2>nul
del /Q "build\SimpleThink.lib" >nul 2>nul
del /Q "build\stc.exe" >nul 2>nul

echo.
echo Compiling ALL C source files...
echo.

REM ========================================
REM GCC
REM ========================================

if "%COMPILER%"=="GCC" goto BUILD_GCC

REM ========================================
REM Clang
REM ========================================

if "%COMPILER%"=="CLANG" goto BUILD_CLANG

REM ========================================
REM MSVC
REM ========================================

if "%COMPILER%"=="MSVC" goto BUILD_MSVC

goto BUILD_FAILED


:BUILD_GCC

set "SOURCE_COUNT=0"

for %%F in (*.c) do (
    set /a SOURCE_COUNT+=1

    echo   [!SOURCE_COUNT!] %%F

    gcc ^
        -std=c11 ^
        -Wall ^
        -Wextra ^
        -O2 ^
        -I. ^
        -c "%%F" ^
        -o "build\obj\%%~nF.o"

    if errorlevel 1 (
        echo.
        echo ========================================
        echo BUILD FAILED
        echo ========================================
        echo.
        echo Error while compiling:
        echo   %%F
        echo.
        goto BUILD_FAILED
    )
)

goto GCC_LIBRARY


:GCC_LIBRARY

echo.
echo Compiled !SOURCE_COUNT! C source files.
echo.

echo Creating SimpleThink library...

del /Q "build\SimpleThink.lib" >nul 2>nul

set "FIRST_OBJECT=1"

for %%F in (build\obj\*.o) do (

    REM main.o belongs to the executable, not the library.
    if /I not "%%~nF"=="main" (

        if "!FIRST_OBJECT!"=="1" (
            ar rcs "build\SimpleThink.lib" "%%F"
            set "FIRST_OBJECT=0"
        ) else (
            ar r "build\SimpleThink.lib" "%%F"
        )

        if errorlevel 1 (
            echo.
            echo ERROR: Failed to add:
            echo   %%F
            echo.
            goto BUILD_FAILED
        )
    )
)

echo.
echo Linking stc.exe...

gcc ^
    -o "build\stc.exe" ^
    "build\obj\main.o" ^
    "build\SimpleThink.lib" ^
    -lws2_32 ^
    -lwinmm

if errorlevel 1 (
    echo.
    echo ERROR: Failed to link stc.exe.
    echo.
    goto BUILD_FAILED
)

goto BUILD_SUCCESS


:BUILD_CLANG

set "SOURCE_COUNT=0"

for %%F in (*.c) do (
    set /a SOURCE_COUNT+=1

    echo   [!SOURCE_COUNT!] %%F

    clang ^
        -std=c11 ^
        -Wall ^
        -Wextra ^
        -O2 ^
        -I. ^
        -c "%%F" ^
        -o "build\obj\%%~nF.o"

    if errorlevel 1 (
        echo.
        echo ========================================
        echo BUILD FAILED
        echo ========================================
        echo.
        echo Error while compiling:
        echo   %%F
        echo.
        goto BUILD_FAILED
    )
)

goto CLANG_LIBRARY


:CLANG_LIBRARY

echo.
echo Compiled !SOURCE_COUNT! C source files.
echo.

echo Creating SimpleThink library...

del /Q "build\SimpleThink.lib" >nul 2>nul

set "FIRST_OBJECT=1"

for %%F in (build\obj\*.o) do (

    REM main.o belongs to the executable, not the library.
    if /I not "%%~nF"=="main" (

        if "!FIRST_OBJECT!"=="1" (
            llvm-ar rcs "build\SimpleThink.lib" "%%F"
            set "FIRST_OBJECT=0"
        ) else (
            llvm-ar r "build\SimpleThink.lib" "%%F"
        )

        if errorlevel 1 (
            echo.
            echo ERROR: Failed to add:
            echo   %%F
            echo.
            goto BUILD_FAILED
        )
    )
)

echo.
echo Linking stc.exe...

clang ^
    -o "build\stc.exe" ^
    "build\obj\main.o" ^
    "build\SimpleThink.lib" ^
    -lws2_32 ^
    -lwinmm

if errorlevel 1 (
    echo.
    echo ERROR: Failed to link stc.exe.
    echo.
    goto BUILD_FAILED
)

goto BUILD_SUCCESS


:BUILD_MSVC

set "SOURCE_COUNT=0"

for %%F in (*.c) do (
    set /a SOURCE_COUNT+=1

    echo   [!SOURCE_COUNT!] %%F

    cl ^
        /nologo ^
        /std:c11 ^
        /O2 ^
        /I. ^
        /c "%%F" ^
        /Fo"build\obj\%%~nF.obj"

    if errorlevel 1 (
        echo.
        echo ========================================
        echo BUILD FAILED
        echo ========================================
        echo.
        echo Error while compiling:
        echo   %%F
        echo.
        goto BUILD_FAILED
    )
)

echo.
echo Compiled !SOURCE_COUNT! C source files.
echo.

echo Creating SimpleThink library...

del /Q "build\SimpleThink.lib" >nul 2>nul

REM main.obj belongs to the executable, not the library.
set "LIB_OBJECTS="

for %%F in (build\obj\*.obj) do (
    if /I not "%%~nF"=="main" (
        set "LIB_OBJECTS=!LIB_OBJECTS! "%%F""
    )
)

lib ^
    /nologo ^
    /OUT:"build\SimpleThink.lib" ^
    !LIB_OBJECTS!

if errorlevel 1 (
    echo.
    echo ERROR: Failed to create library.
    echo.
    goto BUILD_FAILED
)

echo.
echo Linking stc.exe...

link ^
    /OUT:"build\stc.exe" ^
    "build\obj\main.obj" ^
    "build\SimpleThink.lib" ^
    ws2_32.lib ^
    winmm.lib

if errorlevel 1 (
    echo.
    echo ERROR: Failed to link stc.exe.
    echo.
    goto BUILD_FAILED
)

goto BUILD_SUCCESS


:BUILD_SUCCESS

echo.
echo ========================================
echo          BUILD SUCCESSFUL
echo ========================================
echo.
echo C source files compiled:
echo   !SOURCE_COUNT!
echo.
echo Library created:
echo   build\SimpleThink.lib
echo.
echo Compiler executable:
echo   build\stc.exe
echo.
echo Object files:
echo   build\obj\
echo.
echo ========================================
echo.

goto END


:BUILD_FAILED

echo.
echo ========================================
echo            BUILD FAILED
echo ========================================
echo.

:END

pause
endlocal