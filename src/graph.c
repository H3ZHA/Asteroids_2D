#include"graph.h"

player_ship player_data;
player_ship gun;
player_ship missile_launcher;
int close_up = 0;
int close_down = 0;
int close_left = 0;
int close_right = 0;
int shooting_state = 0;
bullet bullets[max_bullet_number];
asteroid asteroids[max_asteroid_number];
int game_over = 0;
int number_of_asteroid = 1;
int gun_switch = 0;
bullet missiles[max_bullet_number];
int asteroid_destryoed = 0;
int time_second = 0;
int time_minute = 0;
int game_start = 0;
int game_stop = 0;
int game_restart = 0;
int countdown = time_between_waves / 1000 + 1;
int keyboard_lock = 0;
particle particles[max_asteroid_number * 7];


/* Display callback */
void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	if (game_start == 0) {
		draw_title();
	}

	if (game_over == 1 && game_restart == 0) {
		draw_restart();
	}

	if (game_over == 1 && game_restart == 1) {
		draw_countdown();
	}

	arena();

	// draw below only game not over
	if (game_over == 0) {
		// draw bullets
		for (int i = 0; i < max_bullet_number; i++) {
			// only draw valid pointer
			if (bullet_exist(&bullets[i]) == 1) {
				draw_bullet(&bullets[i]);
			}
		}

		// draw player and gun
		player(&player_data);
		if (gun_switch == 0) {
			draw_gun(&gun, &player_data);
		}
		else if (gun_switch == 1) {
			draw_missile_launcher(&missile_launcher, &player_data);
		}

		// draw missiles
		for (int i = 0; i < max_bullet_number; i++) {
			// only draw valid pointer
			if (bullet_exist(&missiles[i]) == 1) {
				draw_missile(&missiles[i]);
			}
		}
	}
	else {
		memset(bullets, 0, sizeof(bullets));
		memset(missiles, 0, sizeof(missiles));
	}

	// draw asteroids
	for (int i = 0; i < max_asteroid_number; i++) {
		// only draw valid pointer
		if (asteroid_exist(&asteroids[i]) == 1) {
			draw_asteroid(&asteroids[i]);
		}
	}

	// draw explosion particle
	for (int i = 0; i < max_asteroid_number*7; i++) {
		// only draw valid pointer
		if (particle_exist(&particles[i]) != 0) {
			draw_particle(&particles[i]);
		}
	}

	// draw score
	draw_score();

	int err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("display: %s\n", gluErrorString(err));
	glutSwapBuffers();
}

void arena() {
	// draw 4 separate lines as arena, z coordinates is 0
	int x = arena_length / 2;
	int y = arena_height / 2;

	// draw up, down, left and right successively
	// Detection distance
	if (close_up == 0) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 0, 0);
	}
	glBegin(GL_POLYGON);
	glVertex3f(-x-border, y, 0);
	glVertex3f(x+border, y, 0);
	glVertex3f(x+border, y+border, 0);
	glVertex3f(-x-border, y+border, 0);
	glEnd();

	if (close_down == 0) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 0, 0);
	}
	glBegin(GL_POLYGON);
	glVertex3f(-x-border, -y-border, 0);
	glVertex3f(x+border, -y-border, 0);
	glVertex3f(x+border, -y, 0);
	glVertex3f(-x-border, -y, 0);
	glEnd();

	if (close_left == 0) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 0, 0);
	}
	glBegin(GL_POLYGON);
	glVertex3f(-x-border, -y-border, 0);
	glVertex3f(-x, -y-border, 0);
	glVertex3f(-x, y+border, 0);
	glVertex3f(-x-border, y+border, 0);
	glEnd();
	
	if (close_right == 0) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 0, 0);
	}
	glBegin(GL_POLYGON);
	glVertex3f(x+border, -y-border, 0);
	glVertex3f(x, -y-border, 0);
	glVertex3f(x, y+border, 0);
	glVertex3f(x+border, y+border, 0);
	glEnd();
}

