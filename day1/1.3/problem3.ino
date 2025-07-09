#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define INCREMENT_BUTTON 8
#define RESET_BUTTON 11
#define POLLING_DELAY 250
#define SERIAL_NUMBER 9600
#define OFFSET 3
//offset represents the fact that LED 1 is PORT 3

//left to right, 1 is all the way on the left
int COUNTER;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(INCREMENT_BUTTON, INPUT);
  COUNTER = 0;
  Serial.begin(SERIAL_NUMBER);
}

void binaryArray(int* arr) {
  int TEMP = COUNTER;
  int binaryArray[4] = {0};
  for (int i = 0; i < 4; i++) {
    arr[i] = TEMP % 2;
    TEMP /= 2;
  }
}

void loop() {

  // DECLARE THE ARRAY SIZE AND CALL HELPER
  int BINARY[4];
  binaryArray(BINARY);

  if (digitalRead(INCREMENT_BUTTON)) {
    delay(POLLING_DELAY);
    COUNTER += 1;
    
    // for (int i = 3; i >= 0; i--) {
    //   Serial.println(BINARY[i]);
    // }
    
    // Serial.println("NEW 4 BITS");
  }

  

  for (int i = 3; i >= 0; i--) {
    if (BINARY[i] == 1) {
      Serial.println(COUNTER);
      Serial.println(i+3);
      digitalWrite(i+OFFSET, HIGH);
    }
    else {
      digitalWrite(i+OFFSET, LOW);
    }
  }
}

