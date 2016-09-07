/**
 *  \file openag_ds18b20.h
 *  \brief Sensor module for temperature.
 */
#ifndef MHZ19_H
#define MHZ19_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include "WProgram.h"
#endif

#include <openag_module.h>
#include <std_msgs/UInt32.h>
#include <SoftwareSerial.h>

/**
 * \brief Sensor module for temperature
 */
class Mhz19 : public Module {
  public:
    Mhz19();
    void begin();
    void update();
    bool get_air_carbon_dioxide(std_msgs::UInt32 &msg);

  private:
    SoftwareSerial _serial;
    bool _send_co2;
    UInt32 _co2_ppm;
    uint32_t _time_of_last_query;
    bool _waiting_for_conversion;
    // it seems sensor's internal logic takes co2 measure once per 6 sec
    // so no need to update more often
    const static uint32_t _min_update_interval = 7000;
};

#endif
