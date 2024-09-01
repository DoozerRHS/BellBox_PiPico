//tests - defines the unit testing with the google test framework
#include <gtest/gtest.h>
#include "../../BellBox/BellBox_PiPico/src/bellRingState.h"
#include "../../BellBox/BellBox_PiPico/src/bellboxTiming.h"
#include "../../BellBox/BellBox_PiPico/src/bellRingServoPositions.h"
#include "BellBoxBellTests.h"


//set of test to verify the bell ringining sequence on button demand and returning to "still" state.
struct BellBoxBellTests : public ::testing::Test
{
    bellRingState* bellRingCntrl;

    virtual void SetUp() override 
    {
        bellRingCntrl = new bellRingState();
    }

    virtual void TearDown() override 
    {
        bellRingCntrl->~bellRingState();    
    }
};

/// @brief Test no flag demanded when not pressed
/// @param  
/// @param  
TEST_F(BellBoxBellTests, NotPressed)
{
    
    float startPos = bellRingCntrl->getBellServoDemandedPosition();
    EXPECT_EQ(startPos, BELL_DOWN);

    for(int i = 0; i< 100; i++)
    {
        bellRingCntrl->doBellActionOnTick(i, false, false);
        EXPECT_EQ(startPos, bellRingCntrl->getBellServoDemandedPosition());
    }
}

enum demandButtonMidRing 
{
   NO = 0,
   FRONT = 1,
   BACK = 2
};



int isStillCheck(bellRingState* a_bellRingCntrl, float a_startPos, int a_tickCounter,int a_tickCount)
{

    int stopPos = a_tickCounter + a_tickCount;
    for(; a_tickCounter< stopPos; a_tickCounter++)
    {
        a_bellRingCntrl->doBellActionOnTick(a_tickCounter, false, false);
        EXPECT_EQ(a_startPos, a_bellRingCntrl->getBellServoDemandedPosition()) << "tick counter" << a_tickCounter << "stop pos" << stopPos;
    }
    return a_tickCounter;
}

int ringSequenceCheck(bellRingState* a_bellRingCntrl,  float a_startPos, int a_tickCounter, int a_repeatCount, demandButtonMidRing a_extraPress = demandButtonMidRing::NO)
{
    int positionChangeCount = 0;
    float lastPos = a_startPos;
    float thisPos = 0.0;
    // ring
    for (int repeat = 0; repeat < a_repeatCount; repeat++)
    {
        for (int index = 0; index < UP_DOWN_SEQUENCE_TICKS; index++)
        {
            //logic to inject extra bell press request when not "Still"
            bool frontDemand = false;
            bool backDemand = false;
            if(index == 1)
            {
                if(a_extraPress == demandButtonMidRing::FRONT)
                {
                    frontDemand = true;
                }
                if(a_extraPress == demandButtonMidRing::BACK)
                {
                    backDemand = true;
                }
            }
            a_tickCounter++;
            a_bellRingCntrl->doBellActionOnTick(a_tickCounter, frontDemand, backDemand);
            thisPos = a_bellRingCntrl->getBellServoDemandedPosition();
            // bell moves
            EXPECT_EQ(thisPos, BELL_BACK_AND_FORTH[index]) << "index=" << index << " repeat=" <<repeat << " tickCount="<<a_tickCounter;
            if (thisPos != lastPos)
            {
                lastPos = thisPos;
                positionChangeCount++;
            }
        }
    }

    // Expect position to change for the number of repeats * 2 (up and down = 2 per repeat)
    EXPECT_EQ(positionChangeCount, a_repeatCount*2);

    return a_tickCounter;
}


int ringPauseRingSequenceCheck(bellRingState* a_bellRingCntrl,  float a_startPos, int a_tickCounter, int a_repeatCount, int a_pauseBeforeRepeatTickCount, demandButtonMidRing a_extraPress = demandButtonMidRing::NO)
{
    int retVal = ringSequenceCheck(a_bellRingCntrl, a_startPos, a_tickCounter, a_repeatCount, a_extraPress);
    retVal += isStillCheck(a_bellRingCntrl, a_startPos, retVal, a_pauseBeforeRepeatTickCount);
    retVal += ringSequenceCheck(a_bellRingCntrl, a_startPos, retVal, a_repeatCount, a_extraPress);
    return retVal;
}


/// @brief Test bell does front ring sequence when front button pressed
/// @param  
/// @param  
TEST_F(BellBoxBellTests, FrontButtonPressedOnce)
{

    //start at still position
    float startPos = bellRingCntrl->getBellServoDemandedPosition();
    EXPECT_EQ(startPos, BELL_DOWN);
    //some ticks
    int tickCounter = 0;
    tickCounter+= isStillCheck(bellRingCntrl, startPos, tickCounter, 100);

    tickCounter++;
    //button demanded
    bellRingCntrl->doBellActionOnTick(tickCounter, true, false);


    tickCounter = ringPauseRingSequenceCheck(bellRingCntrl, startPos, tickCounter, FRONT_REPEAT, FRONT_PAUSE_BEFORE_REPEAT_TICKS);
    
    tickCounter+= isStillCheck(bellRingCntrl, startPos, tickCounter, 100);

}


/// @brief Test bell does back ring sequence when back button pressed
/// @param  
/// @param  
TEST_F(BellBoxBellTests, BackButtonPressedOnce)
{

    //start at still position
    float startPos = bellRingCntrl->getBellServoDemandedPosition();
    EXPECT_EQ(startPos, BELL_DOWN);
    //some ticks
    int tickCounter = 0;
    tickCounter+= isStillCheck(bellRingCntrl, startPos, tickCounter, 100);
    
    tickCounter++;
    //button demanded
    bellRingCntrl->doBellActionOnTick(tickCounter, false, true);

    tickCounter = ringPauseRingSequenceCheck(bellRingCntrl, startPos, tickCounter, BACK_REPEAT, BACK_PAUSE_BEFORE_REPEAT_TICKS);

    tickCounter+= isStillCheck(bellRingCntrl, startPos, tickCounter, 100);

}


/// @brief Test bell does front ring sequence when both front and back button pressed
/// @param  
/// @param  
TEST_F(BellBoxBellTests, FrontButtonWins)
{

   //start at still position
    float startPos = bellRingCntrl->getBellServoDemandedPosition();
    EXPECT_EQ(startPos, BELL_DOWN);
    //some ticks
    int tickCounter = 0;
    tickCounter+= isStillCheck(bellRingCntrl, startPos, tickCounter, 100);

    tickCounter++;
    //button demanded
    bellRingCntrl->doBellActionOnTick(tickCounter, true, true);


    tickCounter = ringPauseRingSequenceCheck(bellRingCntrl, startPos, tickCounter, FRONT_REPEAT, FRONT_PAUSE_BEFORE_REPEAT_TICKS);
    
    tickCounter+= isStillCheck(bellRingCntrl, startPos, tickCounter, 100);


}

