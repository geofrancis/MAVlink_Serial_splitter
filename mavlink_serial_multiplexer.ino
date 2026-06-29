#include "mavlink/common/mavlink.h"

unsigned long previousMillis = 0;
const long interval = 1000;

SerialPIO ESC1(2, 3);
SerialPIO ESC2(4, 5);
SerialPIO ESC3(6, 7);
SerialPIO Serial6(10, 11);

// We remove Serial2 from the general inputs array so it can be handled separately
Stream* generalInputs[] = { &Serial6, &ESC1, &ESC2, &ESC3 };
const int numGeneralInputs = sizeof(generalInputs) / sizeof(generalInputs[0]);

const size_t BUFFER_SIZE = 128;

void setup() {
  Serial1.begin(460800);  // Connected to primary MAVLink device
  Serial2.begin(460800);  // Connected to secondary MAVLink device

  ESC1.begin(460800);
  ESC2.begin(460800);
  ESC3.begin(460800);
  Serial6.begin(460800);
}

void loop() {
  OUT();
  MAVLINK_HB(); // Heartbeat still goes out to Serial1
}

void loop1() {
  IN();
}

// Handles data coming INTO the system
void IN() {
  uint8_t buffer[BUFFER_SIZE];

  // 1. Bidirectional: Forward anything from Serial2 straight to Serial1
  int serial2Available = Serial2.available();
  if (serial2Available > 0) {
    size_t bytesToRead = (serial2Available > BUFFER_SIZE) ? BUFFER_SIZE : serial2Available;
    Serial2.readBytes(buffer, bytesToRead);
    Serial1.write(buffer, bytesToRead);
  }

  // 2. Forward data from general peripherals (ESCs, etc.) to Serial1
  for (int i = 0; i < numGeneralInputs; i++) {
    int availableBytes = generalInputs[i]->available();
    if (availableBytes > 0) {
      size_t bytesToRead = (availableBytes > BUFFER_SIZE) ? BUFFER_SIZE : availableBytes;
      generalInputs[i]->readBytes(buffer, bytesToRead);
      Serial1.write(buffer, bytesToRead);
    }
  }
}

// Handles data going OUT of the system from Serial1
void OUT() {
  uint8_t buffer[BUFFER_SIZE];
  int availableBytes = Serial1.available();

  if (availableBytes > 0) {
    size_t bytesToRead = (availableBytes > BUFFER_SIZE) ? BUFFER_SIZE : availableBytes;
    Serial1.readBytes(buffer, bytesToRead);

    // 1. Bidirectional: Forward everything from Serial1 straight to Serial2
    Serial2.write(buffer, bytesToRead);

    // 2. Broadcast the same data to the rest of the peripherals
    ESC1.write(buffer, bytesToRead);
    ESC2.write(buffer, bytesToRead);
    ESC3.write(buffer, bytesToRead);
    Serial6.write(buffer, bytesToRead);
  }
}

void MAVLINK_HB() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    uint8_t autopilot_type = MAV_AUTOPILOT_INVALID;
    uint8_t system_mode = MAV_MODE_PREFLIGHT;  
    uint32_t custom_mode = 2;                  
    uint8_t system_state = 0;                  
    int type = MAV_TYPE_ONBOARD_CONTROLLER;
    
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_heartbeat_pack(1, 241, &msg, type, autopilot_type, system_mode, custom_mode, system_state);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
    // Sends the heartbeat packet to Serial1
    Serial1.write(buf, len);
  }
}
