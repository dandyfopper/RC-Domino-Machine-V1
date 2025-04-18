# Arduino Remote Controlled Domino Laying Machine - With Web Bluetooth Control
3D Printed

How to 3D print and assemble your very own programmable and remote controllable automatic domino laying machine.

Who likes knocking down absurdly long runs of dominoes? I do. Who doesn't like laying them all by hand. I don't. That's why I made this. 😊 

I made a detailed project video here: https://youtu.be/QsAplPLtriw 

Using some basic electronics this machine can line up well spaced rows of wooden dominoes. You can drive it around corners, forwards and backwards - just don't drive into your already laid dominoes. You can enter a list of instructions to your robot via the Arduino IDE, upload with USB and then send it on its way to follow your instructions, or you can control it live via Bluetooth using the web controller.

## Updates (April 2025)

This repository has been updated with a new version of the firmware and controller application:

- **DominoMachine_V4.ino**: Updated firmware that uses standard Bluetooth communication instead of the deprecated Blynk app
- **domino_controller.html**: A web-based controller that works with any modern browser that 
### How to Use the New Web Controller

1. Upload the `DominoMachine_V4.ino` firmware to your Arduino Uno
2. Connect an HC-05 or HC-06 Bluetooth module to the Arduino (pins 12 & 13)
3. Power on the Domino Machine
4. Open `domino_controller.html` in a Chrome or Edge browser
5. Click "Connect to Domino Machine" and select your Bluetooth device from the popup
6. Control your Domino Machine using the intuitive interface:
   - Use the directional pad to move the machine
   - Adjust the steering angle with the slider
   - Rotate carousel or toggle firebreak mode with the action buttons
   - Monitor domino count and machine status in real-time

==========

## List of items used in this project and where to find them:

■ Elegoo Arduino Uno (x1) - https://geni.us/ArduinoUno

■ Micro continuous rotation servo (x1) - https://geni.us/360MicroServo

■ Micro 180º servo (x1) - https://geni.us/MicroServo

■ LM317 DC-DC Converter (x1) - https://geni.us/LM317

■ Six AA Battery Holder (x1) - https://geni.us/6AABatteryHolder

■ AA Batteries (x6) - https://geni.us/AABatteries

■ 1:90 TT DC Motor (x1) - https://geni.us/1-90GearedTTMotor 

■  L298N Motor Driver (x1) - https://geni.us/L298N

■ Limit Switch with roller arm (x2) - https://geni.us/ContactSwitch

■ 1602 LCD Shield with keypad (x1) - https://geni.us/1602LCDKeypad 

■ HC-05/HC-06 Bluetooth Module (x1) - https://geni.us/HC05Bluetooth

■ M3 and M5 Bolts  - https://geni.us/NutsAndBolts

M5x15mm (x2) M5x25mm (x3).  M3x

■ Rubber Bands - https://geni.us/RubberBands 

■ Dominoes  the ones I used  (x lots) - https://geni.us/Dominoes 

■ 4 Way Wire Connector (x1) - https://geni.us/WagoConnectors 

■ Electrical 'Hookup' wire - https://geni.us/22AWGWire

■ Filament for 3D printing:
   - Silky silver from Geeetech: https://geni.us/GeeetechSilkSilverPLA 
  - White PLA: https://geni.us/GeeetechWhitePLA
  
■ Velcro Cable Tidy (for domino ejector) - https://geni.us/VelcroCableTidy 

==========

The code can be downloaded from here: 
https://github.com/DIY-Machines/RC-Domino-Machine-V1

Guides for programming, FAQ, wiring diagram and helpful people available here:
https://www.diymachines.co.uk/rc-domino-laying-machine

3D printed parts can be downloaded from here:
https://www.thingiverse.com/thing:4694940

Support the channel with a one of donation and enjoy the not-yet-patented Rapid Domino Reloader Stick and some extra accessories to print for your dominoes: https://www.etsy.com/uk/listing/1077959216

==========

SAY THANKS:

Buy me a coffee to say thanks: https://ko-fi.com/diymachines
Support us on Patreon: https://www.patreon.com/diymachines

SUBSCRIBE: 
■ https://www.youtube.com/channel/UC3jc4X-kEq-dEDYhQ8QoYnQ?sub_confirmation=1

INSTAGRAM: https://www.instagram.com/diy_machines/?hl=en
FACEBOOK: https://www.facebook.com/diymachines/


Disclaimer:
The video is shared for demonstration purposes only.
