/**
 * Rob Harwood-Smith July 2024
 *
  */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "servoCntrl.h"
#include "buttonPressState.h"
#include "bellRingState.h"
#include "main.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;  //On board LED - can be used for crude debug indication or "alive" indication.
const uint FRONT_LED_PIN = 12; //GPIO12 - Pin #16
const uint REAR_LED_PIN = 15; //GPIO15 - Pin #20
const uint FRONT_FLAG_SERVO_PIN = 3; //GPIO3 - pin #5 - PWM Channel 1B
const uint BACK_FLAG_SERVO_PIN = 4; //GPIO4 - pin #6 - PWM Channel 2A
const uint FRONT_SWITCH_PIN = 16; //GPIO16 - Pin #21
const uint REAR_SWITCH_PIN = 17; //GPIO17 - Pin #22
const uint BELL_SERVO_PIN = 11; //GPI11 - pin #15 - PWM Channel 5B

/// @brief initialise LED and Switch pins - Digital IO
void setUpGPIO_Pins()
{
    // Initialise GPIO LEDs as outputs
    gpio_init(LED_PIN); // Onboard LED
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(FRONT_LED_PIN);
    gpio_set_dir(FRONT_LED_PIN, GPIO_OUT);

    gpio_init(REAR_LED_PIN);
    gpio_set_dir(REAR_LED_PIN, GPIO_OUT);

    // set switch inputs
    gpio_init(FRONT_SWITCH_PIN);
    gpio_set_dir(FRONT_SWITCH_PIN, GPIO_IN);
    gpio_pull_up(FRONT_SWITCH_PIN);

    gpio_init(REAR_SWITCH_PIN);
    gpio_set_dir(REAR_SWITCH_PIN, GPIO_IN);
    gpio_pull_up(REAR_SWITCH_PIN);

    // Initialise GPIO Switch inputs on rising edge
    stdio_init_all();
}


int main() {

    
    //Initialise servo controls for flags
    servoCntrl front_flag_servo(FRONT_FLAG_SERVO_PIN);   //initialise servo control on GPIO3
    servoCntrl back_flag_servo(BACK_FLAG_SERVO_PIN);   //initialise servo control on GPIO4

    //Initialise servo controls for bell
    servoCntrl bell_servo(BELL_SERVO_PIN);   //initialise servo control on GPIO1

    setUpGPIO_Pins();
    int count = 0;
    bool heartbeat;
    uint64_t tickCounterFromBoot = 0;  //64 bits gives enough 50ms counts for lifetime without wrapping

    //It is possible to do code via call-backs - but simplified design to do all things on 50ms ticks instead
    //struct repeating_timer timer;
    //add_repeating_timer_ms(500, big_tick_callback, NULL, &timer);

    buttonPressState* frontBellPush = new buttonPressState(FRONT_SWITCH_PIN);
    buttonPressState* backBellPush = new buttonPressState(REAR_SWITCH_PIN);
    bellRingState* bellRingCntrl = new bellRingState();

    const float FLAG_DOWN_ANGLE = 10.0; 
    const float FLAG_UP_ANGLE = 170.0; 
    float frontFlagServoPos = FLAG_DOWN_ANGLE;
    float backFlagServoPos = FLAG_DOWN_ANGLE;


    //Main loop - loop for ever
    while (true)  
    {
        //heartbeat logic on / off for 1.25seconds = 50ms*25
        count ++;
        if(count % 25 == 0)
        {
            heartbeat =!heartbeat;
            count = 0;
        }
        gpio_put(LED_PIN, heartbeat);

        //do logic based on bell push switch demand and current bell state
        frontBellPush->doButtonActionOnTick(tickCounterFromBoot, gpio_get(FRONT_SWITCH_PIN)==0x00);
        backBellPush->doButtonActionOnTick(tickCounterFromBoot, gpio_get(REAR_SWITCH_PIN)==0x00);

        //do bell logic based on bell button demand - demand
        bool frontBellRequestAcceptedThisTick = frontBellPush->bellDemandedThisTick(tickCounterFromBoot);
        bool backBellRequestAcceptedThisTick = backBellPush->bellDemandedThisTick(tickCounterFromBoot);
        bellRingCntrl->doBellActionOnTick(tickCounterFromBoot, 
                                          frontBellRequestAcceptedThisTick,
                                          backBellRequestAcceptedThisTick);

        //Set Flags and LEDs based on bell logic - 5mins flag demand after button press 
        frontFlagServoPos = (frontBellPush->getFlagIsDemanded() ? FLAG_UP_ANGLE : FLAG_DOWN_ANGLE);
        backFlagServoPos = (backBellPush->getFlagIsDemanded() ? FLAG_UP_ANGLE : FLAG_DOWN_ANGLE);

        front_flag_servo.goToDegree(frontFlagServoPos);
        back_flag_servo.goToDegree(backFlagServoPos);

        gpio_put(FRONT_LED_PIN, frontFlagServoPos == FLAG_UP_ANGLE);
        gpio_put(REAR_LED_PIN, backFlagServoPos == FLAG_UP_ANGLE);

       
        bell_servo.goToDegree(bellRingCntrl->getBellServoDemandedPosition());
       
        //aproximate 50ms per tick - time to execute code in loop is minimal
        sleep_ms(50);
        tickCounterFromBoot ++;
    }

}


