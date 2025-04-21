#!/bin/bash
# Simple script to start a local HTTP server for the domino machine controller
# This allows you to access the controllers from any device on your local network

echo "Starting local web server for Domino Machine Controller..."
echo "Note: Web Bluetooth API requires HTTPS for production use,"
echo "      but works with HTTP on localhost for development."
echo ""

# Determine the local IP address
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    LOCAL_IP=$(ipconfig getifaddr en0 2>/dev/null || ipconfig getifaddr en1)
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    LOCAL_IP=$(hostname -I | awk '{print $1}')
else
    # Windows or other
    LOCAL_IP="localhost"
fi

PORT=8000

echo "Starting server at http://$LOCAL_IP:$PORT"
echo "Access the controllers from any device on your network using this URL."
echo "For mobile devices, use: http://$LOCAL_IP:$PORT/mobile_controller.html"
echo "For desktop, use: http://$LOCAL_IP:$PORT/domino_controller.html"
echo ""
echo "Press Ctrl+C to stop the server when finished."
echo ""

# Python version detection and appropriate server start
if command -v python3 &>/dev/null; then
    # Python 3
    python3 -m http.server $PORT
elif command -v python &>/dev/null; then
    # Python 2
    python -m SimpleHTTPServer $PORT
else
    echo "Error: Python not found. Please install Python to use this script."
    echo "Visit https://www.python.org/downloads/ to download Python."
    exit 1
fi