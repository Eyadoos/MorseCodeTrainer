//#include <MI0283QT2.h>
#include <math.h>

//Declare display !
//MI0283QT2 lcd;  //MI0283QT2 Adapter v1

const char CLEAR = 0;
const char DOT = 1;
const char DASH = 2;
const char alphabet[26][6] {
  { 'A', DOT, DASH, CLEAR, CLEAR, CLEAR},
  { 'B', DASH, DOT, DOT, DOT, CLEAR},
  { 'C', DASH, DOT, DASH, DOT, CLEAR},
  { 'D', DASH, DOT, DOT, CLEAR, CLEAR},
  { 'E', DOT, CLEAR, CLEAR, CLEAR, CLEAR},
  { 'F', DOT, DOT, DASH, DOT, CLEAR},
  { 'G', DASH, DASH, DOT, CLEAR, CLEAR},
  { 'H', DOT, DOT, DOT, DOT, CLEAR},
  { 'I', DOT, DOT, CLEAR, CLEAR, CLEAR},
  { 'J', DOT, DASH, DASH, DASH, CLEAR},
  { 'K', DASH, DOT, DASH, CLEAR, CLEAR},
  { 'L', DOT, DASH, DOT, DOT, CLEAR},
  { 'M', DASH, DASH, CLEAR, CLEAR, CLEAR},
  { 'N', DASH, DOT, CLEAR, CLEAR, CLEAR},
  { 'O', DASH, DASH, DASH, CLEAR, CLEAR},
  { 'P', DOT, DASH, DASH, DOT, CLEAR},
  { 'Q', DASH, DASH, DOT, DASH, CLEAR},
  { 'R', DOT, DASH, DOT, CLEAR, CLEAR},
  { 'S', DOT, DOT, DOT, CLEAR, CLEAR},
  { 'T', DASH, CLEAR, CLEAR, CLEAR, CLEAR},
  { 'U', DOT, DOT, DASH, CLEAR, CLEAR},
  { 'V', DOT, DOT, DOT, DASH, CLEAR},
  { 'W', DOT, DASH, DASH, CLEAR, CLEAR},
  { 'X', DASH, DOT, DOT, DASH, CLEAR},
  { 'Y', DASH, DOT, DASH, DASH, CLEAR},
  { 'Z', DASH, DASH, DOT, DOT, CLEAR}
};

const unsigned long dotDuration = 300; // duration [ms] of a dot; basic time unit
const unsigned long tolerance = 250; // duration that dash, dot or gap may deviate from the ideal length in order to still be recognized
const unsigned long dashDuration = 3 * dotDuration;
const unsigned long shortGap = dotDuration;
const unsigned long mediumGap = 3 * dotDuration;
const unsigned long longGap = 7 * dotDuration;
String global_string = ""; 

// current state of the button
enum State { 
  UP = 0, 
  DOWN = 1 } state;

// time index of the last button state change (from up to down or down to up)
unsigned long lastChange;

// duration of the last button press
unsigned long downDuration;

// dash-dot-sequence of the current character
//char character[5];

// index of the current character matching the played sequence
int characterIndex;

// Binary tree to store the alphabet depending on the sequence
char searchTree[31];

// the next thing to do
enum Action {
  START = 0,
  READ_DASHDOT = 1,
  READ_CHARACTER = 2,
  READ_WORD = 3,
  WAITING_FOR_INPUT = 4
} action;

// pin to which the button is connected
int buttonPin = 9;
// pin to which the buzzer is connected
int buzzerPin = A0;

void buildTree() {

    for (int i = 0; i < 31; i++) {
        searchTree[i] = CLEAR;
    }

    for (int i = 0; i < 26; i++) {
        //now we just add each character to the tree. I think that should be simple.
        int index = 0;
        for (int j = 0; j < 6; j++) { //iterate over the (up to) five dits and dahs
            //dahs (<0) go left dits (>0) go right
            if (alphabet[i][j] == DASH) { //if we have a dah
                //go to the node to the left
                index = 2 * index + 1;
            } else if (alphabet[i][j] == DOT) {
                index = 2 * index + 2;
            } else if (alphabet[i][j] == CLEAR || j == 5) { // fill in the charcter pointed to by i in the search tree
                searchTree[index] = alphabet[i][0];
                break;
            }
        }
    }
}

