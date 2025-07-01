#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 0, A1, A2, 10, 9);

const byte ROWS = 4;
const byte COLS = 4;

String num1 = "";
String num2 = "";
String lastResult = "";
bool currentNumber = false;
bool error = false;
char operation = ' ';
char keys[ROWS][COLS] = {
  {'7','8','9','D'},
  {'4','5','6','M'},
  {'1','2','3','S'},
  {'C','0','E','A'}
};

byte rowPins[ROWS] = {12,11,8,7};
byte colPins[COLS] = {6,5,4,2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    switch (key) {

      case 'C':
        lcd.clear();
        reset();
        lastResult = "";
        return;
      break;


      case 'D':
      case 'M':
      case 'S':
      case 'A':
      if (!error)   
      {
        num1 = lastResult; 
      } 
      
        if (!currentNumber && !error) {
          lcd.clear();
          lcd.print(num1);
          operation = key;
          currentNumber = true;

          switch (key) {
            case 'D': lcd.print(" % "); break;
            case 'M': lcd.print(" * "); break;
            case 'S': lcd.print(" - "); break;
            case 'A': lcd.print(" + "); break;
          }
        }
        break;

      case 'E':
        if (operation == ' ' || num1 == "" || num2 == "") return;

        int number1 = num1.toInt();
        int number2 = num2.toInt();
        int result = 0;
        error = false;

        switch (operation) {
          case 'M':
            result = number1 * number2;
            break;
          case 'A':
            result = number1 + number2;
            break;
          case 'S':
            result = number1 - number2;
            break;
          case 'D':
            if (number2 != 0)
              result = number1 / number2;
            else
              error = true;
            break;
        }

        lcd.setCursor(0, 1);

        if (error) {
          lcd.print("Error");
          lastResult = "";
        } else {
          lcd.print(result);
          lastResult = String(result);
        }

        reset();
        break;
    }

    switch (key) {
      case '0'...'9':
        if (!currentNumber) {
          error = false;
          num1 += key;
          lastResult = num1;
          lcd.clear();
          lcd.print(num1);
        } else {
          num2 += key;
          lcd.print(key); 
        }
        break;
    }
  }
}

void reset() {
  num1 = "";
  num2 = "";
  operation = ' ';
  currentNumber = false;
}
