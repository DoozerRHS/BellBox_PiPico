//tests - defines the unit testing with the google test framework
#include <gtest/gtest.h>
#include "../../BellBox/BellBox_PiPico/src/buttonPressState.h"
#include "../../BellBox/BellBox_PiPico/src/bellboxTiming.h"

#define BUTTON1_PRESS_TICK 100
#define BUTTON2_PRESS_TICK 240

struct BellBoxButtonTests : public ::testing::Test
{
    buttonPressState* btn1;
    buttonPressState* btn2;

    virtual void SetUp() override 
    {
        btn1 = new buttonPressState(1);
        btn2 = new buttonPressState(2);
    }

    virtual void TearDown() override 
    {
        btn1->~buttonPressState();    
        btn2->~buttonPressState();    
    }
};

/// @brief Test no flag demanded when not pressed
/// @param  
/// @param  
TEST_F(BellBoxButtonTests, NotPressed)
{
    
    for(int i = 0; i< 10; i++)
    {
        btn1->doButtonActionOnTick(i, false);
        btn2->doButtonActionOnTick(i, false);
        EXPECT_FALSE(btn1->getFlagIsDemanded());
        EXPECT_FALSE(btn2->getFlagIsDemanded());
    }
}

/// @brief Test flag is demanded when one press is made and it stays demanded for 5 minutes (only) 5 mins is 6000 ticks.
/// @param  
/// @param  
TEST_F(BellBoxButtonTests, ButtonsPressed)
{

    for(uint64_t i = 0; i< 6500; i++)
    {
        bool btn1Press =  i == BUTTON1_PRESS_TICK;
        bool btn2Press = i == BUTTON2_PRESS_TICK;
        btn1->doButtonActionOnTick(i, btn1Press);
        btn2->doButtonActionOnTick(i, btn2Press);
        EXPECT_EQ(btn1->getFlagIsDemanded(), (i>=BUTTON1_PRESS_TICK && i<(BUTTON1_PRESS_TICK+FIVE_MINS_AS_TICKS)));
        EXPECT_EQ(btn2->getFlagIsDemanded(), (i>=BUTTON2_PRESS_TICK && i<(BUTTON2_PRESS_TICK+FIVE_MINS_AS_TICKS)));
        EXPECT_EQ(btn1->bellDemandedThisTick(i), btn1Press);
        EXPECT_EQ(btn2->bellDemandedThisTick(i), btn2Press);
    }
}


