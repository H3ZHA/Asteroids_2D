#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "graph.h"

extern void keyboard(unsigned char key, int x, int y);
extern void on_mouse_button(int button, int state, int x, int y);
extern void player_rotate(player_ship* player_ship, int r);
extern void player_move(player_ship* player_ship, int speed);


#endif // !KEYBOARD_H
