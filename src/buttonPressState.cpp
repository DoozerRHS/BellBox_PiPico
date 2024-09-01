

#include "buttonPressState.h"
#include "bellboxTiming.h"


/// @brief Class to do button logic - one instance per bell-push supported.
/// class not aware of IO interface pure logic class.
/// @param a_gpio the button input pin - used to distinguish instances e.g. for debugging.
buttonPressState::buttonPressState(uint8_t a_gpio) 
{
    _buttonPinId = a_gpio;
}

/// @brief Destructor
buttonPressState::~buttonPressState() 
{
	// Nothing to do on destruction
}


/// @brief called once per main loop tick - to do button logic
/// @param a_ticksSinceBoot count of ticks since boot - will never wrap
/// @param a_buttonPressed true if button is pressed on this tick
void buttonPressState::doButtonActionOnTick(const uint64_t a_ticksSinceBoot, const bool a_buttonPressed)
{
    //Start of 5 min sequence detected
    if(_ticksFiveMinCountDown == 0 && a_buttonPressed)
    {
        _ticksFiveMinCountDown = FIVE_MINS_AS_TICKS;
        _ticksUntilSecondPushAllowed = BUTTON_LATCH_NORMAL_TICKS;
        _tickBellDemanded = a_ticksSinceBoot;  //remember tick when bell demanded.
        return;
    }

    //If button pressed within 5 minute segment for a subsequent ring
    if(a_buttonPressed && _ticksUntilSecondPushAllowed == 0)
    {
        //reset the count down for subsequent rings - longer period 40 seconds now.
         _ticksUntilSecondPushAllowed = BUTTON_LATCH_LONGER_WAIT_TICKS + 1;  //will get immediately deprecated.
        _tickBellDemanded = a_ticksSinceBoot;  //remember tick when bell demanded.
    }

    //if tracking when second press is permitted within a 5 min segment, decrement until zero
    if(_ticksUntilSecondPushAllowed > 0)
    {
        _ticksUntilSecondPushAllowed--;
    }

    //if tracking 5 min segment, decrement until zero
    if(_ticksFiveMinCountDown > 0)
    {
        _ticksFiveMinCountDown--;
    }

   
}

/// @brief in a button push sequence alerting period?
/// @return Returns true if within 5 minutes of the start of a button push sequence
bool buttonPressState::getFlagIsDemanded()
{
    return _ticksFiveMinCountDown > 0;
}

/// @brief tells caller if bell ring was demanded for this button on this tick
/// @param a_ticksSinceBoot counter of ticks since boot
/// @return true if bell was demanded to start rinnging on this tick
bool buttonPressState::bellDemandedThisTick(const uint64_t a_ticksSinceBoot)
{
    return a_ticksSinceBoot == _tickBellDemanded;
}
