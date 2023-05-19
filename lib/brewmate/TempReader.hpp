#ifndef TEMP_READER_HPP
#define TEMP_READER_HPP

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

enum class EtempState : uint8_t 
{
    tempTooLow  = 0u, 
    tempTooHigh = 1u,
    tempOk      = 2u 
};

constexpr uint8_t SENSOR_DATA_PIN {5u};
constexpr uint8_t SENSOR_ADDRESS [8] = {0x28, 0x7B, 0x3B, 0x56, 0xB5, 0x01, 0x3C, 0x0B};


void initTempSensor();
float getAverageTemperature(const uint8_t numberMeasurements);
EtempState getTemperatureState(const uint8_t targetTemperature);
void setTwoPointHysteresis(const float hysteresisDeviation);

#endif // TEMP_READER_HPP