int nextCharacterIndex = 0;
int difficultyLevel = 1;
char letters[26];
char selectedCharacter;
//char userCharacter[5] = {CLEAR,CLEAR,CLEAR,CLEAR,CLEAR};

void getLettersInLevel() {

    for (int j = 0; j < 26; j++) {
        letters[j] = CLEAR;
    }

    int index = 0;
    for (int i = 0; i < 26; i++) {
        for (int j = 1; j < 6; j++) {
            if (alphabet[i][j] == CLEAR && j == difficultyLevel + 1) {
                letters[index] = alphabet[i][0];
                index++;
                break;
            }
        }
    }

}

void playCharacter() {

    char characterSequence[5] = {CLEAR,CLEAR,CLEAR,CLEAR,CLEAR};
    Serial.println("Playing Sequence...");
    for (int i = 0; i < 26; i++) {
        if (alphabet[i][0] == selectedCharacter) {
            for (int j = 1; j < 6; j++) {
                characterSequence[j-1] = alphabet[i][j];
                Serial.print(characterSequence[j-1]);
            }
        }
    }

    for (int i = 0; i < 5; i++) {
      if (characterSequence[i] == DOT) {
        tone(buzzerPin, 400, dotDuration);
        delay(dotDuration + (shortGap/2));
      } else if (characterSequence[i] == DASH) {
        tone(buzzerPin, 400, dashDuration);
        delay(dashDuration + (shortGap/2));
      } else {
        delay(shortGap);
        action = READ_DASHDOT;
        break;
      }
    }

}

void getNextCharacter() {

    int maxIndex = 0;
    int localLevel = difficultyLevel;
    while (localLevel > 0) {
        maxIndex += pow(2, localLevel);
        localLevel--;
    }

    if (nextCharacterIndex < maxIndex) {
        selectedCharacter = letters[nextCharacterIndex];
        playCharacter();
        nextCharacterIndex++;
        Serial.println("Selected Character to learn:");
        Serial.println(selectedCharacter);
    } else {
        Serial.println("Moving to next difficulty level...");
        nextCharacterIndex = 0;
        difficultyLevel++;
        getLettersInLevel();
        getNextCharacter();
    }

}

// reset the dash-dot-sequence of the current character
void clearCharacter() {
  characterIndex = 0;
//  for (int i = 0; i < 5; ++i) {
//    character[i] = CLEAR;
//  }
}

// determine the next dash or dot in the dash-dot sequence of the current character
void readDashDot() {
  Serial.print("Down duration was: ");
  Serial.println(downDuration);
  
  if (downDuration >= dashDuration - tolerance && downDuration < dashDuration + tolerance) {
//    character[characterIndex] = DASH;
    Serial.println("DASH");
//    characterIndex++;
    characterIndex = 2 * characterIndex + 1;
  } else if (downDuration >= dotDuration - tolerance && downDuration < dotDuration + tolerance) {
//    character[characterIndex] = DOT;
    Serial.println("DOT");
//    characterIndex++;
    characterIndex = 2 * characterIndex + 2;
  } else {
    Serial.println("Down duration unrecognized");
  }
}

void checkCharacter() {

//    char playedCharacter;
//    int index = 0;
//    for (int j = 0; j < 5; j++) { //iterate over the (up to) five dits and dahs
//        //dahs (<0) go left dits (>0) go right
//        if (userCharacter[j] == DASH) { //if we have a dah
//            //go to the node to the left
//            index = 2 * index + 1;
//        } else if (userCharacter[j] == DOT) {
//            index = 2 * index + 2;
//        } else if (userCharacter[j] == CLEAR || j == 5) { // fill in the charcter pointed to by i in the search tree
//            playedCharacter = searchTree[index];
//            break;
//        }
//    }

    char playedCharacter = searchTree[characterIndex];
    if (playedCharacter == selectedCharacter) {
        getNextCharacter();
    } else {
        playCharacter();
    }

}

