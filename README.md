# Smart_Irrigation_System
3rd year project by Danford Mponda and Kenneth Hastings

PROGRESS REPORT

Week1
-During this week the code developed was for a home screen of the main program to be displayed on the LCD screen

Week2
-Durring this week the code sets up an Arduino to control a liquid crystal display (LCD) and four buttons(ENTER, RETURN, UP, DOWN). It has two functions: homeScreen() and navigationMenu().

The homeScreen() function is called when the program starts and displays a welcome message on the LCD. The user is prompted to press the Enter button. Once the Enter button is pressed, the navigationMenu() function is called.

The navigationMenu() function displays a menu with four items on the LCD. The user can select a menu item using the up and down buttons. When the user presses the Enter button, the selected menu item is executed and a message is displayed on the LCD indicating that the item is being executed. After a short delay, the LCD is updated to show that the item has been executed.

If the user presses the Return button while on a menu item, the function returns to the main menu. If the user presses the Return button while on the main menu, the homeScreen() function is called. The program loops continuously between the home screen and the navigation menu until the Arduino is reset or turned off.




