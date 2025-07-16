static const char MESSAGE[] = "I LOVE WILLY ";

static const uint8_t WINDOW_SIZE = 4;

static const uint8_t MESSAGES_SIZE = sizeof(MESSAGE) - 1;

static const uint32_t BAUD = 115200;
static const uint16_t DELAY = 1000;

static const uint8_t DIGITS = 10;
static const uint8_t INVALID_INDEX = 0;

static uint8_t ascii_to_index(const unsigned char c)
{
  if ((c >= '0') && (c <= '9'))
  return c - '0';
  if ((c >= 'A') && (c <= 'Z'))
  return c - 'A' + DIGITS;
  if ((c >= 'a') && (c <= 'z'))
  return c - 'a' + DIGITS;
  if ((c == " "))
  return 35;

  return INVALID_INDEX;
}


enum {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  DP,
  SEG_TOTAL
};
const uint8_t SEGPIN[SEG_TOTAL] = {
  [A] = 8,
  [B] = 4,
  [C] = 5,
  [D] = 7,
  [E] = 9,
  [F] = 6,
  [G] = 2,
  [DP] = 3
};

enum {
DIG1,
DIG2,
DIG3,
DIG4,
DIGTOTAL
};

const uint8_t DIGIT_TO_PIN[DIGTOTAL] = {
[DIG1] = 10,
[DIG2] = 11,
[DIG3] = 12,
[DIG4] = 13
};

static const uint8_t BITMASK[36] = {
  [0] = 0,
  [1] = 0,
  [2] = 0,
  [3] = 0,
  [4] = 0,
  [5] = 0,
  [6] = 0,
  [7] = 0,
  [8] = 0,
  [9] = 0,


  [10] = 0, //a
  [11] = 0, //b
  [12] = 0b00111001, //c
  [13] = 0, //d
  [14] = 0b01111001, //e
  [15] = 0, //f
  [16] = 0,
  [17] = 0,
  [18] = 0b00000100,
  [19] = 0,
  [20] = 0,
  [21] = 0b00111000, //L
  [22] = 0, //m
  [23] = 0b11010100, //n
  [24] = 0b01011100, //o
  [25] = 0b01110011, //p
  [26] = 0, //q
  [27] = 0b11010000,
  [28] = 0,
  [29] = 0b11111000,
  [30] = 0b10011100, //u
  [31] = 0b00011100, //v
  [32] = 0b10111000, //w
  [33] = 0,
  [34] = 0b01101110,
  [35] = 0
};

void setup() {
  for (uint8_t i = 0; i < SEG_TOTAL; i++) {
    pinMode(SEGPIN[i], OUTPUT);
  }
  for (uint8_t i = 0; i < DIGTOTAL; i++) {
    pinMode(DIGIT_TO_PIN[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  lights();
}

void lights() {
  static uint8_t index = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > DELAY) {
    index = (index + 1) % MESSAGES_SIZE;
    lastUpdate = millis();
  }

  for (int i = 0; i < DIGTOTAL; i++) {

    //turn everythhing off
    for (int j = 0; j < DIGTOTAL; j++) {
      digitalWrite(DIGIT_TO_PIN[j], HIGH);
    }

    digitalWrite(DIGIT_TO_PIN[i], LOW);

    uint8_t ascii = ascii_to_index(MESSAGE[i + index % MESSAGES_SIZE]);

    uint8_t mask = BITMASK[ascii];
    
    Serial.println(ascii);

    for (int seg = 0; seg < SEG_TOTAL; seg++) {
      digitalWrite(SEGPIN[seg], (mask & (1 << seg)) ? HIGH : LOW);
    }
    delay(5);
  }
}


