@echo off
setlocal
echo ===================================================
echo   Compiling VRChat DOOM over OSC (MSVC C++17)
echo ===================================================

set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VS_PATH%" (
    set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
)

if not exist "%VS_PATH%" (
    echo [ERROR] Visual Studio 2022 compiler environment not found!
    pause
    exit /b 1
)

call "%VS_PATH%" >nul 2>&1

if not exist "bin" mkdir "bin"

cl.exe /nologo /O2 /std:c++17 /EHsc /W3 /I src src\main.cpp /Fe"bin\vrc_doom_osc.exe" /link ws2_32.lib user32.lib

if %ERRORLEVEL% equ 0 (
    echo.
    echo ===================================================
    echo  [SUCCESS] Build complete: bin\vrc_doom_osc.exe
    echo ===================================================
    del *.obj >nul 2>&1
) else (
    echo.
    echo [ERROR] Compilation failed with error code %ERRORLEVEL%
)

endlocal
