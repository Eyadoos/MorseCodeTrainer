# MorseCodeTrainer
## Purpose:

  The purpose of this project is to develop a Morse code trainer that will display one of
  the keys A-Z and the player should enter the corresponding Morse code.
 
## Description:

  The main function of this model is to develop a Morse code trainer that uses the Koch method for training Morse code. It divides the alphabet into 4 difficulties based on the number of dots or dashes each letter has. The process starts by displaying letters from the  the first and easiest difficulty until and keeps repeating the letter in this difficulty until the user masters these letters then moves on to the next difficulty level where it selects letters from this level and any previous level. After selecting a letter from the corresponding difficulty level, the circuit plays the sound of this letter in Morse Code using a beeper. The user should enter the Morse code for the corresponding letter using a button.
  
  The beeper and
   the button are connected to the Arduino board via 2 digital i/o pins. Button transmits the data
  to the Arduino board, which will further assess the Morse code in dot or dash format.
  Whenever the button is pressed, we check the duration of the button press and comparte it to the dot duration which is 300ms or dash duration which is 1sec.
  
## Hardware Components:

  To achieve the purpose of this project we need the following hardware components:
  
    ● Bread board;
    ● Arduino uno;
    ● Resistor;
    ● Beeper;
    ● Connecting Wires;
    ● Button.

## Design:

  The final result of the project will look the following:
  
  ![Project Schematics - Fritzing](https://i.imgur.com/Vr5dwgv.png)
  
  And the schematic for the whole project is the fo

## Implementation

## License
[MIT](https://choosealicense.com/licenses/mit/)
