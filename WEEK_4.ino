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
  * sensor input to pin A1
  * Pin for the Y-axis of the joystick (A1)
  * Pin for sensor (A0)
*/

#include <LiquidCrystal.h>
#include <TimeLib.h>

int RS = 12;
int RE = 11;
int D4 = 5;
int D5 = 4;
int D6 = 3;
int D7 = 2;

LiquidCrystal lcd(RS, RE, D4, D5, D6, D7);


int enterButton = 9;
int returnButton = 8;
int menu = 1;
int menuHome = 1;

int msvalue = 0; // moisture sensor value
boolean flag = false;

int yAxisPin = A1; // Pin for the Y-axis of the joystick
int sensor_pin = A0; // Pin for sensor
int output_value = 0;
int mode;

void setup() {
  
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  pinMode(enterButton, INPUT_PULLUP);
  pinMode(returnButton, INPUT_PULLUP);
  pinMode(6,OUTPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(sensor_pin,INPUT);

  setTime(12, 0, 0, 30, 5, 23);
}

void loop() {
int yAxis = analogRead(yAxisPin);  // Joystick Y-axis

homeScreen();
}

void homeScreen() {
  
  // display welcome message
  lcd.setCursor(0, 0);
  lcd.print("SMART IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("Press Enter");
  lcd.blink();
  delay(400);

  // wait for enter button press
  while (!digitalRead(enterButton)){
    delay(10);
  menuLoading();
  // proceed to navigation menu
  navigationMenu();
  }
}

void menuLoading() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LOADING MENU");
  lcd.setCursor(0, 1);
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
    delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
}

void moistureReading(){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("MOISTURE VALUE:");
  while (true) {
    
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,550,0,0,100);
  lcd.setCursor(0,0);
  lcd.print("Mositure Value: ");
  lcd.setCursor(0,1);
  lcd.print(output_value + 86);
  lcd.println("%   ");
  delay(1000);

  if((output_value + 86) < 30 ){
    lcd.setCursor(8,8);
    lcd.print("->LOW ");
    if((mode == 0) && (hour() < 10 && hour() > 17) ){
      pinMode(6,HIGH); 
    }
    else if((mode == 1) && (hour() < 10 && hour() > 17) ){
      pinMode(6,HIGH);  
    }
    else{
      pinMode(6,LOW);
    }
  }
  else if((output_value + 86) > 30 && (output_value + 86) < 60){
    lcd.setCursor(8,8);
    lcd.print("->MID ");
    pinMode(6,LOW);
  }   
  else if((output_value + 86) > 60 ){
    lcd.setCursor(8,8);
    lcd.print("->HIGH");
    pinMode(6,LOW);
   } 
  }
}


void operationsMode() {
  const int numModes = 2;
  const char* modeOptions[] = {"Automatic", "Manual"};

  int selectedMode = 0;  // current selected mode

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OPERATION MODE");
  lcd.setCursor(0, 1);
  lcd.print(">");  // highlight selected mode
  lcd.print(modeOptions[selectedMode]);

  while (true) {
    int yAxis = analogRead(yAxisPin);  // Read the joystick's Y-axis

    if (yAxis > 900) {
      // DOWN button is pressed, select next mode
      selectedMode = (selectedMode + 1) % numModes;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OPERATION MODE");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected mode
      lcd.print(modeOptions[selectedMode]);
      delay(500);  // debounce the button
      while (yAxis < 100);  // wait for button release
    }

    if (yAxis < 100) {
      // UP button is pressed, select previous mode
      selectedMode = (selectedMode + numModes - 1) % numModes;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OPERATION MODE");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected mode
      lcd.print(modeOptions[selectedMode]);
      delay(500);  // debounce the button
      while (yAxis > 900);  // wait for button release
    }

    if (!digitalRead(enterButton)) {
      // ENTER button is pressed, execute selected mode
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Selected Mode:");
      lcd.setCursor(0, 1);
      lcd.print(modeOptions[selectedMode]);
      delay(1000);  // simulate execution delay

      if (selectedMode == 0) {
        // Automatic mode
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensetive Mode");
        // Implement logic for automatic mode
        mode = 0;
        delay(2000);
      } else if (selectedMode == 1) {
        // Manual mode
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Regular Mode");
        mode = 1; 
        // Implement logic for manual mode
        delay(2000);
      }

      // Clear the LCD for the next iteration
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OPERATION MODE");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected mode
      lcd.print(modeOptions[selectedMode]);
    }

    if (!digitalRead(returnButton)) {
      // RETURN button is pressed, return to previous menu
      return;
    }
  }
}

