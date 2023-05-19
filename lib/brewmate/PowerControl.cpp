#include <PowerControl.hpp>

static RCSwitch rcSender{};

void initPowerControl()
{
    rcSender.enableTransmit(RADIO_TRANSMIT_PIN); // On Pin 3
    //rcSender.setProtocol(1);
    //rcSender.setPulseLength(PULSE_LENGHT_IN_US);
    //pinMode(LED_BUILTIN, OUTPUT);
}

void switchHeater(const bool newState)
{
    if (newState == HEATER_ON)
    {
        //digitalWrite(LED_BUILTIN, HIGH);
        //rcSender.send(OUTLET_TURN_ON_CMD);
        rcSender.switchOn("11111", "11111");
    }
    else
    {
        //digitalWrite(LED_BUILTIN, LOW);
        //rcSender.send(OUTLET_TURN_OFF_CMD);
        rcSender.switchOff("11111", "11111");
    }
}