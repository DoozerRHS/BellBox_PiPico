

#include "bellRingState.h"


/// @brief Class to do button logic - one instance per bell-push supported.
/// class not aware of IO interface pure logic class.
/// @param a_gpio the button input pin - used to distinguish instances e.g. for debugging.
bellRingState::bellRingState() 
{
    
}

/// @brief Destructor
bellRingState::~bellRingState() 
{
	// Nothing to do on destruction
}

/// @brief called once per main loop tick - to do button logic
/// @param a_ticksSinceBoot count of ticks since boot - will never wrap
/// @param a_button1Pressed true if button is pressed on this tick - FRONT
/// @param a_button2Pressed true if button is pressed on this tick - REAR
void bellRingState::doBellActionOnTick(const uint64_t a_ticksSinceBoot, const bool a_button1Pressed, const bool a_button2Pressed)
{
    //tick in state has passed
    if(_ticksBellStateCountDown > 0)
    {
        _ticksBellStateCountDown--;
    }

    //do state change logic  
    //Only accept new bell ring demands if bell is actually still already
    if(_currentBellState == BellState::STILL)
    {
        _bellServoPatternIndex  = 0;
        //Front button demand wins over rear
        if(a_button1Pressed)
        {
            _currentBellState = BellState::FRONT_RING;
            _ticksBellStateCountDown = UP_DOWN_SEQUENCE_TICKS*FRONT_REPEAT;
            _tickBellWasDemanded = a_ticksSinceBoot;
            return;
        }

        if(a_button2Pressed)
        {
            _currentBellState = BellState::BACK_RING;
            _ticksBellStateCountDown = UP_DOWN_SEQUENCE_TICKS*BACK_REPEAT;
            _tickBellWasDemanded = a_ticksSinceBoot;
        }
        return;
    }
    //If time to change state
    if(_ticksBellStateCountDown == 0)
    {
    
        switch(_currentBellState)
        {
            case BellState::FRONT_RING:
            {
                _currentBellState = BellState::FRONT_PAUSE;
                _ticksBellStateCountDown = FRONT_PAUSE_BEFORE_REPEAT_TICKS;
                break;
            }
            case BellState::FRONT_PAUSE:
            {
                _currentBellState = BellState::FRONT_REPREAT_RING;
                _ticksBellStateCountDown = UP_DOWN_SEQUENCE_TICKS*FRONT_REPEAT;
                break;
            }
            case BellState::FRONT_REPREAT_RING:
            case BellState::BACK_REPREAT_RING:
            {
                _currentBellState = BellState::STILL;
                _ticksBellStateCountDown = 0;
                break;
            }
            case BellState::BACK_RING:
            {
                _currentBellState = BellState::BACK_PAUSE;
                _ticksBellStateCountDown = BACK_PAUSE_BEFORE_REPEAT_TICKS;
                break;
            }
            case BellState::BACK_PAUSE:
            {
                _currentBellState = BellState::BACK_REPREAT_RING;
                _ticksBellStateCountDown = UP_DOWN_SEQUENCE_TICKS*BACK_REPEAT;
                break;
            }

        }
       
    }

    //if Ringning
    if(_currentBellState == BellState::FRONT_RING || 
       _currentBellState == BellState::FRONT_REPREAT_RING ||
       _currentBellState == BellState::BACK_RING ||
       _currentBellState == BellState::BACK_REPREAT_RING)
    {
        _currentBellPostion = BELL_BACK_AND_FORTH[_bellServoPatternIndex];
        _bellServoPatternIndex++;
        if(_bellServoPatternIndex >= UP_DOWN_SEQUENCE_TICKS)
        {
            //goto start of pattern to repeat
            _bellServoPatternIndex  = 0;
        }
 
    }
}

/// @brief Get the position for the bell servo - for a given tick
/// @return Demanded bell servo position - if servo is still there is no bell sound
float bellRingState::getBellServoDemandedPosition()
{
    return _currentBellPostion;
}