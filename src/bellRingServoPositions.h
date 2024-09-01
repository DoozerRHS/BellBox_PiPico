#ifndef BELL_RING_SERVO_TIMINGS_H
#define BELL_RING_SERVO_TIMINGS_H

#define UP_DOWN_SEQUENCE_TICKS 8

#define BELL_UP 20.0
#define BELL_DOWN 110.0

const float BELL_BACK_AND_FORTH[UP_DOWN_SEQUENCE_TICKS] 
{
    BELL_UP,
    BELL_UP,
    BELL_UP,
    BELL_UP,
    BELL_DOWN,
    BELL_DOWN,
    BELL_DOWN,
    BELL_DOWN,
};

#define FRONT_REPEAT 3
#define BACK_REPEAT 4

#define FRONT_PAUSE_BEFORE_REPEAT_TICKS 40 //2 seconds
#define BACK_PAUSE_BEFORE_REPEAT_TICKS 60 //3 seconds

#endif /* BELL_RING_SERVO_TIMINGS_H */