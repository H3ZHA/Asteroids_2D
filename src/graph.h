#ifndef GRAPH_H
#define GRAPH_H

#include "constant.h"

extern player_ship player_data;
extern player_ship gun;
extern player_ship missile_launcher;
extern int shooting_state;
extern int number_of_asteroid;
extern int gun_switch;
extern int time_second;
extern int time_minute;
extern int game_start;
extern int game_stop;
extern int game_restart;
extern int countdown;
extern int keyboard_lock;

extern void display();
extern void arena();
extern void player(player_ship* player_ship);
extern void init_player(player_ship* player_ship);
extern void reset_game(player_ship* player_ship);

//related to bullets
extern void add_bullet(player_ship* player_ship);
extern void draw_bullet(bullet* bullets);
extern int bullet_exist(bullet* bullet);
extern void shoot_bullet(player_ship* player_ship, bullet* bullet);
extern void add_missile(player_ship* missile_launcher);
extern void draw_missile(bullet* missile);

// related to asteroid
extern float asteroid_generate_radius();
extern float generate_random_angle();
extern void add_asteroid(player_ship* player_ship);
extern int asteroid_exist(asteroid* asteroid);
extern void generate_asteroid(player_ship* player_ship, asteroid* asteroid);
extern void draw_asteroid(asteroid* asteroid);
extern void detection_into_arena(asteroid* asteroid);
extern void asteroid_split(asteroid* asteroid);
extern void split_new_asteroid(asteroid* asteroid, float x, float y, float angle, 
	float rotation_angle, int speed, float size, int rotation_speed, int rotation_direction);

extern int particle_exist(particle* particle);
extern void explosion_particle_effect(particle* particle, float x, float y, float angle, int speed, int time_pass);
extern void draw_particle(particle* particle);

// related to gun on ship
extern void draw_gun(player_ship* gun, player_ship* ship);
extern void draw_missile_launcher(player_ship* missile_launcher, player_ship* ship);
extern void missile_graph(player_ship* missile_launcher);

// related to calculate
extern float included_angle(float x_1, float y_1, float x_2, float y_2);
extern float calculate_distence(float x_1, float y_1, float x_2, float y_2);
extern void collision_detection_ship(player_ship* player_ship, asteroid* asteroid);
extern void collision_detection_bullet(bullet* bullet, asteroid* asteroid);
extern float calculate_distence_missile_asteroid(bullet* missile, asteroid* asteroid);
extern float included_angle_missile_asteroid(bullet* missile, asteroid* asteroid);
extern void collision_detection_missile(bullet* missile, asteroid* asteroid);
extern void collision_detection_arena(asteroid* asteroid);
extern void collision_detection_asteroid(asteroid* asteroid_1, asteroid* asteroid_2);

// related to score
extern void draw_score();
extern void draw_title();
extern void draw_restart();
extern void restart_game();
extern void draw_countdown();
extern void keyboard_unlock();

#endif // !GRAPH_H
