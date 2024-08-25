/**
 * Rob Harwood-Smith July 2024
 *
  */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "servoCntrl.h"

bool volatile front_switch_is_latched = false;
bool volatile rear_led_on = true;
bool volatile front_led_on = true;
uint volatile bigTickCount = 0;



/*
void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
   front_switch_is_latched = true;
   
   
}
*/

/*! fn called on every big tick (every 100ms approx)
 * \brief Logic to start / stop functions
 * \param t details of the timer
 * \return true to keep ticking
 */
bool big_tick_callback(struct repeating_timer *t) {
    //printf("Repeat at %lld\n", time_us_64());
    rear_led_on = !rear_led_on;
    bigTickCount++;
    if(bigTickCount > 20)
    {
        front_led_on = ! front_led_on;
        front_switch_is_latched = false;
        bigTickCount = 0;
    }
    return true;
}


int main() {

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;  //On board LED - can be used for crude debug indication or "alive" indication.
    const uint FRONT_LED_PIN = 11; //GPIO11 - Pin #15

    const uint REAR_LED_PIN = 12; //GPIO12 - Pin #16

    const uint FRONT_SWITCH_PIN = 16; //GPIO16 - Pin #21
    const uint REAR_SWITCH_PIN = 17; //GPIO17 - Pin #23
    const uint FRONT_BUTTON_LATCHED_PIN = 3; //GPIO3 - pin 5
  
    servoCntrl front_flag_servo(4);   //initialise servo control on GPIO4

    //Initialise GPIO LEDs as outputs
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(FRONT_LED_PIN);
    gpio_set_dir(FRONT_LED_PIN, GPIO_OUT);

    gpio_init(REAR_LED_PIN);
    gpio_set_dir(REAR_LED_PIN, GPIO_OUT);

    gpio_init(FRONT_BUTTON_LATCHED_PIN);
    gpio_set_dir(FRONT_BUTTON_LATCHED_PIN, GPIO_OUT);

    gpio_init(FRONT_SWITCH_PIN);
    gpio_set_dir(FRONT_SWITCH_PIN, GPIO_IN);
    gpio_pull_up(FRONT_SWITCH_PIN);


    gpio_init(REAR_SWITCH_PIN);
    gpio_set_dir(REAR_SWITCH_PIN, GPIO_IN);
    gpio_pull_up(REAR_SWITCH_PIN);


  //  gpio_set_irq_enabled_with_callback(FRONT_SWITCH_PIN, /*GPIO_IRQ_LEVEL_LOW |*/ GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    //Initialise GPIO Switch inputs on rising edge
    stdio_init_all();

 //   absolute_time_t currentTime = get_absolute_time();
 //   absolute_time_t nextBlinkTime = make_timeout_time_ms(1000);
 //   bool rearLED = true;
    int count = 0;
    bool heartbeat;
    struct repeating_timer timer;
    add_repeating_timer_ms(500, big_tick_callback, NULL, &timer);
    while (true)  {

        count ++;
        if(count % 25 == 0)
        {
            heartbeat =!heartbeat;
            count = 0;
        }
        
        gpio_put(LED_PIN, heartbeat);
        gpio_put(FRONT_LED_PIN, front_led_on);
        gpio_put(REAR_LED_PIN, rear_led_on);
        gpio_put(FRONT_BUTTON_LATCHED_PIN, front_switch_is_latched);
        if(gpio_get(FRONT_SWITCH_PIN)==0x00)
        {
             front_switch_is_latched = true;
        }

        front_flag_servo.goToDegree((float)count*5);
        sleep_ms(50);
    }

}
