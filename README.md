# Random Dice Generator (Arduino)



## Description 

This is a random number generator which will simulate dice throws in Advanced Dungeons&Dragons game. The game uses various types of polyhedral dices with different numbers of sides (d4, d6, d8, ...). E.g. 4d8 means that we have a dice with 8 sides and it should be thrown 4 times, so generated number will represent the sum of these 4 throws. 

## How to use?

Requirements: Arduino Uno R3 with multi function shield

1) Install solution.ino file 
2) Open in Arduino IDE 
3) Connect Arduino Uno to computer and upload the project 

On a multifunction shield there are 3 buttons and a LED display. There two modes` Normal Mode and Configuration Mode. Normal mode displays the last generated number, while Configuration Mode represents the type of dice and number of throws. 

**Button 1**

- If in Configuration Mode, button press switches the dice to Normal Mode
- When pressed down, the random number is being generated, and when released the generated number will be displayed


**Button 2**

- If in Normal Mode, button press switches the dice to Configuration Mode
- Increments the number of throws (if 9 is exceeded, the number returns to 1)



**Button 3**

- If in Normal Mode, button press switches the dice to Configuration Mode
- Changes the dice type from 1 to 100. 

