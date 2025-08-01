uint32_t PREV;
uint32_t PREV_1;
const uint8_t DELAY_1 = 389;
const uint16_t DELAY_2 = 991;

const uint8_t TIME_ON = 500;

const uint8_t LED1 = 2;
const uint8_t LED2 = 3;

void setup() {
  PREV = millis();
  PREV_1 = millis();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  if (millis() - PREV >= DELAY_1) {
    digitalWrite(LED1, !digitalRead(LED1));
    PREV = millis();
  }

  if (millis() - PREV_1 >= DELAY_2) {
    digitalWrite(LED2, !digitalRead(LED2));
    PREV_1 = millis();
  }
}
