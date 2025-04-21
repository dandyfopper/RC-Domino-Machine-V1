#include <Servo.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

/*******************************************************
V4 - Release Version - Bluetooth Control

DIY Machines project - Domino laying machine.

Original build video: https://youtu.be/QsAplPLtriw 
Original Help and FAQs: https://www.diymachines.co.uk/rc-domino-laying-machine

www.youtube.com/c/diymachines

This version replaces the deprecated Blynk app with standard Bluetooth control
********************************************************/

// Configuration variables
const byte carouselStackHeight = 22;  // How many dominoes are in a single fully stocked vertical stack on the carousel
const byte carouselQtyStacks = 7;     // How many vertical stacks are on the carousel

const byte amountToTurn = 40;          // How extreme the steering angle should be (1-90)
const byte carouselServoNeutral = 91;  // Neutral position for continuous rotation servo
const byte dcMotorSpeed = 120;         // Speed/power of the DC motor

// Bluetooth communication
SoftwareSerial BTSerial(12, 13);  // RX, TX pins for HC-05/HC-06 Bluetooth module
#define BT_BAUD_RATE 9600

// Hardware setup
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // LCD panel pins

Servo steeringServo;  // Servo for steering
Servo carouselServo;  // Servo for rotating the carousel

// L298N motor controller pins
const byte enA = 3;   // Enable pin for motor A
const byte in1 = A1;  // Input 1 for motor A
const byte in2 = A5;  // Input 2 for motor A

// Contact switches
const byte carouselSwitch = A2;  // Switch for carousel position
const byte ejectorSwitch = A3;   // Switch for ejector position

// State variables
byte remainingInCurrentStack = carouselStackHeight;
int remainingInCarousel = carouselStackHeight * carouselQtyStacks;
byte carouselSwitchState;
byte ejectorSwitchState;
boolean firebreakMode = false;
boolean bluetoothControlled = false;
char command = 'S';      // Default command (Stop)
byte steeringAngle = 90;  // Default steering angle (straight)
boolean carouselBusy = false; // Flag to track carousel operation

// Button values
byte lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

// Commands from Bluetooth
#define CMD_FORWARD 'F'        // Move forward
#define CMD_BACKWARD 'B'       // Move backward
#define CMD_LEFT 'L'           // Turn left
#define CMD_RIGHT 'R'          // Turn right
#define CMD_STOP 'S'           // Stop
#define CMD_CAROUSEL 'C'       // Rotate carousel
#define CMD_FIREBREAK_ON 'O'   // Enable firebreak
#define CMD_FIREBREAK_OFF 'X'  // Disable firebreak
#define CMD_STEERING 'T'       // Set steering angle (followed by 3-digit angle)

// Read the buttons on LCD shield
int read_LCD_buttons() {
  adc_key_in = analogRead(0);  // Read value from sensor

  // Threshold values for button detection
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 195) return btnUP;
  if (adc_key_in < 380) return btnDOWN;
  if (adc_key_in < 555) return btnLEFT;
  if (adc_key_in < 790) return btnSELECT;

  return btnNONE;  // Default case
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  BTSerial.begin(BT_BAUD_RATE);

  // Configure input pins
  pinMode(carouselSwitch, INPUT_PULLUP);
  pinMode(ejectorSwitch, INPUT_PULLUP);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  // Initialize servos
  steeringServo.attach(11);
  carouselServo.attach(2);
  carouselServo.write(carouselServoNeutral);
  steeringServo.write(90);

  // Initialize motor control pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Disable the DC motor initially
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Initial setup instructions
  printToLCD(F("Domino Machine"), F("V4.0 - Release"));
  delay(1000);

  printToLCD(F("Remove carousel"), F(""));
  delay(500);
  printToLCD(F("Is it removed?"), F("Press any button"));

  lcd_key = read_LCD_buttons();
  while (lcd_key == btnNONE) {
    lcd_key = read_LCD_buttons();
  }

  if (lcd_key != btnNONE) {
    layDomino(1, 'F');
  }

  printToLCD(F("Choose control:"), F("1=Manual 2=BT"));
  lcd_key = read_LCD_buttons();

  while (lcd_key == btnNONE) {
    lcd_key = read_LCD_buttons();
  }

  if (lcd_key == btnLEFT) {
    printToLCD(F("Manual Mode"), F("Ready"));
    delay(500);
  } else if (lcd_key == btnRIGHT) {
    bluetoothControl();
  }
}