void digitalClockDisplay() {
  lcd.setCursor(0, 0);
  lcd.print(hour());
  lcd.print(":");
  printDigits(minute());
  lcd.print(":");
  printDigits(second());

  lcd.setCursor(0, 1);
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year());
}

void printDigits(int digits) {
  if (digits < 10) {
    lcd.print("0");
  }
  lcd.print(digits);
}

void dateAndTime() {
  const int numOptions = 2;
  const char* options[] = {"Display", "Set"};

  int selectedOption = 0;  // current selected option

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DATE & TIME");
  lcd.setCursor(0, 1);
  lcd.print(">");  // highlight selected option
  lcd.print(options[selectedOption]);

  while (true) {
    int yAxis = analogRead(yAxisPin);  // Read the joystick's X-axis

    if (yAxis > 900) {
      // RIGHT button is pressed, select next option
      selectedOption = (selectedOption + 1) % numOptions;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DATE & TIME");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected option
      lcd.print(options[selectedOption]);
      delay(500);  // debounce the button
      while (yAxis < 100);  // wait for button release
    }

    if (yAxis < 100) {
      // LEFT button is pressed, select previous option
      selectedOption = (selectedOption + numOptions - 1) % numOptions;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DATE & TIME");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected option
      lcd.print(options[selectedOption]);
      delay(500);  // debounce the button
      while (yAxis > 900);  // wait for button release
    }

    if (!digitalRead(enterButton)) {
      // ENTER button is pressed, execute selected option
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Selected Option:");
      lcd.setCursor(0, 1);
      lcd.print(options[selectedOption]);
      delay(1000);  // simulate execution delay

      if (selectedOption == 0) {
        // Display date and time
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("Displaying"); 
        lcd.println("Date & Time");
        lcd.clear();

       while(digitalRead(returnButton)){
          digitalClockDisplay();
          delay(1000);
       }
          
        }
        
      } else if (selectedOption == 1) {
        // Set date and time
         lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("Setting"); 
        lcd.println("Date & Time");
        lcd.clear();
        delay(1000);
        setDateAndTime();
      }

      // Clear the LCD for the next iteration
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DATE & TIME");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected option
      lcd.print(options[selectedOption]);
    

    if (!digitalRead(returnButton)) {
      // RETURN button is pressed, return to previous menu
      navigationMenu();
    }
  }
}

void waitForButtonRelease(int yAxis, int threshold) {
  while (yAxis < threshold) {
    yAxis = analogRead(yAxisPin);  // Read the joystick's X-axis
    delay(10);  // Adjust the delay as needed for your application
  }
}

void incrementFieldValue(int field, int& year, int& month, int& day, int& hour, int& minute, int& second) {
  switch (field) {
    case 0:  // Year
      year++;
      break;
    case 1:  // Month
      month = (month % 12) + 1;
      break;
    case 2:  // Day
      day = (day % 31) + 1;
      break;
    case 3:  // Hour
      hour = (hour % 23) + 1;
      break;
    case 4:  // Minute
      minute = (minute % 59) + 1;
      break;
    case 5:  // Second
      second = (second % 59) + 1;
      break;
  }
}

void decrementFieldValue(int field, int& year, int& month, int& day, int& hour, int& minute, int& second) {
  switch (field) {
    case 0:  // Year
      year--;
      break;
    case 1:  // Month
      month = (month + 10) % 12 + 1;
      break;
    case 2:  // Day
      day = (day + 30) % 31 + 1;
      break;
    case 3:  // Hour
      hour = (hour + 22) % 23 + 1;
      break;
    case 4:  // Minute
      minute = (minute + 58) % 59 + 1;
      break;
    case 5:  // Second
      second = (second + 58) % 59 + 1;
      break;
  }
}

