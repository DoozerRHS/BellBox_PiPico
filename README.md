# BellBox_PiPico
Bellbox logic for pi pico microcontroller - two bells, two servo flags, one servo bell, 2 lights.

Code developed with free tools, VSCode used.

Getting started editing on Windows:  See https://www.raspberrypi.com/news/raspberry-pi-pico-windows-installer/ 
To Debug, see Debug with a second Pico in https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf 
Getting started including loading code onto board: https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html


Pinout used:
GP11  Front LED (Pin 15) Yellow			5v Power VBUS (Pin 40)
GP 12 Back LED (Pin 16) Green			Gnd (Pin 23)
Front Flag Servo GP 3 (Pin 5)
Rear Flag Servo GP 4 (Pin 6)
Bell Servo GP 2(Pin 4)
Front switch GP16 (Pin 21) and Gnd (Pin 23) Yellow/Blk
Back switch GP17 (Pin 22) and Gnd (Pin 23) Green/Black

Switches with 1kΩ resistor in series
LEDs with 220Ω resitor in series

1.	Detect Button, latch Button Pressed.  After N seconds release latch (allow next press)
2.	Button pushed corresponding LED lights for period (5 mins)
3.	Front button pressed do_front_flag_move, after 5 mins retract flag.
4.	Rear button pressed do_rear_flag_move, after 5 mins retract flag.
5.	When front button latched do bell servo back-forth three times pause 2 secs repeat.
6.	When rear button latched do bell servo back-forth four times pause 4 secs repeat.
7.	If other button pressed when ringing, has no effect on ringing (just flags and LEDs)
8.	If rung bell don’t ring again for Latch period.  Period is 20 seconds, then 40, then 60, etc.  After 5 mins reset to 20 seconds - resist impatient pushers!

