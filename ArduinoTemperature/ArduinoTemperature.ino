// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 

#define BUZZER_PIN 6

// Debug printing
bool debugPrints = false; // interferes with serial output for processing
#define debugPrint(s) { if (debugPrints) Serial.println(s); }
#define debugPrintLn(s) { if (debugPrints) Serial.println(s); }

// Global objects 

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// LCD object
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup(void) 
{ 
  // start serial port 
  Serial.begin(115200); 
  debugPrintLn("Sportsfreunde Huegelstrasse ultimate temperature tool");

  // Start up the sensor library
  sensors.begin();

  // Detect and initialise LCD
  debugPrintLn("Checking for LCD");
  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x27);
  int error = Wire.endTransmission();
  debugPrint("Error: ");

  debugPrint(error);
  if (error == 0) {
    debugPrintLn(": LCD found.");
  } else {
    debugPrintLn(": LCD not found.");
  }
  lcd.begin(16, 2); // initialize the lcd

  lcd.clear();
  lcd.setBacklight(255);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
}


void loop(void) 
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus 

  debugPrint(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  debugPrintLn("DONE"); 

  debugPrint("Temperature is: "); 
  // You can have more than one DS18B20 on the same bus.  
  // 0 refers to the first IC on the wire
  float temperature = sensors.getTempCByIndex(0);
  Serial.println(temperature);

  // Print temperature on LCD
  lcd.clear();
  lcd.setCursor(0, 0); // top row
  lcd.print("Temperature C");
  lcd.setCursor(0, 1); // bottom row
  lcd.print(temperature);
  
  // Buzz
  analogWrite(BUZZER_PIN, temperature > 25.0 ? 5 : 0); // 5 is nice and silent
   
  delay(300);
} 