void player(player_ship* player_ship) {
	float r = player_ship->r;
	float g = player_ship->g;
	float b = player_ship->b;
	float x = player_ship->x;
	float y = player_ship->y;
	float angle = player_ship->angle;

	glPushMatrix();

	// set color
	glColor3f(r, g, b);

	// movement and rotation
	glTranslatef(x, y, 0);
	glRotatef(-angle, 0, 0, 1);
	
	
	// draw player
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0+player_edge/2, 0);
	glVertex3f(0, 0-player_edge/2, 0);
	glVertex3f(0+player_span, 0-(player_edge/2)-player_extra_edge, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0+player_edge/2, 0);
	glVertex3f(0, 0-player_edge/2, 0);
	glVertex3f(0-player_span, 0-(player_edge/2)-player_extra_edge, 0);
	glEnd();

	// Detection distance
	float x_min = -arena_length / 2;
	float x_max = arena_length / 2;
	float y_min = -arena_height / 2;
	float y_max = arena_height / 2;

	if (x > (x_max - boundary_warning_distance)) {
		close_right = 1;
	}
	else {
		close_right = 0;
	}

	if (x < (x_min + boundary_warning_distance)) {
		close_left = 1;
	}
	else {
		close_left = 0;
	}

	if (y > (y_max - boundary_warning_distance)) {
		close_up = 1;
	}
	else {
		close_up = 0;
	}

	if (y < (y_min + boundary_warning_distance)) {
		close_down = 1;
	}
	else {
		close_down = 0;
	}

	if (x + player_edge/2 > x_max || x - player_edge/2 < x_min || 
				y + player_edge/2 > y_max || y - player_edge/2 < y_min) {
		game_over = 1;
		game_stop = 1;
		keyboard_lock = 1;
		glutTimerFunc(500, keyboard_unlock, 1);
	}

	glPopMatrix();
}

void init_player(player_ship* player_ship) {
	player_ship->x = 0;
	player_ship->y = 0;
	player_ship->r = 1;
	player_ship->g = 1;
	player_ship->b = 1;
	player_ship->angle = 0;
}

void reset_game(player_ship* player_ship) {
	init_player(player_ship);
	close_up = 0;
	close_down = 0;
	close_left = 0;
	close_right = 0;
	shooting_state = 0;
	number_of_asteroid = 1;
	gun_switch = 0;
	asteroid_destryoed = 0;
	time_second = 0;
	time_minute = 0;

	// reset bullet and asteroid
	memset(bullets, 0, sizeof(bullets));
	memset(asteroids, 0, sizeof(asteroids));
	memset(missiles, 0, sizeof(missiles));
}



/*--------------------------related to bullets--------------------------*/
void add_bullet(player_ship* gun) {
	for (int i = 0; i < max_bullet_number; i++) {
		// only write invalid pointer
		if (bullet_exist(&bullets[i]) == 0) {
			shoot_bullet(gun, &bullets[i]);
			break;
		}
	}
}

