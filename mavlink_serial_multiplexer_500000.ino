
// Use all serial ports on the Pico
#include "mavlink/common/mavlink.h"  // Mavlink interface

unsigned long previousMillis = 0; 
const long interval = 1000;    



SerialPIO Serial3(2, 3);
SerialPIO Serial4(4, 5);
SerialPIO Serial5(6, 7);
SerialPIO Serial6(10, 11);

void setup() {
 // Serial.begin(115200);    // DISABLE USB SERVICE ITS VERY SLOW
  Serial1.begin(500000);  // 0,1
  Serial2.begin(500000);   // 8,9
  Serial3.begin(500000);
  Serial4.begin(500000);
  Serial5.begin(500000);
  Serial6.begin(500000);

}


void loop() {

  MAVLINK_HB();
  IN();

}



void IN() {

  if (Serial2.available()) {
    Serial1.write(Serial2.read());
  //  Serial1.flush();
  }

  if (Serial3.available()) {
    Serial1.write(Serial3.read());
 //   Serial1.flush();
  }


  if (Serial4.available()) {
    Serial1.write(Serial4.read());
   // Serial1.flush();
  }

  if (Serial5.available()) {
    Serial1.write(Serial5.read());
    //Serial1.flush();
  }

  if (Serial6.available()) {
    Serial1.write(Serial6.read());
   // Serial1.flush();
  }
  
}






void MAVLINK_HB() {


  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;


    uint16_t len;
    uint8_t autopilot_type = MAV_AUTOPILOT_INVALID;
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
