#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "player-input.h"
#include "error.h"

#include <SDL.h>
#include <SDL_image.h>

#include <dirent.h>

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16

#define PLAYER_TEXTURE_WIDTH 16
#define PLAYER_TEXTURE_HEIGHT 16

#define IDLE 0
#define ANIMATION_COUNT 1
#define PLAYER_ANIMATION_FRAMERATE 5 // 12 FPS
#define PLAYER_MOVEMENT_FRAMERATE 3 // 20 FPS

typedef uint8_t bool;

#define CAT 0
typedef uint32_t PlayerId;

typedef struct {
	float length;
	int8_t connection;
} Limb;

#define HEAD -1
#define CHEST 0
#define BOTTOM 1
#define LEFT_ELBOW 2
#define RIGHT_ELBOW 3
#define LEFT_KNEE 4
#define RIGHT_KNEE 5
static const Limb CAT_BODY[] = {
	{ .length = 1.0, .connection = HEAD }, 			// Neck
	{ .length = 1.1, .connection = CHEST }, 		// Back
	{ .length = 0.5, .connection = CHEST }, 		// Left upper arm
	{ .length = 0.5, .connection = CHEST }, 		// Right upper arm
	{ .length = 0.5, .connection = BOTTOM },		// Left thigh
	{ .length = 0.5, .connection = BOTTOM },		// Right thigh
	{ .length = 0.5, .connection = LEFT_ELBOW }, 	// Left forearm
	{ .length = 0.5, .connection = RIGHT_ELBOW },	// Right forearm
	{ .length = 0.5, .connection = LEFT_KNEE },		// Left calf 
	{ .length = 0.5, .connection = RIGHT_KNEE },	// Right calf
};

typedef struct {
	PlayerId id;

	struct { float x, y, xvel, yvel; } head;
	int8_t dir;

	bool attached_to_gamepad;
	Controller controller;
	ControlStatus control_status;
} Player;

Error initPlayer(Player *player, PlayerId player_name, SDL_Renderer *renderer);

Error initPlayerConstants(Player *player, PlayerId player_id);

Error updatePlayer(Player *player);
Error drawPlayer(Player player, SDL_Renderer *renderer);

void freePlayer(Player *player);

#endif // PLAYER