void loop() {
  // Check for Bluetooth commands if in Bluetooth mode
  if (bluetoothControlled) {
    handleBluetooth();
    return;
  }

  // Manual control via LCD buttons
  lcd.setCursor(0, 1);
  lcd_key = read_LCD_buttons();

  switch (lcd_key) {
    case btnRIGHT:  // Button 2
      startProgramme();
      machineMove(1, 12); // F-Straight
      machineMove(1, 12); // F-Straight
      endProgramme();
      break;

    case btnLEFT:  // Button 1
      startProgramme();
      machineMove(1, 1); // F-Straight
      endProgramme();
      break;

    case btnUP:  // Up arrow
      startProgramme();
      machineMove(1, 4); // F-Straight
      machineMove(2, 4); // F-Right
      machineMove(3, 4); // F-Left
      machineMove(4, 4); // B-Straight
      endProgramme();
      break;

    case btnDOWN:  // Down arrow
      break;

    case btnSELECT:  // Tick/Select
      moveCarousel(1);
      break;

    case btnNONE:
      break;
  }
}

// Check for and handle Bluetooth commands
void handleBluetooth() {
  // Skip handling commands if carousel is busy
  if (carouselBusy) return;
  
  // Check if data is available from Bluetooth module
  if (BTSerial.available()) {
    char receivedChar = BTSerial.read();

    // Process command char
    switch (receivedChar) {
      case CMD_FORWARD:
        command = CMD_FORWARD;
        machineMove(1, 1); // Forward
        break;

      case CMD_BACKWARD:
        command = CMD_BACKWARD;
        machineMove(5, 1); // Backward
        break;

      case CMD_LEFT:
        steeringServo.write(90 + amountToTurn);
        updateScreen(3, 0); // Left
        break;

      case CMD_RIGHT:
        steeringServo.write(90 - amountToTurn);
        updateScreen(2, 0); // Right
        break;

      case CMD_STOP:
        command = CMD_STOP;
        updateScreen(0, 0); // Stop
        break;

      case CMD_CAROUSEL:
        moveCarousel(1);
        break;

      case CMD_FIREBREAK_ON:
        firebreak(true);
        break;

      case CMD_FIREBREAK_OFF:
        firebreak(false);
        break;

      case CMD_STEERING:
        // Read 3 more characters to get the angle (000-180)
        unsigned long startTime = millis();
        while ((BTSerial.available() < 3) && ((millis() - startTime) < 500)) {
          // Wait for data
          delay(5);
        }

        if (BTSerial.available() >= 3) {
          char val1 = BTSerial.read();
          char val2 = BTSerial.read();
          char val3 = BTSerial.read();

          if (isDigit(val1) && isDigit(val2) && isDigit(val3)) {
            int angle = (val1 - '0') * 100 + (val2 - '0') * 10 + (val3 - '0');

            // Limit angle to valid range
            if (angle >= 0 && angle <= 180) {
              steeringAngle = angle;
              steeringServo.write(steeringAngle);
              char buf[7];
              sprintf(buf, "S:%d", steeringAngle);
              updateScreenChar(buf, 0);
            }
          }
        }
        break;

      default:
        break;
    }

    // Send status back to the controller
    sendStatus();
  }
}

// Send machine status to Bluetooth client
void sendStatus() {
  char statusBuffer[20]; // Buffer for formatting status message
  sprintf(statusBuffer, "S:%d,F:%d,A:%d", remainingInCarousel, firebreakMode ? 1 : 0, steeringAngle);

  // Send status string through Bluetooth
  BTSerial.println(statusBuffer);
}

