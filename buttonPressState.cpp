#include "buttonPressState.h"


/// @brief Class to control servo via pico - translates desired servo position in degrees to appriopriate PWM output on control GPIO pin
/// @param a_gpio servo control PWM gpio
buttonPressState::buttonPressState(uint8_t a_gpio) 
{
    _buttonPinId = a_gpio;
}

/// @brief Destructor
buttonPressState::~buttonPressState() 
{
	// Nothing to do on destruction
}

/**
 * move to angle: 0 to 180.
 * @param degree
 */
void buttonPressState::doButtonActionOnTick()
{
}