// Check bullet pointer is valid
int bullet_exist(bullet* bullet) {
	if (bullet->exist == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

void shoot_bullet(player_ship* gun, bullet* bullet) {
	bullet->exist = 1;
	bullet->angle = gun->angle;
	bullet->x = gun->x;
	bullet->y = gun->y;
}

void draw_bullet(bullet* bullet) {
	// Calculate the location
	float pi = 4.0 * atan(1);
	bullet->x = bullet->x + bullet_speed * sin(bullet->angle / 180 * pi);
	bullet->y = bullet->y + bullet_speed * cos(bullet->angle / 180 * pi);

	// Detect Boundaries
	float x_min = -arena_length / 2;
	float x_max = arena_length / 2;
	float y_min = -arena_height / 2;
	float y_max = arena_height / 2;

	if (bullet->x > x_max || bullet->x < x_min || bullet->y > y_max || bullet->y < y_min) {
		bullet->exist = 0;
	}

	// darw bullet
	if (bullet->exist == 1) {
		glPushMatrix();

		glColor3f(1, 1, 1);

		// movement and rotation
		glTranslatef(bullet->x, bullet->y, 0);
		glRotatef(-bullet->angle, 0, 0, 1);

		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		float r = bullet_radius;
		glVertex3f(0, -r, 0);
		glVertex3f(r, 0, 0);
		glVertex3f(0, r, 0);
		glVertex3f(-r, 0, 0);
		glEnd();

		glPopMatrix();
	}

}

void add_missile(player_ship* missile_launcher) {
	for (int i = 0; i < max_bullet_number; i++) {
		// only write invalid pointer
		if (bullet_exist(&missiles[i]) == 0) {
			shoot_bullet(missile_launcher, &missiles[i]);
			break;
		}
	}
}

void draw_missile(bullet* missile) {
	// lock the nearest asteroid
	int asteroid_number = -1;
	float distance_between = -1;

	for (int i = 0; i < max_asteroid_number; i++) {
		if (asteroid_exist(&asteroids[i]) == 1) {
			float distance = calculate_distence_missile_asteroid(missile, &asteroids[i]);
			if (distance_between == -1) {
				distance_between = distance;
				asteroid_number = i;
			}
			else {
				if (distance < distance_between) {
					distance_between = distance;
					asteroid_number = i;
				}
			}
		}
	}
	// Correction angle
	if (asteroid_number != -1) {
		missile->angle = included_angle_missile_asteroid(missile, &asteroids[asteroid_number]);
	}

	// Calculate the location
	float pi = 4.0 * atan(1);
	missile->x = missile->x + missile_speed * sin(missile->angle / 180 * pi);
	missile->y = missile->y + missile_speed * cos(missile->angle / 180 * pi);

	// Detect Boundaries
	float x_min = -arena_length / 2;
	float x_max = arena_length / 2;
	float y_min = -arena_height / 2;
	float y_max = arena_height / 2;

	int launcher_length = player_span / 3;
	int launcher_width = player_span / 2;
	int launcher_location = player_span * 1.5;
	float radius = launcher_length + launcher_location;

	if (missile->x + radius > x_max || missile->x - radius < x_min || 
			missile->y + radius > y_max || missile->y - radius < y_min) {
		missile->exist = 0;
	}

	// darw missile
	if (missile->exist == 1) {
		glPushMatrix();

		// movement and rotation
		glTranslatef(missile->x, missile->y, 0);
		glRotatef(-missile->angle, 0, 0, 1);

		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex3f(-launcher_width, -launcher_length - launcher_location, 0);
		glVertex3f(launcher_width, -launcher_length - launcher_location, 0);
		glVertex3f(launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
		glVertex3f(0, player_edge / 2 + launcher_length, 0);
		glVertex3f(-launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
		glEnd();

		glColor3f(1, 1, 1);
		glLineWidth(1);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-launcher_width, -launcher_length - launcher_location, 0);
		glVertex3f(launcher_width, -launcher_length - launcher_location, 0);
		glVertex3f(launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
		glVertex3f(0, player_edge / 2 + launcher_length, 0);
		glVertex3f(-launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
		glEnd();

		glPopMatrix();
	}
}



/*--------------------------related to asteroid--------------------------*/
float asteroid_generate_radius() {
	return sqrt(pow(arena_length, 2) + pow(arena_height, 2)) / 2 + asteroid_max_radius;
}

float generate_random_angle() {
	return rand() % 360;
}

void add_asteroid(player_ship* player_ship) {
	for (int i = 0; i < max_asteroid_number; i++) {
		// only write invalid pointer
		if (asteroid_exist(&asteroids[i]) == 0) {
			generate_asteroid(player_ship, &asteroids[i]);
			break;
		}
	}
}

// Check asteroid pointer is valid
int asteroid_exist(asteroid* asteroid) {
	if (asteroid->exist == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

void generate_asteroid(player_ship* player_ship, asteroid* asteroid) {
	asteroid->exist = 1;
	asteroid->into_arena = 0;
	asteroid->can_split = 1;

	// generation location
	float pi = 4.0 * atan(1);
	float angle = generate_random_angle();
	float x = asteroid_generate_radius() * sin(angle / 180 * pi);
	float y = asteroid_generate_radius() * cos(angle / 180 * pi);

	// angle toward player
	float angle_base_on_player = included_angle(x, y, player_ship->x, player_ship->y);
	
	asteroid->angle = angle_base_on_player;
	asteroid->x = x;
	asteroid->y = y;
	asteroid->rotation_angle = 0;

	// set speed
	int speed = rand() % (asteroid_max_speed - asteroid_min_speed) + asteroid_min_speed;
	asteroid->speed = speed;

	// set size and hp
	float size = rand() % (asteroid_max_radius - asteroid_min_radius) + asteroid_min_radius;
	asteroid->size = size;
	asteroid->hp = size;

	// set surface bumps
	// random surface bumps, max bump is one third radius
	for (int i = 0; i < asteroid_roundness; i++) {
		asteroid->bumps[i] = rand() % (int)(size / 3);
	}

	// set rotation
	int rotation = rand() % 2;
	if (rotation == 1) {
		asteroid->rotation_direction = 1;
	}
	else {
		asteroid->rotation_direction = -1;
	}
	int rotation_speed = rand() % (asteroid_rotation_max_speed - asteroid_rotation_min_speed) 
										+ asteroid_rotation_min_speed;
	asteroid->rotation_speed = rotation_speed;
}

void draw_asteroid(asteroid* asteroid) {
	// Calculate the location
	float pi = 4.0 * atan(1);
	asteroid->x = asteroid->x + asteroid->speed * sin(asteroid->angle / 180 * pi);
	asteroid->y = asteroid->y + asteroid->speed * cos(asteroid->angle / 180 * pi);

	// rotation
	asteroid->rotation_angle = asteroid->rotation_angle + asteroid->rotation_direction * asteroid->rotation_speed;
	
	// darw asteroid
	glPushMatrix();

	float hp_percentage = asteroid->hp / asteroid->size;
	glColor3f(1, hp_percentage, hp_percentage);

	// movement and rotation
	glTranslatef(asteroid->x, asteroid->y, 0);
	glRotatef(asteroid->rotation_angle, 0, 0, 1);

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	float x, y;
	float r = asteroid->size;
	int n = asteroid_roundness;
	for (int i = 0; i < n; i++) {
		x = (i / (float)n - 0.5) * 2.0 * r;
		y = sqrt(r * r - x * x) - asteroid->bumps[i];
		glVertex2f(x, y);
	}
	for (int i = n; i > 0; i--) {
		x = (i / (float)n - 0.5) * 2.0 * r;
		y = -sqrt(r * r - x * x) + asteroid->bumps[i - 1];
		glVertex2f(x, y);
	}
	glEnd();

	glPopMatrix();


	// collision_detection
	for (int i = 0; i < max_bullet_number; i++) {
		// only valid pointer
		if (bullet_exist(&bullets[i]) == 1) {
			collision_detection_bullet(&bullets[i], asteroid);
		}
	}

	for (int i = 0; i < max_bullet_number; i++) {
		// only valid pointer
		if (bullet_exist(&missiles[i]) == 1) {
			collision_detection_missile(&missiles[i], asteroid);
		}
	}

	// detect asteroid exist
	if (asteroid->hp <= 0) {
		asteroid_split(asteroid);
		asteroid_destryoed++;
	}

	// detect only when asteroid exist
	if (asteroid->exist == 1) {
		collision_detection_ship(&player_data, asteroid);

		// detection board
		if (asteroid->into_arena == 0) {
			detection_into_arena(asteroid);
		}
		else {
			collision_detection_arena(asteroid);
			// detection other asteroid
			for (int i = 0; i < max_asteroid_number; i++) {
				// only valid pointer
				if (asteroid_exist(&asteroids[i]) == 1 && &asteroids[i] != asteroid) {
					collision_detection_asteroid(asteroid, &asteroids[i]);
				}
			}
		}
	}
}

void detection_into_arena(asteroid* asteroid) {
	float x = asteroid->x;
	float y = asteroid->y;
	float size = asteroid->size;
	if (x-size > -arena_length/2 && x+size < arena_length/2 
			&& y-size > -arena_height/2 && y+size < arena_height/2) {
		asteroid->into_arena = 1;
	}
}

void asteroid_split(asteroid* asteroid) {
	asteroid->exist = 0;
	float x = asteroid->x;
	float y = asteroid->y;

	// Number of explosion particle related to size of asteroid (minimum 3, maximum 7)
	float number_of_explosion_particle = asteroid->size / 5;
	if (number_of_explosion_particle < 3) {
		number_of_explosion_particle = 3;
	}
	else if (number_of_explosion_particle > 7) {
		number_of_explosion_particle = 7;
	}

	// add explosion particle
	for (int i = 0; i < number_of_explosion_particle; i++) {
		int speed = rand() % (particle_max_speed - particle_min_speed) + particle_min_speed;
		float angle = generate_random_angle();

		for (int i = 0; i < max_bullet_number; i++) {
			// only write invalid pointer
			if (particle_exist(&particles[i]) == 0) {
				explosion_particle_effect(&particles[i], x, y, angle, speed, particle_exist_time);
				break;
			}
		}
	}

	// Asteroids can only be split once
	if (asteroid->can_split == 1) {
		float angle_1 = asteroid->angle - 45;
		float angle_2 = asteroid->angle + 45;
		float size = asteroid->size / 2;

		// set first asteroid
		for (int i = 0; i < max_asteroid_number; i++) {
			// only write invalid pointer
			if (asteroid_exist(&asteroids[i]) == 0) {
				split_new_asteroid(&asteroids[i], asteroid->x, asteroid->y, angle_1,
					asteroid->rotation_angle, asteroid->speed, size, asteroid->rotation_speed, 1);
				break;
			}
		}

		// set second asteroid
		for (int i = 0; i < max_asteroid_number; i++) {
			// only write invalid pointer
			if (asteroid_exist(&asteroids[i]) == 0) {
				split_new_asteroid(&asteroids[i], asteroid->x, asteroid->y, angle_2,
					asteroid->rotation_angle, asteroid->speed, size, asteroid->rotation_speed, -1);
				break;
			}
		}
	}
}

void split_new_asteroid(asteroid* asteroid, float x, float y, float angle, float rotation_angle, int speed,
	float size, int rotation_speed, int rotation_direction) {
	asteroid->exist = 1;
	asteroid->into_arena = 0;
	asteroid->can_split = 0;

	// Calculate the location
	float pi = 4.0 * atan(1);
	asteroid->x = x + size * sin(angle / 180 * pi) * 2;
	asteroid->y = y + size * cos(angle / 180 * pi) * 2;

	asteroid->angle = angle;
	asteroid->rotation_angle = rotation_angle;
	asteroid->speed = speed;
	asteroid->size = size;
	asteroid->hp = size;
	asteroid->rotation_direction = rotation_direction;
	asteroid->rotation_speed = rotation_speed;

	// set surface bumps
	// random surface bumps, max bump is one third radius
	for (int i = 0; i < asteroid_roundness; i++) {
		asteroid->bumps[i] = rand() % (int)(size / 3);
	}
}

int particle_exist(particle* particle) {
	if (particle->time_pass == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void explosion_particle_effect(particle* particle, float x, float y, float angle, int speed, int time_pass) {
	particle->x = x;
	particle->y = y;
	particle->angle = angle;
	particle->speed = speed;
	particle->time_pass = time_pass;
}

void draw_particle(particle* particle){
	// Calculate the location
	float pi = 4.0 * atan(1);
	particle->x = particle->x + particle->speed * sin(particle->angle / 180 * pi);
	particle->y = particle->y + particle->speed * cos(particle->angle / 180 * pi);

	glPushMatrix();

	float color = (float)particle->time_pass / (float)particle_exist_time;
	glColor3f(color, color, color);

	// movement and rotation
	glTranslatef(particle->x, particle->y, 0);
	glRotatef(-particle->angle, 0, 0, 1);

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	float r = 3;
	glVertex3f(0, -r, 0);
	glVertex3f(r, 0, 0);
	glVertex3f(0, r, 0);
	glVertex3f(-r, 0, 0);
	glEnd();

	glPopMatrix();

	particle->time_pass--;
}



/*--------------------------related to gun on ship--------------------------*/
void draw_gun(player_ship* gun, player_ship* ship) {
	gun->x = ship->x;
	gun->y = ship->y;
	gun->angle = ship->angle;

	glPushMatrix();

	// movement and rotation
	glTranslatef(gun->x, gun->y, 0);
	glRotatef(-gun->angle, 0, 0, 1);

	int gun_length = player_span / 2;
	int gun_width = player_span / 4;
	int gun_location = player_span / 3;

	glColor3f(0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex3f(-gun_width, -gun_length - gun_location, 0);
	glVertex3f(gun_width, -gun_length - gun_location, 0);
	glVertex3f(gun_width, player_edge / 2 + gun_length - gun_location, 0);
	glVertex3f(-gun_width, player_edge / 2 + gun_length - gun_location, 0);
	glEnd();

	glColor3f(1, 1, 1);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-gun_width, -gun_length - gun_location, 0);
	glVertex3f(gun_width, -gun_length - gun_location, 0);
	glVertex3f(gun_width, player_edge/2 + gun_length - gun_location, 0);
	glVertex3f(-gun_width, player_edge/2 + gun_length - gun_location, 0);
	glEnd();

	glPopMatrix();
}

void draw_missile_launcher(player_ship* missile_launcher, player_ship* ship) {
	missile_launcher->x = ship->x;
	missile_launcher->y = ship->y;
	missile_launcher->angle = ship->angle;

	missile_graph(missile_launcher);
}

void missile_graph(player_ship* missile_launcher) {
	glPushMatrix();

	// movement and rotation
	glTranslatef(missile_launcher->x, missile_launcher->y, 0);
	glRotatef(-missile_launcher->angle, 0, 0, 1);

	int launcher_length = player_span / 3;
	int launcher_width = player_span / 2;
	int launcher_location = player_span * 1.5;

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-launcher_width, -launcher_length - launcher_location, 0);
	glVertex3f(launcher_width, -launcher_length - launcher_location, 0);
	glVertex3f(launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
	glVertex3f(0, player_edge / 2 + launcher_length, 0);
	glVertex3f(-launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
	glEnd();

	glColor3f(1, 1, 1);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-launcher_width, -launcher_length - launcher_location, 0);
	glVertex3f(launcher_width, -launcher_length - launcher_location, 0);
	glVertex3f(launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
	glVertex3f(0, player_edge / 2 + launcher_length, 0);
	glVertex3f(-launcher_width, player_edge / 2 + launcher_length - launcher_location, 0);
	glEnd();

	glPopMatrix();
}



/*--------------------------related to calculate--------------------------*/
float included_angle(float x_1, float y_1, float x_2, float y_2) {
	float angle;
	float pi = 4.0 * atan(1);

	// the angle is from x_1, y_1 toward x_2, y_2 (target is x_2, y_2)
	// first quadrant
	if (x_1 >= x_2 && y_1 >= y_2) {
		float new_x = x_1 - x_2;
		float new_y = y_1 - y_2;
		angle = atan(new_x / new_y) * 180 / pi + 180;
	}
	// second quadrant
	else if (x_1 <= x_2 && y_1 >= y_2) {
		float new_x = x_2 - x_1;
		float new_y = y_1 - y_2;
		angle = 180 - atan(new_x / new_y) * 180 / pi;
	}
	// third quadrant
	else if (x_1 <= x_2 && y_1 <= y_2) {
		float new_x = x_2 - x_1;
		float new_y = y_2 - y_1;
		angle = atan(new_x / new_y) * 180 / pi;
	}
	// fourth quadrant
	else if (x_1 >= x_2 && y_1 <= y_2) {
		float new_x = x_1 - x_2;
		float new_y = y_2 - y_1;
		angle = 360 - atan(new_x / new_y) * 180 / pi;
	}
	
	return angle;
}

float calculate_distence(float x_1, float y_1, float x_2, float y_2) {
	float x_distance;
	float y_distance;
	if ((x_1 * x_2) < 0) {
		x_distance = abs(x_1) + abs(x_2);
	}
	else {
		x_distance = abs(x_1) - abs(x_2);
	}

	if ((y_1 * y_2) < 0) {
		y_distance = abs(y_1) + abs(y_2);
	}
	else {
		y_distance = abs(y_1) - abs(y_2);
	}

	return sqrt(pow(x_distance, 2) + pow(y_distance, 2));
}

// detect ship
void collision_detection_ship(player_ship* player_ship, asteroid* asteroid) {
	float safe_distance = player_edge / 2 + asteroid->size;

	float distance_between = calculate_distence(player_ship->x, player_ship->y, asteroid->x, asteroid->y);
	if (distance_between <= safe_distance) {
		game_over = 1;
		game_stop = 1;
		keyboard_lock = 1;
		glutTimerFunc(500, keyboard_unlock, 1);
	}
}

// detect bullet
void collision_detection_bullet(bullet* bullet, asteroid* asteroid) {
	float safe_distance = bullet_radius + asteroid->size;

	float distance_between = calculate_distence(bullet->x, bullet->y, asteroid->x, asteroid->y);
	if (distance_between <= safe_distance) {
		bullet->exist = 0;
		asteroid->hp = asteroid->hp - bullet_damage;
	}
}

float calculate_distence_missile_asteroid(bullet* missile, asteroid* asteroid) {
	return calculate_distence(missile->x, missile->y, asteroid->x, asteroid->y);
}

float included_angle_missile_asteroid(bullet* missile, asteroid* asteroid) {
	return included_angle(missile->x, missile->y, asteroid->x, asteroid->y);
}

void collision_detection_missile(bullet* missile, asteroid* asteroid) {
	float radius = player_span / 3 + player_span * 1.5;
	float safe_distance = radius + asteroid->size;

	float distance_between = calculate_distence(missile->x, missile->y, asteroid->x, asteroid->y);
	if (distance_between <= safe_distance) {
		missile->exist = 0;
		asteroid->hp = asteroid->hp - missile_damage;
	}
}

void collision_detection_arena(asteroid* asteroid) {
	float x = asteroid->x;
	float y = asteroid->y;
	float size = asteroid->size;

	// left board
	if (x-size <= -arena_length/2) {
		asteroid->angle = -asteroid->angle;
		asteroid->x++;
	}
	// right board
	else if (x+size >= arena_length/2) {
		asteroid->angle = -asteroid->angle;
		asteroid->x--;
	}
	// top board
	else if (y+size >= arena_height/2) {
		asteroid->angle = 180 - asteroid->angle;
		asteroid->y--;
	}
	// bottom board
	else if (y-size <= -arena_height/2) {
		asteroid->angle = 180 - asteroid->angle;
		asteroid->y++;
	}

}

void collision_detection_asteroid(asteroid* asteroid_1, asteroid* asteroid_2) {
	// only when both asteroid in arena
	if (asteroid_1->into_arena == 1 && asteroid_2->into_arena == 1) {
		float x_1 = asteroid_1->x;
		float y_1 = asteroid_1->y;
		float x_2 = asteroid_2->x;
		float y_2 = asteroid_2->y;
		float size_1 = asteroid_1->size;
		float size_2 = asteroid_2->size;

		// detect collision
		// if left/right intersect
		if ((x_1 - size_1 < x_2 + size_2 && x_1 - size_1 > x_2 - size_2)
			|| (x_1 + size_1 > x_2 - size_2 && x_1 + size_1 < x_2 + size_2)) {
			// if up/down intersect
			if ((y_1 - size_1 < y_2 + size_2 && y_1 - size_1 > y_2 - size_2)
				|| (y_1 + size_1 > y_2 - size_2 && y_1 + size_1 < y_2 + size_2)) {
				float angle = included_angle(x_1, y_1, x_2, y_2);
				asteroid_1->angle = -angle;
				asteroid_2->angle = angle;
			}
		}
	}
}



/*--------------------------related to score--------------------------*/
void draw_score() {
	// change int to string
	int asteroid_destryoed_length = snprintf(NULL, 0, "%d", asteroid_destryoed) + 1;
	int second_length = snprintf(NULL, 0, "%d", time_second) + 1;
	int minute_length = snprintf(NULL, 0, "%d", time_minute) + 1;

	char* asteroid_destryoed_string = malloc(asteroid_destryoed_length);
	char* second_string = malloc(second_length);
	char* minute_string = malloc(minute_length);

	snprintf(asteroid_destryoed_string, asteroid_destryoed_length, "%d", asteroid_destryoed);
	snprintf(second_string, second_length, "%d", time_second);
	snprintf(minute_string, minute_length, "%d", time_minute);

	// asteroid destryoed
	glPushMatrix();
	// location
	float x = -arena_length / 2 + 10;
	float y = arena_height / 2 - 36;

	glTranslatef(x, y, 0);
	glScalef(0.13, 0.2, 0);
	
	glColor3f(1, 1, 1);
	glLineWidth(2);
	
	for (int i = 0; i < asteroid_destryoed_length; i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, asteroid_destryoed_string[i]);
	}
	
	glPopMatrix();

	// time
	glPushMatrix();
	// location
	x = arena_length / 2 - 90;

	glTranslatef(x, y, 0);
	glScalef(0.13, 0.2, 0);

	glColor3f(1, 1, 1);
	glLineWidth(2);

	// minute
	if (time_minute < 100) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0');
	}
	if (time_minute < 10) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0');
	}
	for (int i = 0; i < asteroid_destryoed_length; i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, minute_string[i]);
	}

	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, ':');

	// second
	if (time_second < 10) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0');
	}
	for (int i = 0; i < asteroid_destryoed_length; i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, second_string[i]);
	}

	glPopMatrix();
}

void draw_title() {
	glPushMatrix();
	// location
	float x = -arena_length / 4;
	float y = arena_height / 4;

	glTranslatef(x, y, 0);
	glScalef(0.16, 0.2, 0);
	
	glColor3f(1, 1, 1);
	glLineWidth(2);
	char title[] = "Press any key to start...";

	for (int i = 0; i < sizeof(title); i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, title[i]);
	}

	glPopMatrix();
}

void draw_restart() {
	glPushMatrix();
	// location
	float x = -arena_length / 2.35;
	float y = arena_height / 4;

	glTranslatef(x, y, 0);
	glScalef(0.16, 0.2, 0);

	glColor3f(1, 1, 1);
	glLineWidth(2);
	char title[] = "Game Over. Press any key to play again...";

	for (int i = 0; i < sizeof(title); i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, title[i]);
	}

	glPopMatrix();
}

void restart_game() {
	reset_game(&player_data);
	game_over = 0;
	game_restart = 0;
	game_stop = 0;
	countdown = time_between_waves / 1000 + 1;
}

void draw_countdown() {
	glPushMatrix();
	// location
	float x = -arena_length / 4.35;
	float y = arena_height / 4;

	glTranslatef(x, y, 0);
	glScalef(0.16, 0.2, 0);

	glColor3f(1, 1, 1);
	glLineWidth(2);
	char title[] = "Game will restart in";

	for (int i = 0; i < sizeof(title); i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, title[i]);
	}
	glPopMatrix();

	glPushMatrix();
	// change int to string
	int countdown_length = snprintf(NULL, 0, "%d", countdown) + 1;
	char* countdown_string = malloc(countdown_length);
	snprintf(countdown_string, countdown_length, "%d", countdown);

	glTranslatef(-50, 0, 0);
	glColor3f(1, 1, 1);
	glLineWidth(10);

	for (int i = 0; i < sizeof(countdown_string); i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, countdown_string[i]);
	}
	
	glPopMatrix();
}

void keyboard_unlock() {
	keyboard_lock = 0;
}