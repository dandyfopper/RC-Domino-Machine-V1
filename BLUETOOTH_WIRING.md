# Bluetooth Module Wiring Guide

This guide explains how to connect an HC-05 or HC-06 Bluetooth module to your Domino Machine for use with the new web controller.

## Wiring Connections

Connect your HC-05/HC-06 Bluetooth module to the Arduino as follows:

| Bluetooth Module Pin | Arduino Uno Pin |
|---------------------|-----------------|
| VCC                 | 5V              |
| GND                 | GND             |
| TXD                 | Pin 12 (RX)     |
| RXD                 | Pin 13 (TX)     |

## Important Notes:

1. **Logic Level Shifting**: The HC-05/HC-06 operates at 3.3V logic levels, while the Arduino operates at 5V. For the most reliable operation, use a logic level shifter between the Arduino and the Bluetooth module's RX pin. However, many users have found direct connection works adequately.

2. **Serial Communication**: When the Bluetooth module is connected, you cannot use the USB serial port for debugging while the machine is operating. If you need to debug, temporarily disconnect the Bluetooth module's TX and RX pins.

3. **Pairing Code**: The default pairing code for most HC-05 modules is "1234" and for HC-06 modules is "0000". You may need this when connecting from your device.

4. **Configuration**: If your Bluetooth module isn't working correctly, you may need to configure its baud rate to 9600 using AT commands. This is the default for most modules but some might be set differently.

## Testing the Connection

1. Upload the `DominoMachine_V4.ino` firmware to your Arduino.
2. Connect the Bluetooth module as described above.
3. Power on your Domino Machine.
4. On startup, your machine's LCD should show "Bluetooth Mode" when this option is selected.
5. Open the `domino_controller.html` in a Chrome browser.
6. Click "Connect to Domino Machine" and select your Bluetooth device from the list.

## Troubleshooting

If you experience connection issues:

1. **Module not visible**: Make sure your Bluetooth module's power LED is on, indicating it's receiving power.
2. **Can't connect**: Try resetting the Bluetooth module by cycling power to your machine.
3. **Commands not working**: Double-check the wiring, especially TX/RX connections. Remember that TX from the module goes to RX on the Arduino and vice versa.
4. **Pairing issues**: Try pairing the Bluetooth module with your device before opening the web controller.
5. **Browser compatibility**: Make sure you're using a browser that supports Web Bluetooth API (Chrome, Edge, Opera).

For additional support, please visit the DIY Machines forum or GitHub issues page.