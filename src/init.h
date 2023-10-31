#ifndef INIT_H
#define INIT_H

#include "keyboard.h"

extern void init(int argc, char** argv);
extern void on_idle();
extern void timer_gun_cooldown();
extern void timer_asteroid_wave();
extern void timer_missile_cooldown();
extern void timer_timing();
extern void timer_countdown();


#endif // !INIT_H