// interpret the current dash-dot-sequence as a character
//char readCharacter() {
//  bool found;
//  for (int i = 0; i < 26; ++i) {
//    found = true;
//    for (int j = 0; found && j < 5; ++j) {
//      if (character[j] != alphabet[i][j + 1]) {
//        found = false;
//      }
//    }
//    if (found) {
//      return alphabet[i][0];
//    }
//  }
//  return 0;
//}
//
//# define LCD_WIDTH    320
//# define LCD_HEIGHT   240

void setup()
{

  //initialize LCD Display
//  Serial.println("Init Display...");
//  lcd.begin();
  
//  
//  lcd.fillScreen(RGB(255,255,255));
//  lcd.drawText((LCD_WIDTH/2)-50, 1, "Morse", RGB(0,0,0), RGB(255,255,255), 2);
//  lcd.drawText((LCD_WIDTH/2)-50, 50, "Code", RGB(0,0,0), RGB(255,255,255), 2);
//  lcd.drawText((LCD_WIDTH/2)-50, 100, "Generator", RGB(0,0,0), RGB(255,255,255), 2);

  Serial.begin(9600);

  Serial.println("Starting...");
  
  pinMode(buttonPin, INPUT_PULLUP);

  buildTree();
  
  state = UP;
  characterIndex = 0;
  downDuration = 0;
  lastChange = 0;
  action = START;
}





void loop() {
  
  State newState = digitalRead(buttonPin) ? UP : DOWN;
  
  //if the button is pressed, play a tone
  if (newState == DOWN) {
    tone(buzzerPin, 400, 100);
  } else {
    noTone(buzzerPin);
  }

    if (action == START) {

      Serial.println("Inside start action...");
//      delay(longGap);
      if (newState == DOWN && state == UP) {
        getLettersInLevel();
         getNextCharacter();
       }
    
    } else if (action == WAITING_FOR_INPUT) {

      if (newState == DOWN && state == UP) {
        delay(shortGap);
        action = READ_DASHDOT;
      }
    
    } else {

    if (newState == state) {
      
      if (newState == UP) {
        // calculate the the time the button has been in UP state
        unsigned long upDuration = (millis() - lastChange);
       
        if (action == READ_DASHDOT && upDuration >= shortGap - tolerance && upDuration < shortGap + tolerance) {
        
          readDashDot();
          action = READ_CHARACTER;
        
        } else if (action == READ_CHARACTER && upDuration >= mediumGap - tolerance && upDuration < mediumGap + tolerance) {
          
//          char c = readCharacter();
//          
//          if (c != 0) {
//            
//            // a valid character has been recognized
//            Serial.print("Read character: ");
//            Serial.println(c);
//            
//            global_string += c;
//  //          lcd.fillScreen(RGB(255,255,255)); 
//  //          lcd.drawText(5, 2, global_string, RGB(0,0,0), RGB(255,255,255), 2);
//            Serial.println(global_string);
//            
//          } else {
//            
//            Serial.println("Unrecognized character");
//  //          lcd.fillScreen(RGB(255,255,255)); 
//  //          lcd.drawText(5, 2, global_string, RGB(0,0,0), RGB(255,255,255), 2);
//  
//          }

          checkCharacter();
          clearCharacter();
        
//          action = READ_WORD;
        
        } else if (action == READ_WORD && upDuration >= longGap - tolerance && upDuration < longGap + tolerance) {
          
          Serial.println("Read next word");
  //        lcd.print(' ');
          action = READ_DASHDOT;
          
        }
      
      } else {
        
        downDuration = (millis() - lastChange);
      
      }
      
    } else {
      
      if (state == UP && newState == DOWN) {
        downDuration = 0;
      }
      
      lastChange = millis();
      state = newState;
      action = READ_DASHDOT;
    
    }

  }

}
