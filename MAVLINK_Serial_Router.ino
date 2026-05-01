
// Use all serial ports on the Pico
#include "mavlink/common/mavlink.h"  // Mavlink interface

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 1000;        // interval at which to blink (milliseconds)

SerialPIO Serial3(2, 3);
SerialPIO Serial4(4, 5);
SerialPIO Serial5(6, 7);
SerialPIO Serial6(10, 11);


void setup() {
  // Serial.begin(115200);    // DISABLE USB SERVICE ITS VERY SLOW
  Serial1.begin(1500000);  // 0,1
  Serial2.begin(115200);   // 8,9
  Serial3.begin(115200);
  Serial4.begin(115200);
  Serial5.begin(115200);
  Serial6.begin(115200);
}


void loop() {

  MAVLINK_HB();

  if (Serial1.available()) {
    char S1 = Serial1.read();

    Serial2.print(S1);
    Serial3.print(S1);
    Serial4.print(S1);
    Serial5.print(S1);
    Serial6.print(S1);
    Serial.println("S1");
  }

  
  if (Serial2.available()) {
    char S2 = Serial2.read();
    Serial1.print(S2);
    Serial3.print(S2);
    Serial4.print(S2);
    Serial5.print(S2);
    Serial6.print(S2);
    Serial.println("S2");
  }

  
  if (Serial3.available()) {
    char S3 = Serial3.read();
    Serial1.print(S3);
    Serial2.print(S3);
    Serial4.print(S3);
    Serial5.print(S3);
    Serial6.print(S3);
    Serial.println("S3");
  }

  
  if (Serial4.available()) {
    char S4 = Serial4.read();
    Serial1.print(S4);
    Serial2.print(S4);
    Serial3.print(S4);
    Serial5.print(S4);
    Serial6.print(S4);
    Serial.println("S4");
  }

  
  if (Serial5.available()) {
    char S5 = Serial5.read();
    Serial1.print(S5);
    Serial2.print(S5);
    Serial3.print(S5);
    Serial4.print(S5);
    Serial6.print(S5);
    Serial.println("S5");
  }

  
  if (Serial6.available()) {
    char S6 = Serial6.read();
    Serial1.print(S6);
    Serial2.print(S6);
    Serial3.print(S6);
    Serial4.print(S6);
    Serial5.print(S6);
    Serial.println("S6");
  }

}

  void MAVLINK_HB() {


    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;


      uint16_t len;
      uint8_t autopilot_type = MAV_TYPE_ONBOARD_CONTROLLER;
      uint8_t system_mode = MAV_MODE_PREFLIGHT;  ///< Booting up
      uint32_t custom_mode = 0;                  ///< Custom mode, can be defined by user/adopter
      uint8_t system_state = 0;                  ///< System ready for flight
      mavlink_message_t msg;
      uint8_t buf[MAVLINK_MAX_PACKET_LEN];
      int type = MAV_TYPE_ONBOARD_CONTROLLER;
      // Pack the message

      mavlink_msg_heartbeat_pack(1, 241, &msg, type, autopilot_type, system_mode, custom_mode, system_state);
      len = mavlink_msg_to_send_buffer(buf, &msg);
      Serial1.write(buf, len);
      Serial.println("HB");
    }
  }
