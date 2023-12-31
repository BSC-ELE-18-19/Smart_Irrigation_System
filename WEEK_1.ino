/*
  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
  lcd.begin(16, 2); // Set up the number of columns and rows on the LCD.
}

void loop(){
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0);
  // print the home screen
  lcd.print("SMART IRRIGATION");
  
  // set the cursor to column 1, line 1
  lcd.setCursor(0, 1);
  // print prompt
  lcd.print("Press Enter");
  lcd.blink(); // turn on the blinking cursor
  delay(3000);
}
