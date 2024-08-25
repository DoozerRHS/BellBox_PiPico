#ifndef BUTTON_PRESS_STATE_H
#define BUTTON_PRESS_STATE_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/types.h"


/// @brief Class to control servo via pico - translates desired servo position in degrees to appriopriate PWM output on control GPIO pin
class buttonPressState {
public:
	buttonPressState(const uint8_t a_buttonPinId);
	virtual ~buttonPressState();

    /// @brief Called on each small tick - detect is button pressed?  Do action
    void doButtonActionOnTick();

private:
	uint8_t _buttonPinId = 0;  //The GPIO pin as set up at construction
    uint32_t _ticksSinceLastButtonPress = UINT32_MAX;  //number of ticks since last button push - increments on each tick, stops incrementing at uint max value
    bool secondPushQueued = false;  //True if button pushed, whilst first push is still "latched"
    uint32_t _ticksUntilSecondPushAllowed = 0;
};

#endif /* BUTTON_PRESS_STATE_H */