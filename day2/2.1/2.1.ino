
#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define POTENTIMETER 12
#define POLLING_DELAY 250
#define SERIAL_NUMBER 9600
#define OFFSET 3
#define BUFFER 100
#define DELAY 100
//offset represents the fact that LED 1 is PORT 3

//left to right, 1 is all the way on the left
int COUNTER;
int RAW_READING;
int MAPED = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(POTENTIMETER, INPUT);
  Serial.begin(SERIAL_NUMBER);
  RAW_READING = 0;
}

void loop() {
  int STORED_STATE_RAW_READING = analogRead(A0);
  
  delay(DELAY);

  int CURRENT_STATE_RAW_READING = analogRead(A0);

  //two examples, og 800 NEW 950, OG 800 new 650
  if (map(STORED_STATE_RAW_READING, 0, 1023, 0, 4) != map(CURRENT_STATE_RAW_READING, 0, 1023, 0, 4)) {

    int difference = CURRENT_STATE_RAW_READING - STORED_STATE_RAW_READING;
    if (difference >= BUFFER || difference <= -BUFFER) {
      MAPED = map(CURRENT_STATE_RAW_READING, 0, 1023, 0, 4);
    }
    MAPED = map(STORED_STATE_RAW_READING, 0, 1023, 0, 4);
  }
  if (CURRENT_STATE_RAW_READING == 1023) {
      MAPED = 4;
    }

  if (CURRENT_STATE_RAW_READING == 0) {
      MAPED = 0;
  }
  Serial.println(CURRENT_STATE_RAW_READING);
  Serial.println(STORED_STATE_RAW_READING);
  Serial.println(MAPED);
  
  switch (MAPED) {
    case 0:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      break;
    case 1:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      break;
    case 2:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      break;
    case 3:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      break;
    case 4:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      break;
  }
}

