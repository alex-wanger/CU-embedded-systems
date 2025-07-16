#define LED_R 9
#define LED_G 10
#define LED_B 11
#define POTENTIMETER_1 A0
#define POTENTIMETER_2 A1
#define POTENTIMETER_3 A2
#define SERIAL_NUMBER 9600
#define OFFSET 8
#define BUFFER 100
#define DELAY 100

//offset represents the fact that LED 1 is PORT 3
//bc theyre sequential i can use loops to reduce redunant code
//left to right, 1 is all the way on the left

int RAW_READING_R;
int RAW_READING_G;
int RAW_READING_B;

int rawReadings[3] = {RAW_READING_R,RAW_READING_G, RAW_READING_B};

int MAPPED_R;
int MAPPED_G;
int MAPPED_B;

int mappedReadings[3] = {MAPPED_R, MAPPED_G, MAPPED_B};

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(POTENTIMETER_1, INPUT);
  pinMode(POTENTIMETER_2, INPUT);
  pinMode(POTENTIMETER_3, INPUT);
  Serial.begin(SERIAL_NUMBER);
  RAW_READING_R = 0;
  RAW_READING_G = 0;
  RAW_READING_B = 0;
  MAPPED_R = 0;
  MAPPED_G = 0;
  MAPPED_B = 0;
}

void setColor(int R, int G, int B) {
  analogWrite(LED_R, R);
  analogWrite(LED_G, G);
  analogWrite(LED_B, B);
}

void loop() {
  setColor(mappedReadings[0], mappedReadings[1], mappedReadings[2]);
  for (int i = 0; i < 3; i++) {
    Serial.println(mappedReadings[i]);
  }
}


void analogInputToMapped() {
  delay(100);
  //left to right
  rawReadings[0] = analogRead(A0);
  rawReadings[1] = analogRead(A1);
  rawReadings[2] = analogRead(A2);

  for (int i = 0; i < 3; i++) {
    mappedReadings[i] = map(rawReadings[i], 0, 1023, 0, 255);
  }
}