void updateFieldDisplay(int currentField, int year, int month, int day, int hour, int minute, int second) {
  lcd.setCursor(11, 1);
  switch (currentField) {
    case 0:  // Year
      lcd.print(year);
      break;
    case 1:  // Month
      printDigits(month);
      break;
    case 2:  // Day
      printDigits(day);
      break;
    case 3:  // Hour
      printDigits(hour);
      break;
    case 4:  // Minute
      printDigits(minute);
      break;
    case 5:  // Second
      printDigits(second);
      break;
  }
}

void setDateAndTime() {
  int year = 2023; // Initial year value
  int month = 1;   // Initial month value
  int day = 1;     // Initial day value
  int hour = 0;    // Initial hour value
  int minute = 0;  // Initial minute value
  int second = 0;  // Initial second value

  int currentField = 0;  // Current field being edited (0 - year, 1 - month, 2 - day, 3 - hour, 4 - minute, 5 - second)

  const int numFields = 6;
  const char* fieldNames[] = {"Year", "Month", "Day", "Hour", "Minute", "Second"};

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SET DATE & TIME");

  while (true) {
    lcd.setCursor(0, 1);
    lcd.print(fieldNames[currentField]);
    lcd.print(": ");

    int yAxis = analogRead(yAxisPin);  // Read the joystick's X-axis

    if (yAxis > 900) {
      // RIGHT button is pressed, increment the value of the current field
      incrementFieldValue(currentField, year, month, day, hour, minute, second);
      updateFieldDisplay(currentField, year, month, day, hour, minute, second);
      delay(500);  // debounce the button
      waitForButtonRelease(yAxis, 100);
    }

    if (yAxis < 100) {
      // LEFT button is pressed, decrement the value of the current field
      decrementFieldValue(currentField, year, month, day, hour, minute, second);
      updateFieldDisplay(currentField, year, month, day, hour, minute, second);
      delay(500);  // debounce the button
      waitForButtonRelease(yAxis, 900);
    }

    if (!digitalRead(enterButton)) {
      // ENTER button is pressed, move to the next field or exit if all fields are edited
      currentField++;
      if (currentField >= numFields) {
        // All fields are edited, exit the function
        setTime(hour, minute, second, day, month, year); // Update the system time
        break;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SET DATE & TIME");
      delay(1000);  // simulate field transition delay
    }

    if (!digitalRead(returnButton)) {
      // RETURN button is pressed, discard changes and exit the function
      break;
    }
  }

  return;
}


void navigationMenu() {

  lcd.blink();
  int selectedItem = 0;  // current menu item
  const int numItems = 4;  // number of menu items
  const char* menuItems[] = {"MONITOR READINGS", "OPERATION MODE", "DATE AND TIME", "RETURN"};  // menu items
  
  // initialize LCD
  lcd.clear();
  
  // display initial menu item
  lcd.setCursor(0, 0);
  lcd.print("MAIN MENU");
  lcd.setCursor(0, 1);
  lcd.print(">");  // highlight selected item
  lcd.print(menuItems[selectedItem]);
  
  while (true) {
    int yAxis = analogRead(yAxisPin);  // Read the joystick's Y-axis    
    if (yAxis > 900) {
      // DOWN button is pressed, select next menu item
      selectedItem = (selectedItem + 1) % numItems;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected item
      lcd.print(menuItems[selectedItem]);
      delay(500);  // debounce the button
      while (yAxis < 100);  // wait for button release
    }
    if (yAxis < 100) {
      // UP button is pressed, select previous menu item
      selectedItem = (selectedItem + numItems - 1) % numItems;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print(">");  // highlight selected item
      lcd.print(menuItems[selectedItem]);
      delay(500);  // debounce the button
      while (yAxis > 900);  // wait for button release
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

      if(selectedItem == 0){
        delay(500);
        moistureReading();
      }
      else if(selectedItem == 1){
        operationsMode();
      }
      else if(selectedItem == 2){
        dateAndTime();
      }
      else if(selectedItem == 3){
         delay(500);
        homeScreen();
      }
      else{
        lcd.setCursor(0, 0);
        lcd.print(menuItems[selectedItem]);
        lcd.setCursor(0, 1);
        lcd.print(">"); 
      }
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