// Optimized - Direction by number: 
// 0=Stop, 1=F-Straight, 2=F-Right, 3=F-Left, 4=B-Straight, 5=Backward (no steer change)
void machineMove(byte direction, byte numberOfTurns) {
  while (numberOfTurns > 0) {
    countDominos();

    // Set steering and move
    switch (direction) {
      case 0: // Stop
        updateScreen(0, numberOfTurns);
        break;
      case 1: // Forward Straight
        updateScreen(1, numberOfTurns);
        steeringServo.write(90);
        layDomino(1, 'F');
        break;
      case 2: // Forward Right
        updateScreen(2, numberOfTurns);
        steeringServo.write(90 - amountToTurn);
        layDomino(1, 'F');
        break;
      case 3: // Forward Left
        updateScreen(3, numberOfTurns);
        steeringServo.write(90 + amountToTurn);
        layDomino(1, 'F');
        break;
      case 4: // Backward Straight
        updateScreen(4, numberOfTurns);
        steeringServo.write(90);
        layDomino(1, 'B');
        break;
      case 5: // Backward (no change to steering)
        updateScreen(5, numberOfTurns);
        layDomino(1, 'B');
        break;
    }
    numberOfTurns--;
  }
}

void countDominos() {
  if (firebreakMode == false) {
    remainingInCurrentStack = --remainingInCurrentStack;
    remainingInCarousel = --remainingInCarousel;
  }

  // Handle empty carousel
  if (remainingInCarousel == 0) {
    printToLCD(F("Carousel empty"), F("refill / replace"));
    delay(1000);
    printToLCD(F("When done press"), F("key to continue"));

    lcd_key = read_LCD_buttons();
    while (lcd_key == btnNONE) {
      lcd_key = read_LCD_buttons();
    }

    printToLCD(F("Continuing..."), F(""));
    moveCarousel(1);
    delay(500);
    remainingInCurrentStack = carouselStackHeight;
    remainingInCarousel = carouselStackHeight * carouselQtyStacks;
  }

  // Move to next stack if current is empty
  if (remainingInCurrentStack == 0) {
    moveCarousel(1);
    remainingInCurrentStack = carouselStackHeight;
  }
}

// Direction: 0=Stop, 1=Forward, 2=Right, 3=Left, 4=Reverse, 5=Backward
void updateScreen(byte direction, byte remainingTurns) {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // Display direction based on code
  switch (direction) {
    case 0:
      lcd.print(F("Stop"));
      break;
    case 1:
      lcd.print(F("Forward"));
      break;
    case 2:
      lcd.print(F("Forward right"));
      break;
    case 3:
      lcd.print(F("Forward left"));
      break;
    case 4:
      lcd.print(F("Reverse"));
      break;
    case 5:
      lcd.print(F("Backward"));
      break;
  }
  
  lcd.setCursor(0, 1);
  
  if (bluetoothControlled == false && remainingTurns > 0) {
    lcd.print(remainingTurns);
    lcd.print(F(" remaining."));
  } else if (bluetoothControlled) {
    lcd.print(F("BT: "));
    lcd.print(remainingInCarousel);
    lcd.print(F(" Left"));
  }
}

// Display custom text on screen (for steering angle display)
void updateScreenChar(const char* text, byte remainingTurns) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text);
  
  lcd.setCursor(0, 1);
  if (bluetoothControlled) {
    lcd.print(F("BT: "));
    lcd.print(remainingInCarousel);
    lcd.print(F(" Left"));
  }
}

void layDomino(byte remainingDominoes, char directionOfTravel) {
  ejectorSwitchState = digitalRead(ejectorSwitch);
  while (ejectorSwitchState == HIGH) {
    moveMachine(10, directionOfTravel);
    ejectorSwitchState = digitalRead(ejectorSwitch);
  }
  moveMachine(300, directionOfTravel);
}

