#include <Arduino.h>
#include "MashManager.hpp"
#include "PowerControl.hpp"
#include "TempReader.hpp"

enum class EMashState : uint8_t
{
    PRESS_TO_START = 0U,
    HEAT_UP,
    WAIT_FOR_USER,
    RESTING_COOKING,
    MASH_FINISHED
};

constexpr bool IS_HOP_COOKING{true};
constexpr uint16_t MINUTES_IN_MILLIS{60000u};
constexpr uint16_t SECONDS_IN_MILLIS{1000u};
constexpr uint8_t SNOOZE_BUTTON_PIN{10u};
constexpr uint8_t BUZZER_PIN{9u};
unsigned long startTime{0u};
bool isWarmupDone{false};
EMashState currentMashState = EMashState::PRESS_TO_START;

const float hysteresis = 1.5;
MashManager mashManagerKoelsch;

void setRecipe(MashManager &mashManager)
{
    if (IS_HOP_COOKING)
    {
        mashManager.setStartTemperature(95u);
        mashManager.addRest(100u, 40u);
        mashManager.addRest(100u, 50u);
    }
    else
    {
        mashManager.setStartTemperature(61u);
        mashManager.addRest(56u, 10u);
        mashManager.addRest(66u, 60u);
        mashManager.addRest(77u, 10u);
    }
}

void buzzerStartSound()
{
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 2400);
    delay(600);
    noTone(BUZZER_PIN);
}

void buzzerEndSound()
{
    tone(BUZZER_PIN, 2400);
    delay(600);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
}

void heaterDecision(const uint8_t targetTemperature)
{
    EtempState newTempState{EtempState::tempOk};
    static EtempState formerTempState{EtempState::tempOk};

    newTempState = getTemperatureState(targetTemperature);

    if (formerTempState != newTempState)
    {

        formerTempState = newTempState;

        switch (newTempState)
        {
        case EtempState::tempTooLow:
            switchHeater(HEATER_ON);
            Serial.println("Heater turned on");
            break;

        case EtempState::tempTooHigh:
            switchHeater(HEATER_OFF);
            Serial.println("Heater turned off");
            break;

        default:
            // temperature state is on the right way, do nothing
            break;
        }
    }
}

void cookNextMashRest(MashManager &mashmanagerRecipe)
{
    MashRest currentMashRest = mashmanagerRecipe.getNextMashRest();
    unsigned long cookingTime{0u};
    startTime = millis();

    Serial.println("Starting new rest");
    Serial.println(currentMashRest.time);

    while (cookingTime < static_cast<unsigned long>(currentMashRest.time) * static_cast<unsigned long>(MINUTES_IN_MILLIS) )
    {
        heaterDecision(currentMashRest.temp);
        cookingTime = millis() - startTime;
        
        //TODO: Fix this print
        if ((cookingTime / SECONDS_IN_MILLIS) % 4 == 0)
        {
            Serial.print("Time cooked in milliseconds: ");
            Serial.println(cookingTime / MINUTES_IN_MILLIS);
        }
    }

    Serial.println("Ended current rest");
    tone(BUZZER_PIN, 2400);
    delay(2000);
    noTone(BUZZER_PIN);
}

void playSuccessSound()
{
    tone(BUZZER_PIN, 2400);
    delay(1000);
    noTone(BUZZER_PIN);
}

bool wasButtonPressed()
{
    return digitalRead(SNOOZE_BUTTON_PIN);
}

bool waitForConfirm()
{
    bool wasConfirmed = false;

    if (wasButtonPressed())
    {
        playSuccessSound();
        wasConfirmed = true;
        Serial.println("Confirmed by user");
    }
    else
    {
        tone(BUZZER_PIN, 2400);
        delay(400);
        noTone(BUZZER_PIN);
        delay(400);
    }
    
    return wasConfirmed;
}

void setup()
{
    pinMode(SNOOZE_BUTTON_PIN, INPUT);
    initPowerControl();
    initTempSensor();
    Serial.begin(9600);

    setRecipe(mashManagerKoelsch);
    setTwoPointHysteresis(hysteresis);
    buzzerStartSound();
    delay(1000u);
}

void loop()
{

    switch (currentMashState)
    {
    case EMashState::PRESS_TO_START:
    {
        Serial.println("Press to start brewing");

        if (waitForConfirm())
        {
            switchHeater(HEATER_ON);
            currentMashState = EMashState::HEAT_UP;
        }
    }
    break;

    case EMashState::HEAT_UP:
    {
        float targetTemperature = static_cast<float>(mashManagerKoelsch.getStartTemperature());
        float currentTemperature = getAverageTemperature(3u);
        heaterDecision(mashManagerKoelsch.getStartTemperature());
        Serial.print("Heating to start temperature ");
        Serial.print(currentTemperature);
        Serial.print(" / ");
        Serial.println(targetTemperature);

        if (currentTemperature >= (targetTemperature - hysteresis))
        {
            Serial.println("Target temp reached");
            currentMashState = EMashState::WAIT_FOR_USER;
        }
    }
    break;

    case EMashState::WAIT_FOR_USER:
    {
        Serial.println("Press to start next mash step");
        const bool isConfirmed = waitForConfirm();

        if (isConfirmed)
        {
            currentMashState = EMashState::RESTING_COOKING;
        }
        {
            heaterDecision(mashManagerKoelsch.getStartTemperature());
        }
    }
    break;

    case EMashState::RESTING_COOKING:
    {
        Serial.println("Current mash temperature is held");
        if (mashManagerKoelsch.restsRemaining())
        {
            cookNextMashRest(mashManagerKoelsch);
        }
        else
        {
            currentMashState = EMashState::MASH_FINISHED;
        }
    }
    break;

    case EMashState::MASH_FINISHED:
    {
        Serial.println("Mashing or cooking finished");
        buzzerEndSound();
        delay(5000u);
    }
    break;
    
    default:
        break;
    }
}