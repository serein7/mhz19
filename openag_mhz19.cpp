/**
 *  \file openag_Mhz19.cpp
 *  \brief Sensor module for temperature.
 */
#include "openag_mhz19.h"
#define READ_TIMEOUT 5000

Mhz19::Mhz19() {
  status_level = OK;
  status_msg = "";
  _send_air_carbon_dioxide = false;
  _time_of_last_reading = 0;
}

void Mhz19::begin() {
  Serial3.begin(9600);
  //Serial3.setTimeout(READ_TIMEOUT);
}

void Mhz19::update() {
  if (millis() - _time_of_last_reading > _min_update_interval) {
      readData();
      _time_of_last_reading = millis();
  }
}

bool Mhz19::get_air_carbon_dioxide(std_msgs::Int32 &msg) {
  msg.data = _co2_ppm;
  bool res = _send_air_carbon_dioxide;
  _send_air_carbon_dioxide = false;
  return res;
}

void Mhz19::readData() {
  byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  byte response[9];

  Serial3.write(cmd, 9);
  delay(100);
  Serial3.flush();
  memset(response, 0, 9);
  unsigned long timeout = millis() + READ_TIMEOUT;
  uint8_t inIndex = 0;
  while ( ((int32_t)(millis() - timeout) < 0) && (inIndex < sizeof(response)/sizeof(response[0])) ) {
    if (Serial3.available() > 0) {
        // read the incoming byte:
        response[inIndex] = Serial3.read();
        inIndex++;
    }
  }
//  Serial3.readBytes(response, 9);
  int i;
  byte crc = 0;
  for (i = 1; i < 8; i++) crc+=response[i];
  crc = 255 - crc;
  crc++;
  if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
    status_level = ERROR;
    status_msg = "CRC error " + String(crc) + " / "+ String(response[8]) + " => ";
    for (i = 0; i< 9; i++) status_msg = status_msg + String(response[i], HEX) + ",";
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    _co2_ppm = (256*responseHigh) + responseLow;
    _send_air_carbon_dioxide = true;
    status_level = OK;
    status_msg = "";
  }
}
