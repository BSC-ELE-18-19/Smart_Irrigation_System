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
  * enter button input to pin 8
  * return button input to pin 9
  * up button input to pin 7
  * down button input to pin 6
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the library with the numbers of the interface pins

const int enterButton = 8; // the pin for button 1
const int returnButton = 9; // the pin for button 2
const int upButton = 7; // the pin for button 1
const int downButton = 6; // the pin for button 2


void setup() {
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  pinMode(enterButton,INPUT_PULLUP);
  pinMode(returnButton,INPUT_PULLUP);
  pinMode(upButton,INPUT_PULLUP);
  pinMode(downButton,INPUT_PULLUP);
}

void loop() {
  homeScreen();

}

void homeScreen(){
    // set the cursor to column 0, line 1
  lcd.setCursor(0, 0);
  // print the home screen
  lcd.print("SMART IRRIGATION");
  
  // set the cursor to column 1, line 1
  lcd.setCursor(0, 1);
  // print prompt
  lcd.print("Press Enter");
  lcd.blink(); // turn on the blinking cursor
  delay(100);

  // wait for enter button press
  if (!digitalRead(enterButton)) {
    delay(100);
    while (!digitalRead(enterButton));
    // proceed to navigation menu
    navigationMenu();
  }
}
void navigationMenu(){
    lcd.blink();
  int selectedItem = 0;  // current menu item
  const int numItems = 4;  // number of menu items
  const char* menuItems[] = {"Item 1", "Item 2", "Item 3", "Item 4"};  // menu items
  
  // initialize LCD
  lcd.clear();
  
  // display initial menu item
  lcd.setCursor(0, 0);
  lcd.print("MAIN MENU");
  lcd.setCursor(0, 1);
  lcd.print(">");  // highlight selected item
  lcd.print(menuItems[selectedItem]);
  
  while (true) {
    if (!digitalRead(downButton)) {
      // DOWN button is pressed, select next menu item
      selectedItem = (selectedItem + 1) % numItems;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected item
      lcd.print(menuItems[selectedItem]);
      delay(100);  // debounce the button
      while (!digitalRead(downButton));  // wait for button release
    }
    if (!digitalRead(upButton)) {
      // UP button is pressed, select previous menu item
      selectedItem = (selectedItem + numItems - 1) % numItems;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected item
      lcd.print(menuItems[selectedItem]);
      delay(100);  // debounce the button
      while (!digitalRead(upButton));  // wait for button release
    }
    if (!digitalRead(enterButton)) {
      // ENTER button is pressed, execute selected menu item
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Executing...");
      lcd.setCursor(0, 1);
      lcd.print(menuItems[selectedItem]);
      delay(1000);  // simulate execution delay
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(menuItems[selectedItem]);
      lcd.setCursor(0, 1);
      lcd.print(">"); 
    }
    if (!digitalRead(returnButton)) {
      // RETURN button is pressed, return to home screen or main menu
      if (selectedItem == 0) {
        // on main menu, return to home screen
        lcd.clear();
        homeScreen();
        return;
      } else {
        // on item in menu, return to main menu
        selectedItem = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MAIN MENU");
        lcd.setCursor(0, 1);
        lcd.print(">");  // highlight selected item
        lcd.print(menuItems[selectedItem]);
        delay(100);  // debounce the button
        while (!digitalRead(returnButton));  // wait for button release
      }
    }
  }
  
}
