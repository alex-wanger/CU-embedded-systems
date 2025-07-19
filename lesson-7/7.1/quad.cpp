#include "pins_arduino.h"
#include <cstdint>
#include <Arduino.h>
#include "quad.h"
#include <stdint.h>

struct STATE {
  uint8_t DT;
  uint8_t CLK;
}

static struct PREVIOUS_STATE;

static uint32_t steps;
static uint8_t DT_PIN = 2;
static uint8_t CLK_PIN = 3;

static uint8_t left_reading;
static uint8_t right_reading;

uint32_t encoder_steps() {

}

void encoder_setup() {
  pinMode(DT_PIN, INPUT);
  pinMode(CLK_PIN, INPUT);

  PREVIOUS_STATE.CLK = digitalRead(CLK_PIN);
  PREVIOUS_STATE.DT = digitalRead(DT_PIN);

  attachInterrupt(digitalPinToInterrupt(CLK_PIN), interrupt, CHANGE);
}

static void interrupt() {
  i
}