void moveMachine(int distance, char directionOfTravel) {
  analogWrite(enA, dcMotorSpeed);

  if (directionOfTravel == 'F') {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  delay(distance);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  carouselServo.write(carouselServoNeutral);  // Keep carousel servo steady
}

void moveCarousel(byte numberOfTurns) {
  carouselBusy = true; // Set busy flag
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Spinning"));
  lcd.setCursor(0, 1);
  lcd.print(F("carousel..."));

  byte i = numberOfTurns;
  while (i > 0) {
    carouselSwitchState = digitalRead(carouselSwitch);

    while (carouselSwitchState == HIGH) {  // Not engaged
      carouselServo.write(105);
      carouselSwitchState = digitalRead(carouselSwitch);
      // Allow periodic Bluetooth checks
      if (bluetoothControlled && (random(100) > 95)) {
        BTSerial.read(); // Just clear any waiting data, don't process yet
      }
    }

    while (carouselSwitchState == LOW) {  // Engaged
      carouselServo.write(105);
      carouselSwitchState = digitalRead(carouselSwitch);
      // Allow periodic Bluetooth checks
      if (bluetoothControlled && (random(100) > 95)) {
        BTSerial.read(); // Just clear any waiting data, don't process yet
      }
    }

    carouselServo.write(carouselServoNeutral);

    if (i == 1) {
      bumpCarousel(170);
    }

    i--;
  }

  // Send status update if in Bluetooth mode
  if (bluetoothControlled) {
    sendStatus();
  }
  
  carouselBusy = false; // Clear busy flag
}

void bumpCarousel(int bumpSize) {
  delay(100);
  carouselServo.write(78);
  delay(bumpSize);  // Bump
  carouselServo.write(carouselServoNeutral);
}

void firebreak(bool toggle) {
  carouselBusy = true; // Set busy flag
  
  if (toggle == true) {
    firebreakMode = true;
    printToLCD(F("Moving to fire"), F("break position."));

    carouselSwitchState = digitalRead(carouselSwitch);
    while (carouselSwitchState == HIGH) {  // Not engaged
      carouselServo.write(100);
      carouselSwitchState = digitalRead(carouselSwitch);
    }

    carouselServo.write(carouselServoNeutral);
    carouselServo.write(80);
    delay(110);
    carouselServo.write(carouselServoNeutral);
  } else {
    firebreakMode = false;
    moveCarousel(7);
  }

  // Send status update if in Bluetooth mode
  if (bluetoothControlled) {
    sendStatus();
  }
  
  carouselBusy = false; // Clear busy flag
}

void startProgramme() {
  printToLCD(F("Play programme:"), F(""));
  delay(500);
  moveCarousel(1);
}

void endProgramme() {
  printToLCD(F("Programme"), F("completed."));
}

void bluetoothControl() {
  bluetoothControlled = true;
  printToLCD(F("Bluetooth Mode"), F("Connecting..."));

  delay(500);
  printToLCD(F("Bluetooth Ready"), F("Waiting for app"));

  // Send an initial status update
  sendStatus();

  while (true) {
    handleBluetooth();

    // Also check if there's incoming data on the regular Serial port (for testing)
    if (Serial.available()) {
      char c = Serial.read();
      
      // Forward the command from Serial to Bluetooth handler for testing
      switch (c) {
        case 'F':
        case 'B':
        case 'L':
        case 'R':
        case 'S':
        case 'C':
        case 'O':
        case 'X':
        case 'T':
          switch (c) {
            case CMD_FORWARD:
              command = CMD_FORWARD;
              machineMove(1, 1);
              break;
            case CMD_BACKWARD:
              command = CMD_BACKWARD;
              machineMove(5, 1);
              break;
            case CMD_LEFT:
              steeringServo.write(90 + amountToTurn);
              updateScreen(3, 0);
              break;
            case CMD_RIGHT:
              steeringServo.write(90 - amountToTurn);
              updateScreen(2, 0);
              break;
            case CMD_STOP:
              command = CMD_STOP;
              updateScreen(0, 0);
              break;
            case CMD_CAROUSEL:
              moveCarousel(1);
              break;
            case CMD_FIREBREAK_ON:
              firebreak(true);
              break;
            case CMD_FIREBREAK_OFF:
              firebreak(false);
              break;
            case CMD_STEERING:
              // For simple testing, set to 90 degrees
              steeringAngle = 90;
              steeringServo.write(steeringAngle);
              updateScreen(0, 0);
              break;
          }
          break;
      }
      sendStatus();
    }

    // Short delay to prevent tight loop
    delay(10);
  }
}

void printToLCD(const __FlashStringHelper* Line1, const __FlashStringHelper* Line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Line1);
  lcd.setCursor(0, 1);
  lcd.print(Line2);
}