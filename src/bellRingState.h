#ifndef BELL_RING_STATE_H
#define BELL_RING_STATE_H

//Class is not target hardware (Pico) specific so can work be unit tested with any compiler and test framework.
// only use standard c/c++ headers
#include <cstdint>
#include "bellRingServoPositions.h"

/// @brief Class to do bell movement logic based on button demand
/// class not aware of IO interface pure logic class.
class bellRingState {
public:
	/// @brief constructor - create instance per bell to control - assuming 1 for two buttons in this design
	bellRingState();

	/// @brief tidy up destructor
	virtual ~bellRingState();

    /// @brief called once per main loop tick - to do button logic
    /// @param a_ticksSinceBoot count of ticks since boot - will never wrap
    /// @param a_button1Pressed true if button is pressed on this tick - FRONT
    /// @param a_button2Pressed true if button is pressed on this tick - REAR
    void doBellActionOnTick(const uint64_t a_ticksSinceBoot, const bool a_button1Pressed, const bool a_button2Pressed);

    /// @brief Get the position for the bell servo - for a given tick
    /// @return Demanded bell servo position - if servo is still there is no bell sound
    float getBellServoDemandedPosition();

private:

    enum BellState
    {
        STILL = 0,
        FRONT_RING,
        FRONT_PAUSE,
        FRONT_REPREAT_RING,
        BACK_RING,
        BACK_PAUSE,
        BACK_REPREAT_RING
    };

    BellState _currentBellState = BellState::STILL;
    uint32_t _ticksBellStateCountDown = 0;  //Set to num ticks for state to execute - counts down until zero - then moves into next state unless "Still" in which case will always be zero.
    uint64_t _tickBellWasDemanded = UINT64_MAX; //
    float _currentBellPostion = BELL_DOWN;
    uint8_t _bellServoPatternIndex = 0;  //index into bell up down servo position pattern array definition
};

#endif /* BELL_RING_STATE_H */