#include "M5AtomS3.h"
#include <IRremote.hpp> 

#define DISABLE_CODE_FOR_RECEIVER
#define SEND_PWM_BY_TIMER
#define ATOMS3_IR_TX_PIN 4
#define ATOMS3_LIMIT_SWITCH_RX_PIN 8

uint8_t tx_repeats = 3;
uint8_t lastState = HIGH; // the previous state from the input pin
uint8_t currentState;     // the current reading from the input pin

/*
credit: https://diydrones.com/forum/topics/sony-a7-infrared-codes
*/
enum SonyCodes {
  PHOTO       = 0xB4B8F, // Shutter | take photo
  VIDEO       = 0x12B8F, // Start | stop video recording
  DISP        = 0x28B8F, // Display change cycles round
  MENU        = 0x1CB8F, // Enter menu | leave menu
  MENU_UP     = 0x5CB8F, // Menu up
  MENU_DOWN   = 0xDCB8F, // Menu down
  MENU_RIGHT  = 0xFCB8F, // Menu right
  MENU_LEFT   = 0x7CB8F, // Menu left
  MENU_OK     = 0x9CB91, // Menu OK
  Z_PLUS      = 0x52B8F, // Zoom in
  Z_MINUS     = 0xD2B8F  // Zoom out
};

void prv_tx_ir_sensor(SonyCodes code)
{
  for (uint8_t i = 0; i < tx_repeats; i++)
  {
    Serial.println("[" + String(i) + "] tx");
    IrSender.sendSonyMSB(code, 20);
    delay(40);
  }
}

void setup() {
    Serial.begin(9600);
    
    pinMode(ATOMS3_LIMIT_SWITCH_RX_PIN, INPUT_PULLUP);
    auto cfg = M5.config();
    AtomS3.begin(cfg);
    IrSender.begin(DISABLE_LED_FEEDBACK);
    IrSender.setSendPin(ATOMS3_IR_TX_PIN);
}

void loop() {
  AtomS3.update();

  // limit switch handling
  currentState = digitalRead(ATOMS3_LIMIT_SWITCH_RX_PIN);
  if(lastState == LOW && currentState == HIGH)
  {
    prv_tx_ir_sensor(PHOTO);
  }
  lastState = currentState;

  if (AtomS3.BtnA.wasPressed()) {
      Serial.println("Pressed");
  }
  if (AtomS3.BtnA.wasReleased()) {
      Serial.println("Released");
      prv_tx_ir_sensor(PHOTO);
  }
}
