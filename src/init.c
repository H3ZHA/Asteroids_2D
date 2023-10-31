#include "init.h"

int gun_cooldown = 0;
int missile_cooldown = 0;
int start_thread = 0;
int countdown_start = 0;

void on_idle()
{
	if (shooting_state == 1 && gun_cooldown == 0 && gun_switch == 0) {
		add_bullet(&gun);
		gun_cooldown = 1;
		glutTimerFunc(shooting_cooldown, timer_gun_cooldown, 1);
	}
	else if (shooting_state == 1 && missile_cooldown == 0 && gun_switch == 1) {
		add_missile(&missile_launcher);
		missile_cooldown = 1;
		glutTimerFunc(shoot_missile_cooldown, timer_missile_cooldown, 1);
	}

	// Detect game start
	if (game_start == 1) {
		// thread
		if (start_thread == 0 && game_stop == 0) {
			glutTimerFunc(1000, timer_asteroid_wave, 1);
			glutTimerFunc(1000, timer_timing, 1);
			start_thread = 1;
		}
		if (game_start == 1 && game_stop == 1 && game_restart == 1) {
			if (countdown <= 0) {
				restart_game();
				countdown_start = 0;
			}
			else {
				if (countdown_start == 0) {
					countdown_start = 1;
					glutTimerFunc(1000, timer_countdown, 1);
				}
			}
		}
		glutPostRedisplay();
	}
}

void timer_gun_cooldown() {
	gun_cooldown = 0;
}

void timer_missile_cooldown() {
	missile_cooldown = 0;
}

void timer_asteroid_wave() {
	if (game_stop == 0) {
		for (int i = 0; i < number_of_asteroid; i++) {
			add_asteroid(&player_data);
		}

		number_of_asteroid++;

		glutTimerFunc(time_between_waves, timer_asteroid_wave, 1);
	}
	else {
		start_thread = 0;
	}
}

void timer_timing() {
	if (game_stop == 0) {
		time_second++;
		if (time_second == 60) {
			time_second = 0;
			time_minute++;
		}

		glutTimerFunc(1000, timer_timing, 1);
	}
	else {
		start_thread = 0;
	}
}

void timer_countdown() {
	if (countdown >= 0 && countdown_start == 1) {
		countdown--;
		glutTimerFunc(1000, timer_countdown, 1);
	}
}

void init(int argc, char** argv) {
	glutInit(&argc, argv);
	// RGB mode with double buffering and a depth buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("A1_s3782493");
	// full screen
	glutFullScreen();

	// get window size
	double client_wideth = glutGet(GLUT_SCREEN_WIDTH);
	double client_height = glutGet(GLUT_SCREEN_HEIGHT);

	// adaptive arena size
	double x = arena_length / 2 + margin;
	double y = arena_height / 2 + margin;
	if (client_wideth > client_height) {
		x = x * (client_wideth / client_height);
	}
	else if (client_wideth < client_height) {
		y = y * (client_height / client_wideth);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-x, x, -y, y, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	// graph and keyboard callbacks 
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(on_mouse_button);
	
	// reset game
	reset_game(&player_data);

	glutIdleFunc(on_idle);
}