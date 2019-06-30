# MorseCodeTrainer
Purpose:

  The purpose of this project is to develop a Morse code trainer that will display one of
  the keys A-Z and the player should enter the corresponding Morse code.
 
Description:

  The main function of this model is to develop a Morse code trainer that will display one of
  the keys A-Z. The user should enter the Morse code using a button and as a result
  corresponding alphabet will be displayed on the LCD screen which is connected to an
  Arduino board via the breadboard (as shown in design) and at the end circuit is
  completed by connecting it to a power source (batteries or USB source). Beeper and
  button are connected to the Arduino board via 2 digital i/o pins. Button transmits the data
  to the Arduino board, which will further assess the Morse code in dot or dash format.
  Whenever the button is pressed, the beeper will buzz for dot (1 sec) and dash (1 sec - 3
  sec) accordingly. If the button is pressed beyond 3 seconds, the Arduino will restrict the
  input to 3 second as a dash.
  
Hardware Components:

  To achieve the purpose of this project we need the following hardware components:
  
    ● Bread board;
    ● Arduino uno;
    ● Resistor;
    ● Beeper;
    ● Connecting Wires;
    ● Button.

Design:

  The final result of the project will look the following:
  And the schematic for the whole project is the fo
