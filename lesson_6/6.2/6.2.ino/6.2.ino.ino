enum {
  STATE_WAIT,
  STATE_DISPENSE,
  STATE_RETURN,
  NUM_STATES
};

enum {
  ADD_QUARTERS,
  CHANGE_PRODUCT,
  RETURN_CHANGE,
  NUM_BUTTONS
};
static uint8_t state;

const uint16_t DELAY_FOR_LED = 1000;
const uint8_t DELAY = 250;
uint32_t previous_time;
uint32_t current_time;

const uint8_t SERIAL_BUFFER = 9600;

uint8_t PRODUCT_SELECTION_COUNTER;
uint8_t PRODUCT_SELECTION_PIN;

uint8_t RESET_PIN;

uint8_t MIN_NUM_QUARTERS;
static uint8_t current_number_quarters;
uint8_t QUARTER_ADDER_PIN;
uint32_t time_since_last_quarter;
uint32_t DELAY_FOR_QUARTERS = 30000;

struct PRODUCT {
  uint8_t QUANTITY;
  uint8_t COST_IN_QUARTERS;
  char NAME[10];
  uint8_t NUMBER;
};

struct PRODUCT CURRENT_PRODUCT;

uint8_t STATE_ARRAY[3] = {
  [STATE_WAIT] = 0,
  [STATE_DISPENSE] = 1,
  [STATE_RETURN] = 2
};

struct PRODUCT PRODUCT_LIST[3] = { { 0, 10, "CHIPS", 0 }, { 1, 15, "CANDY", 1 }, { 1, 20, "OPIUM", 2 } };


void setup() {
  state = 0;

  QUARTER_ADDER_PIN = 2;
  PRODUCT_SELECTION_PIN = 5;
  
  PRODUCT_SELECTION_COUNTER = 0;

  RESET_PIN = 9;
  
  previous_time = millis();


  Serial.begin(SERIAL_BUFFER);

  CURRENT_PRODUCT = PRODUCT_LIST[0];

  MIN_NUM_QUARTERS = 10;
  current_number_quarters = 0;

  pinMode(QUARTER_ADDER_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PRODUCT_SELECTION_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
}


void loop() {
  
  
  Serial.println("current_number_quarters : ");
  Serial.println(current_number_quarters);
  // Serial.println("PAUSE");
  Serial.println("current state :");
  Serial.println(state);
  // Serial.println("PAUSE");
  Serial.println("NAME");
  Serial.println(CURRENT_PRODUCT.NAME);
  Serial.println("PRODUCT NUMBER");
  Serial.println(PRODUCT_SELECTION_COUNTER);

  current_time = millis();

  if (BUTTON_DEBOUNCER(RESET_PIN)){
    state = STATE_RETURN;
  }
  
  if (current_time - previous_time >= DELAY_FOR_LED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    previous_time = current_time;
  }

  if (BUTTON_DEBOUNCER(QUARTER_ADDER_PIN)) {
    if (random(100) == 1) {
      Serial.println("Sorry this quarter is fake bro");
    }
    else {
       current_number_quarters += 1;
    }
  }
  STATE_MACHINE();
}

void STATE_MACHINE() {
  switch (state) {
    case 0:
      {
        AWAIT();
        break;
      }
    case 1:
      {
        DISPENSE();
        break;
      }
    case 2:
      {
        RETURN();
        break;
      }
  }
}

void AWAIT() {
  if (BUTTON_DEBOUNCER(PRODUCT_SELECTION_PIN)) {
    PRODUCT_SELECTION_COUNTER += 1;
    PRODUCT_SELECTION_COUNTER = PRODUCT_SELECTION_COUNTER % 3;
  }

  if (current_number_quarters >= MIN_NUM_QUARTERS) {
    CURRENT_PRODUCT = PRODUCT_LIST[PRODUCT_SELECTION_COUNTER];
    state = STATE_DISPENSE;
  }
}

void DISPENSE() {
  if (CURRENT_PRODUCT.QUANTITY == 0) {
    Serial.println("No more stock! Please choose different snack! ");
    state = STATE_WAIT;
  }

  if (current_number_quarters >= CURRENT_PRODUCT.COST_IN_QUARTERS && CURRENT_PRODUCT.QUANTITY > 0) {
    Serial.println("Payment is complete!");
    current_number_quarters = 0;
    CURRENT_PRODUCT.QUANTITY = CURRENT_PRODUCT.QUANTITY - 1;
    state = STATE_WAIT;
  }
}

void RETURN() {
  Serial.println("take ur quarters back brokie");
  current_number_quarters = 0;
  state = STATE_WAIT;
}

bool BUTTON_DEBOUNCER(uint8_t PIN) {
  bool inital = digitalRead(PIN);

  delay(DELAY);
    if (inital == digitalRead(PIN)) {
      return inital;
    }
  return !inital;
}