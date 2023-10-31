#include "keyboard.h"

// Keyboard callback 
void keyboard(unsigned char key, int x, int y) {
    if (game_start == 0) {
        if (key == KEY_ESC) {
            exit(EXIT_SUCCESS);
        }
        game_start = 1;
    }
    else {
        if (game_stop == 1) {
            if (key == KEY_ESC) {
                exit(EXIT_SUCCESS);
            }
            if (keyboard_lock == 0) {
                game_restart = 1;
            }
        }
        else {
            switch (key) {
            case KEY_ESC:
                exit(EXIT_SUCCESS);
                break;
            case 'w':
            case 'W':
                player_move(&player_data, move_speed);
                break;
            case 'a':
            case 'A':
                player_rotate(&player_data, -rotate_speed);
                break;
            case 'd':
            case 'D':
                player_rotate(&player_data, rotate_speed);
                break;
            case 'z':
            case 'Z':
                if (gun_switch == 0) {
                    gun_switch = 1;
                }
                else if (gun_switch == 1) {
                    gun_switch = 0;
                }
                break;
            default:
                fprintf(stderr, "on_key_press(%c)\n", key);
                break;
            }
        }
    }
}

// mouse callback 
void on_mouse_button(int button, int state, int x, int y) {
    if (game_start == 1 && game_stop == 0) {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                shooting_state = 1;
            }
            else if (state == GLUT_UP) {
                shooting_state = 0;
            }
        }
    }
}

void player_rotate(player_ship* player_ship, int r) {
    player_ship->angle = player_ship->angle + r;
}

void player_move(player_ship* player_ship, int speed) {
    float pi = 4.0 * atan(1);
    player_ship->x = player_ship->x + speed * sin(player_ship->angle / 180 * pi);
    player_ship->y = player_ship->y + speed * cos(player_ship->angle / 180 * pi);
}