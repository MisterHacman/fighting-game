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

typedef struct {
	int16_t x, y, xvel, yvel, dir;
	uint8_t xaccel, yaccel, max_xvel, max_yvel;

	uint8_t animation, frame, animation_frame_counter, num_animations,
			freeze_timer, movement_frame_counter;

	SDL_Texture **animation_sheets;
	int8_t **animation_data;

	bool attached_to_gamepad;
	Controller controller;

	ControlStatus control_status;
} Player;

static const int TURTLE_XACCEL = 1;
static const int TURTLE_YACCEL = 1;
static const int TURTLE_MAX_XVEL = 3;
static const int TURTLE_MAX_YVEL = 3; // 4 pixels per 

static const int NUM_TURTLE_ANIMATIONS = 1;

static int8_t TURTLE_IDLE[] = {0,0,0,0,1,2,3,3,3,3,4,5, -1};

static int8_t *TURTLE_ANIMATION_DATA[] = {
	&TURTLE_IDLE[0],
};

Error initPlayer(Player *player, const char *player_name, SDL_Renderer *renderer);

Error initPlayerConstants(Player *player, const char *player_name);

Error initPlayerAnimationSheets(SDL_Texture ** *animation_sheets, const char *player_name, SDL_Renderer *renderer);
int getPlayerNumAnimations(const char *player_name);
int getPlayerAnimationIndex(const char *animation_name);

Error initPlayerAnimationData(int8_t ** *animation_data, const char *player_name);

Error updatePlayer(Player *player);
Error drawPlayer(Player player, SDL_Renderer *renderer);

void freePlayer(Player *player);

#endif // PLAYER
