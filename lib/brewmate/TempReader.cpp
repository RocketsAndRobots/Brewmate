#include <TempReader.hpp>

OneWire oneWireInstance{SENSOR_DATA_PIN};
DallasTemperature sensorDs18b20{&oneWireInstance};

//TODO: handle this variable elsewhere
float ghysteresis = 0.0;

void initTempSensor()
{
    sensorDs18b20.begin();
}

float getAverageTemperature(const uint8_t numberMeasurements)
{
    float summarizedValues {0};

    for(int measurement = 0; measurement < numberMeasurements; ++measurement)
    {
        sensorDs18b20.requestTemperaturesByAddress(SENSOR_ADDRESS);
        summarizedValues += sensorDs18b20.getTempC(SENSOR_ADDRESS);;
        delay(100);
    }

    return summarizedValues / static_cast<float>(numberMeasurements);
}

EtempState getTemperatureState(const uint8_t targetTemperature)
{
    EtempState state {EtempState::tempTooHigh};

    if(targetTemperature > ghysteresis)
    {

        float averageTemp = getAverageTemperature(3u);
        Serial.print("Average temperature: ");
        Serial.println(averageTemp);

        if( averageTemp < (static_cast<float>(targetTemperature) - ghysteresis) )
        {
            state = EtempState::tempTooLow;
        }
        else if(averageTemp > (static_cast<float>(targetTemperature) - (ghysteresis / 2.0)) )
        {
            state = EtempState::tempTooHigh;
        } 
        else
        {
            state = EtempState::tempOk;
        }

    }

    return state;
}

void setTwoPointHysteresis(const float hysteresisDeviation)
{
    ghysteresis = hysteresisDeviation;
}