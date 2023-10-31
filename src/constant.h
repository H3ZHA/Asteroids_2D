#ifndef CONSTANT_H
#define CONSTANT_H

#include "libs.h"
#include <math.h>


// keybord reader
#define KEY_ESC 27

// Arena parameter
#define arena_length 800
#define arena_height 1000
#define margin 70
#define border 10

// player parameter
#define player_edge 28
#define player_extra_edge 8
#define player_span 8

#define rotate_speed 8
#define move_speed 8

#define boundary_warning_distance 80.0

// bullet parameter
#define max_bullet_number 30
#define bullet_speed 7
#define shooting_cooldown 500		// in ms
#define bullet_radius 2.0
#define shoot_missile_cooldown 2000		// in ms
#define missile_speed 4
#define bullet_damage 30
#define missile_damage 200

// asteroid parameter
#define asteroid_max_speed 7
#define asteroid_min_speed 2
#define asteroid_max_radius 70
#define asteroid_min_radius 15
#define max_asteroid_number 30
#define asteroid_roundness 20
#define time_between_waves 5000     // in ms
#define asteroid_rotation_max_speed 6
#define asteroid_rotation_min_speed 2

#define particle_max_speed 5
#define particle_min_speed 2
#define particle_exist_time 70

// player struct
typedef struct {
	float x, y;      // position
	float r, g, b;   // colour
	float angle;
} player_ship;

// bullet struct
typedef struct {
	float x, y;      // position
	float r, g, b;   // colour
	float angle;
	int exist;
} bullet;

// asteroid struct
typedef struct {
	float size;
	float x, y;      // position
	float r, g, b;   // colour
	float angle;
	int exist;
	int speed;
	float bumps[asteroid_roundness];
	int rotation_direction;
	int rotation_speed;
	int rotation_angle;
	float hp;
	int into_arena;
	int can_split;
} asteroid;

// particle struct
typedef struct {
	float x, y;      // position
	float r, g, b;   // colour
	float angle;
	int time_pass;
	int speed;
} particle;

#endif // !CONSTANT_H
