#ifndef POWER_CONTROL_HPP
#define POWER_CONTROL_HPP

#include <RCSwitch.h>

constexpr bool HEATER_ON = true;
constexpr bool HEATER_OFF = false;
constexpr uint8_t RADIO_TRANSMIT_PIN{3u};
constexpr int  PULSE_LENGHT_IN_US{202};
constexpr char OUTLET_TURN_ON_CMD[25]  = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
                                          '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '\0'};
constexpr char OUTLET_TURN_OFF_CMD[25] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
                                          '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '0', '\0'};


void initPowerControl();
void switchHeater(const bool newState);


#endif //POWER_CONTROL_HPP