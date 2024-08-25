#ifndef SERVO_CNTRL_H
#define SERVO_CNTRL_H

#include "pico/stdlib.h"

/// @brief Class to control servo via pico - translates desired servo position in degrees to appriopriate PWM output on control GPIO pin
class servoCntrl {
public:
	servoCntrl(uint8_t a_gpio);
	virtual ~servoCntrl();

	/// @brief Set PWM to move servo to the desired value
	/// @param a_degree betwen o and 180
	void goToDegree(float a_degree);

private:
	uint8_t _gpio = 0;  //The GPIO pin as set up at construction
};

#endif /* SERVO_CNTRL_H */