/// @brief Test bell is demanded at 0, 20 and then every 40 seconds until 5 mins is up then reverts to 20 seconds repeat.
/// @param  
/// @param  
TEST_F(BellBoxButtonTests, ButtonsPressedALotLimitBellRing)
{

    int bt1FlagDemandedCount = 0;
    int bt2FlagDemandedCount = 0;
    int bt1BellDemandedCount = 0;
    int bt2BellDemandedCount = 0;
    
    //ticking for 13,000 ticks = 13,000 * 50 = 650,000 ms = 650s or 10 mins 50 seconds
    for(int i = 0; i< 13000; i++)
    {
        //debug catcher
        if(i == 6100)
        {
            i = i;
        }

        //press the buttons a LOT way more than the "repeat limits" permit acceptance of.
        bool btn1Press =  (i >= BUTTON1_PRESS_TICK) && i % 4 == 0;
        bool btn2Press = (i >= BUTTON2_PRESS_TICK) && i% 4 == 0;
        btn1->doButtonActionOnTick(i, btn1Press);
        btn2->doButtonActionOnTick(i, btn2Press);
        if(btn1->getFlagIsDemanded())
        {
            bt1FlagDemandedCount++;
        }
        if(btn2->getFlagIsDemanded())
        {
            bt2FlagDemandedCount++;
        }

     

        bool btn1BellExpected = (i == BUTTON1_PRESS_TICK ) || //at tick
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 4) ||  //tick + next press (4 ticks) after 20 secs wait
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 8 + BUTTON_LATCH_LONGER_WAIT_TICKS) ||  //tick + 20 secs + 40 secs
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 12 + BUTTON_LATCH_LONGER_WAIT_TICKS *2) || //40 secs later
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 16 + BUTTON_LATCH_LONGER_WAIT_TICKS *3) || //40 secs later
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 20 + BUTTON_LATCH_LONGER_WAIT_TICKS *4) || //40 secs later
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 24 + BUTTON_LATCH_LONGER_WAIT_TICKS *5) || //40 secs later
                                (i == BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 28 + BUTTON_LATCH_LONGER_WAIT_TICKS *6) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK  + 4 ) || //at tick
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 8) ||  //tick + next press (4 ticks) after 20 secs wait
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 12 + BUTTON_LATCH_LONGER_WAIT_TICKS) ||  //tick + 20 secs + 40 secs
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 16 + BUTTON_LATCH_LONGER_WAIT_TICKS *2) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 20 + BUTTON_LATCH_LONGER_WAIT_TICKS *3) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 24 + BUTTON_LATCH_LONGER_WAIT_TICKS *4) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 28 + BUTTON_LATCH_LONGER_WAIT_TICKS *5) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 32 + BUTTON_LATCH_LONGER_WAIT_TICKS *6)  ||  //40 secs later
                                (i == FIVE_MINS_AS_TICKS*2 + BUTTON1_PRESS_TICK  + 8 ) || //after two lots of 5 minutes!
                                (i == FIVE_MINS_AS_TICKS*2 + BUTTON1_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 12);  //tick + next press (4 ticks) after 20 secs wait

        bool btn2BellExpected = (i == BUTTON2_PRESS_TICK ) || //at tick
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 4) ||  //tick + next press (4 ticks) after 20 secs wait
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 8 + BUTTON_LATCH_LONGER_WAIT_TICKS) ||  //tick + 20 secs + 40 secs
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 12 + BUTTON_LATCH_LONGER_WAIT_TICKS *2) || //40 secs later
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 16 + BUTTON_LATCH_LONGER_WAIT_TICKS *3) || //40 secs later
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 20 + BUTTON_LATCH_LONGER_WAIT_TICKS *4) || //40 secs later
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 24 + BUTTON_LATCH_LONGER_WAIT_TICKS *5) || //40 secs later
                                (i == BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 28 + BUTTON_LATCH_LONGER_WAIT_TICKS *6) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK  + 4 ) || //at tick
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 8) ||  //tick + next press (4 ticks) after 20 secs wait
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 12 + BUTTON_LATCH_LONGER_WAIT_TICKS) ||  //tick + 20 secs + 40 secs
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 16 + BUTTON_LATCH_LONGER_WAIT_TICKS *2) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 20 + BUTTON_LATCH_LONGER_WAIT_TICKS *3) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 24 + BUTTON_LATCH_LONGER_WAIT_TICKS *4) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 28 + BUTTON_LATCH_LONGER_WAIT_TICKS *5) || //40 secs later
                                (i == FIVE_MINS_AS_TICKS + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 32 + BUTTON_LATCH_LONGER_WAIT_TICKS *6)  ||  //40 secs later
                                (i == FIVE_MINS_AS_TICKS*2 + BUTTON2_PRESS_TICK  + 8 ) || //after two lots of 5 minutes!
                                (i == FIVE_MINS_AS_TICKS*2 + BUTTON2_PRESS_TICK + BUTTON_LATCH_NORMAL_TICKS + 12);  //tick + next press (4 ticks) after 20 secs wait

        EXPECT_EQ(btn1->bellDemandedThisTick(i), btn1BellExpected) << "tick = " << i;
        EXPECT_EQ(btn2->bellDemandedThisTick(i), btn2BellExpected) << "tick = " << i;
        
        //count successfull ring demands
        if(btn1->bellDemandedThisTick(i))
        {
            bt1BellDemandedCount++;
        }
        if(btn2->bellDemandedThisTick(i))
        {
            bt2BellDemandedCount++;
        }

   }

    //Should be set for two lots of five mins
    EXPECT_EQ(bt1FlagDemandedCount, 13000-BUTTON1_PRESS_TICK - 8);  //not pressed for first 100.  4 ticks between each 4 5 min flag bock
    EXPECT_EQ(bt2FlagDemandedCount, 13000-BUTTON2_PRESS_TICK - 8);

    //get 18 in 650 seconds due to pause at start
    EXPECT_EQ(bt1BellDemandedCount, 18);
    EXPECT_EQ(bt2BellDemandedCount, 18);
}




