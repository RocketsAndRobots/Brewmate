#include <MashManager.hpp>

MashManager::MashManager()
{
    m_mashRestPlan.reserve(5);
}

void MashManager::setStartTemperature(uint8_t startTemperature)
{
    m_startTemperature = startTemperature;
}

void MashManager::setEndTemperature(uint8_t endTemperature)
{
    m_endTemperature = endTemperature;
}

uint8_t MashManager::getStartTemperature()
{
    return m_startTemperature;
}

uint8_t MashManager::getEndTemperature()
{
    return m_endTemperature;
}

void MashManager::addRest(const uint8_t temperature, const uint8_t cookingTimeMinutes)
{
    m_mashRestPlan.push_back(MashRest{temperature, cookingTimeMinutes});
    ++m_numberOfRests;
}

MashRest MashManager::getNextMashRest()
{
    MashRest nextMash{m_mashRestPlan[m_currentRest]};
    ++m_currentRest;
    return nextMash;
}

bool MashManager::restsRemaining()
{
    return ((m_numberOfRests - m_currentRest) > 0u);
}