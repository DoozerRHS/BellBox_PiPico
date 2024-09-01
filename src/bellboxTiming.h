#ifndef BELL_BOX_TIMING_H
#define BELL_BOX_TIMING_H

/* - constants used for bell box "approximate" timings based on main loop "tick" and multiples of ticks - */

//Period between small ticks in the main loop in MS
#define SMALL_TICK_PERIOD 50

//20 Seconds as count of Small tick counts
#define BUTTON_LATCH_NORMAL_TICKS 400

//40 Seconds as count of Small tick counts
#define BUTTON_LATCH_LONGER_WAIT_TICKS 800

#define FIVE_MINS_AS_TICKS 6000


#endif /* BELL_BOX_TIMING_H */