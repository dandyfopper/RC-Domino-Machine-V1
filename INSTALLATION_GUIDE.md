# Domino Machine Installation Guide

This guide provides step-by-step instructions for setting up your Domino Machine with the new Web Bluetooth controller. The updated system replaces the deprecated Blynk app with a browser-based controller that works on most modern devices.

## Table of Contents

1. [Hardware Setup](#hardware-setup)
2. [Firmware Installation](#firmware-installation)
3. [Controller Setup](#controller-setup)
4. [Testing](#testing)
5. [Troubleshooting](#troubleshooting)

## Hardware Setup

### Components Needed

- Domino Machine with Arduino Uno (see original build instructions)
- HC-05 or HC-06 Bluetooth module
- 4 jumper wires for connecting the Bluetooth module
- Optional: Logic level converter for most reliable operation

### Bluetooth Module Connection

Connect your HC-05/HC-06 Bluetooth module to the Arduino as follows:

| Bluetooth Module Pin | Arduino Uno Pin |
|---------------------|-----------------|
| VCC                 | 5V              |
| GND                 | GND             |
| TXD                 | Pin 12 (RX)     |
| RXD                 | Pin 13 (TX)     |

For detailed wiring instructions, see the [BLUETOOTH_WIRING.md](BLUETOOTH_WIRING.md) file.

## Firmware Installation

### Option A: Full Domino Machine Firmware

1. Open the Arduino IDE
2. Load the `DominoMachine_V4.ino` file
3. Select your Arduino Uno board from Tools > Board menu
4. Select the correct COM port from Tools > Port menu
5. Click the Upload button
6. Once uploaded, power cycle your Domino Machine

### Option B: Test Client (for testing without full hardware)

If you want to test the Bluetooth communication without the full Domino Machine hardware:

1. Open the Arduino IDE
2. Load the `DominoMachine_TestClient.ino` file
3. Connect your Arduino Uno with just the Bluetooth module and LCD (if available)
4. Upload the sketch
5. Open the Serial Monitor at 9600 baud to see debug information

## Controller Setup

### Desktop Controller

1. Open the `domino_controller.html` file in Google Chrome, Microsoft Edge, or another Web Bluetooth compatible browser
2. Click "Connect to Domino Machine"
3. Select your Bluetooth device from the popup list (usually named "HC-05" or "HC-06")
4. Once connected, the controller status indicator will turn green
5. Use the directional buttons, steering slider, and action buttons to control the machine

### Mobile Controller

For smartphones and tablets:

1. Transfer the `mobile_controller.html` file to your device or host it on a web server
2. Open the file in Google Chrome (Android) or another Web Bluetooth compatible browser
   - Note: iOS devices currently do not support Web Bluetooth API
3. Tap "Connect to Domino Machine" and select your device
4. Use the touchscreen controls to operate your Domino Machine

## Testing

### Component Tests

To verify your hardware components are working correctly:

1. Upload the `DominoMachine_Tests.ino` file to your Arduino
2. Open the Serial Monitor at 9600 baud
3. Follow the prompts to test each component (servos, motor, switches)
4. Verify each test passes before proceeding to normal operation

### Communication Test

To test just the Bluetooth communication:

1. Upload the `DominoMachine_TestClient.ino` to your Arduino
2. Connect to it using the web or mobile controller
3. Verify commands are received (visible in the Serial Monitor)
4. Verify status updates are displayed in the controller

## Troubleshooting

### Common Issues

1. **Controller can't find Bluetooth device**
   - Make sure Bluetooth is enabled on your device
   - Verify the Bluetooth module's power LED is on
   - Try power cycling the Arduino and Bluetooth module

2. **Connected but commands don't work**
   - Check your wiring, especially TX/RX connections
   - Verify the firmware was uploaded successfully
   - Make sure you're using a Web Bluetooth compatible browser

3. **Status updates not appearing**
   - Check that the format of status messages matches what the controller expects
   - Verify the Bluetooth connection is still active

4. **LCD not displaying information**
   - Check LCD wiring
   - If using the test client, make sure `hasLCD` is set to `true`

### Getting Help

If you encounter issues, check the following resources:

- Review the original project documentation at https://www.diymachines.co.uk/rc-domino-laying-machine
- Visit the GitHub repository for updates and issue reporting
- Join the DIY Machines community forum for community support

## Advanced Usage

### Hosting the Controller Online

You can host the controller files on a web server for easier access:

1. Upload both HTML controller files to your web server
2. Access them via HTTPS (required for Web Bluetooth)
3. Share the URL with others who have built the Domino Machine

### Customizing the Controller

The controllers are built with standard HTML, CSS, and JavaScript:

1. Open the controller files in any text editor
2. Modify the layout, colors, or functionality as desired
3. Test your changes with the Domino Machine or test client