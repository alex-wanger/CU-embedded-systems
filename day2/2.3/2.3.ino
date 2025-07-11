#define LED_R 9
#define LED_G 10
#define LED_B 11
#define SERIAL_NUMBER 9600
#define OFFSET 8
#define HEX_LENGTH 6
#define DELAY 1000

//offset represents the fact that LED 1 is PORT 3
//bc theyre sequential i can use loops to reduce redunant code
//left to right, 1 is all the way on the left

int RAW_READING_R;
int RAW_READING_G;
int RAW_READING_B;

int rawReadings[3] = { RAW_READING_R, RAW_READING_G, RAW_READING_B };

int MAPPED_R;
int MAPPED_G;
int MAPPED_B;

int FINAL_VALUES[3] = {};

int mappedReadings[3] = { MAPPED_R, MAPPED_G, MAPPED_B };

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  RAW_READING_R = 0;
  RAW_READING_G = 0;
  RAW_READING_B = 0;
  MAPPED_R = 0;
  MAPPED_G = 0;
  MAPPED_B = 0;
  Serial.begin(SERIAL_NUMBER);
  FINAL_VALUES[0] = 0;
  FINAL_VALUES[1] = 0;
  FINAL_VALUES[2] = 0;
}

void setColor(int R, int G, int B) {
  analogWrite(LED_R, R);
  analogWrite(LED_G, G);
  analogWrite(LED_B, B);
}

void loop() {
  serial();
  setColor(FINAL_VALUES[0], FINAL_VALUES[1], FINAL_VALUES[2]);
}


void serial() {

  Serial.println("MADE IT HERE 0");

  Serial.println("Input a valid hexcode!");


  String hexcode = Serial.readString();

  hexcode.trim();

  Serial.println(hexcode);

  if (hexcode.length() == HEX_LENGTH) {
    delay(DELAY);
    for (int i = 0; i < hexcode.length(); i++) {
      if (!isHexadecimalDigit(hexcode[i])) {
        Serial.println("Invalid Hexcode");
        break;
      }
    }
    Serial.println("Valid Hexcode");
  }
  else {
    Serial.println("Hex Length Invalid, Length != 6");
  }

  char charArray[HEX_LENGTH] = {}; 

  hexcode.toCharArray(charArray, HEX_LENGTH + 1); //convert string to char so we can work with it

  int hexDigitsToDecimalValues[HEX_LENGTH] = {}; //need to create an int array of the same size to store the chars when they are converted to decimal

  for (int i = 0; i < HEX_LENGTH; i++) {
    hexDigitsToDecimalValues[i] = hexDigitToDecimal(charArray[i]);
  }

  Serial.println("MADE IT HERE 1");

  for (int j = 0; j < 6; j = j + 2) {
    int value = hexDigitsToDecimalValues[j] * 16 + hexDigitsToDecimalValues[j+1];
    FINAL_VALUES[j/2] = value;
  }

  Serial.println("MADE IT HERE 2");
}

int hexDigitToDecimal(char hexDigit) {
  switch (hexDigit) {
    case '0': return (0); break;
    case '1': return (1); break;
    case '2': return (2); break;
    case '3': return (3); break;
    case '4': return (4); break;
    case '5': return (5); break;
    case '6': return (6); break;
    case '7': return (7); break;
    case '8': return (8); break;
    case '9': return (9); break;
    case 'A': return (10); break;
    case 'a': return (10); break;
    case 'B': return (11); break;
    case 'b': return (11); break;
    case 'C': return (12); break;
    case 'c': return (12); break;
    case 'D': return (13); break;
    case 'd': return (13); break;
    case 'E': return (14); break;
    case 'e': return (14); break;
    case 'F': return (15); break;
    case 'f': return (15); break;
    default: ("Invalid Hex Digit");return (0); break;  // Handle invalid input
  }
}
