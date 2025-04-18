@echo off
REM Simple script to start a local HTTP server for the domino machine controller
REM This allows you to access the controllers from any device on your local network

echo Starting local web server for Domino Machine Controller...
echo Note: Web Bluetooth API requires HTTPS for production use,
echo       but works with HTTP on localhost for development.
echo.

REM Find Python installation and start server
SET PORT=8000

REM Get IP address
for /f "tokens=1-2 delims=:" %%a in ('ipconfig ^| findstr "IPv4"') do (
    set IP=%%b
)
set IP=%IP:~1%

echo Starting server at http://%IP%:%PORT%
echo Access the controllers from any device on your network using this URL.
echo For mobile devices, use: http://%IP%:%PORT%/mobile_controller.html
echo For desktop, use: http://%IP%:%PORT%/domino_controller.html
echo.
echo Press Ctrl+C to stop the server when finished.
echo.

where python >nul 2>nul
if %errorlevel% equ 0 (
    python -m http.server %PORT%
) else (
    where py >nul 2>nul
    if %errorlevel% equ 0 (
        py -m http.server %PORT%
    ) else (
        echo Error: Python not found. Please install Python to use this script.
        echo Visit https://www.python.org/downloads/ to download Python.
        pause
        exit /b 1
    )
)