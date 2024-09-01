#ifndef BUTTON_PRESS_STATE_H
#define BUTTON_PRESS_STATE_H

//Class is not target hardware (Pico) specific so can work be unit tested with any compiler and test framework.
// only use standard c/c++ headers
#include <cstdint>


/// @brief Class to do button logic - one instance per bell-push supported.
/// class not aware of IO interface pure logic class.
class buttonPressState {
public:
	/// @brief constructor - create instance per button to monitor
	/// @param a_buttonPinId the button input pin - used to distinguish instances e.g. for debugging.
	buttonPressState(const uint8_t a_buttonPinId);

	/// @brief tidy up destructor
	virtual ~buttonPressState();

    /// @brief called once per main loop tick - to do button logic
    /// @param a_ticksSinceBoot count of ticks since boot - will never wrap
    /// @param a_buttonPressed true if button is pressed on this tick
    void doButtonActionOnTick(const uint64_t a_ticksSinceBoot, const bool a_buttonPressed);

    /// @brief in a button push sequence alerting period?
    /// @return Returns true if within 5 minutes of the start of a button push sequence
    bool getFlagIsDemanded();

    /// @brief tells caller if bell ring was demanded for this button on this tick
    /// @param a_ticksSinceBoot counter of ticks since boot
    /// @return true if bell was demanded to start rinnging on this tick
    bool bellDemandedThisTick(const uint64_t a_ticksSinceBoot);

private:
	uint8_t _buttonPinId = 0;  //The GPIO pin as set up at construction
    uint32_t _ticksSinceLastButtonPress = UINT32_MAX;  //number of ticks since last button push - increments on each tick, stops incrementing at uint max value
    uint32_t _ticksUntilSecondPushAllowed = 0;
    uint32_t _ticksFiveMinCountDown = 0;  //Set to 5 mins worth of ticks on first detecting button click,  counts down until zero.
    uint64_t _tickBellDemanded = UINT64_MAX; //
};

#endif /* BUTTON_PRESS_STATE_H */