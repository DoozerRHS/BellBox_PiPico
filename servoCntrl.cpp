#include "servoCntrl.h"
#include "hardware/pwm.h" 
#include "hardware/clocks.h"
#include <stdio.h>


/// @brief Class to control servo via pico - translates desired servo position in degrees to appriopriate PWM output on control GPIO pin
/// @param a_gpio servo control PWM gpio
servoCntrl::servoCntrl(uint8_t a_gpio) 
{
	_gpio = a_gpio;

	gpio_init(a_gpio);

	//Setup up gpio pin to be in PWM mode
	gpio_set_function(a_gpio, GPIO_FUNC_PWM);
	pwm_set_gpio_level(a_gpio, 0);
	uint slice_num = pwm_gpio_to_slice_num(a_gpio);

	// Get clock speed and compute divider for 50 hz
	uint32_t clk = clock_get_hz(clk_sys);
	uint32_t div = clk / (20000 * 50);

	// Check div is in range
	if ( div < 1 ){
		div = 1;
	}
	if ( div > 255 ){
		div = 255;
	}

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, (float)div);

	// Set wrap for PWM repeat period is 20 ms
	pwm_config_set_wrap(&config, 20000);

	// Load the configuration
	pwm_init(slice_num, &config, false);

	pwm_set_enabled(slice_num, true);
}

/// @brief Destructor
servoCntrl::~servoCntrl() {
	// Nothing to do on destruction
}


/**
 * move to angle: 0 to 180.
 * @param degree
 */
void servoCntrl::goToDegree(float a_degree){
    #define ROTATE_0 700 //PWM micro secs to rotate to 0° - may need to be contructor params - servos differ.
    #define ROTATE_180 2300  //PWM micro secs to rotate to 180° 

	if (a_degree > 180.0){
		a_degree = 180.0;
	}
	if (a_degree < 0){
		a_degree = 0.0;
	}

	int duty = (((float)(ROTATE_180 - ROTATE_0) / 180.0) * a_degree) + ROTATE_0;

	pwm_set_gpio_level(_gpio, duty);

}