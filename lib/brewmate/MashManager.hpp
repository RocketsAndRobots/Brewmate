#ifndef MASH_MANAGER_HPP
#define MASH_MANAGER_HPP

#include<Arduino.h>
#include<ArduinoSTL.h>

#include <TempReader.hpp>
#include <PowerControl.hpp>

struct MashRest
{
    MashRest(uint8_t temperature, uint8_t cookingTime) 
        : temp{temperature}, time{cookingTime} {}

    uint8_t temp = 0u;
    uint8_t time = 0u;
};

class MashManager
{     
    public:
        MashManager();
        ~MashManager() = default;

        void setStartTemperature(uint8_t startTemperature);
        void setEndTemperature(uint8_t endTemperature);
        uint8_t getStartTemperature();
        uint8_t getEndTemperature();
        void addRest(const uint8_t temperature, const uint8_t cookingTimeMinutes);
        MashRest getNextMashRest();
        bool restsRemaining();
        

    private:
        uint8_t m_startTemperature = 0u;
        uint8_t m_endTemperature   = 0u;
        uint8_t m_numberOfRests    = 0u;
        uint8_t m_currentRest      = 0u;
        std::vector<MashRest> m_mashRestPlan;
};

#endif // MASH_MANAGER_HPP