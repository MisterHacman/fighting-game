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
#define PLAYER_FRAME_RATE 5

typedef uint8_t bool;

typedef struct {
	int16_t x, y, xvel, yvel;

	uint8_t animation, frame, frame_counter, num_animations, /*bool*/ attached_to_gamepad;
	SDL_Texture **animation_sheets;
	int8_t **animation_data;

	Controller controller;

	ControlStatus control_status;
} Player;

static int NUM_TURTLE_ANIMATIONS = 1;

static int8_t TURTLE_IDLE[] = {0,0,0,0,1,2,3,3,3,3,4,5, -1};

static int8_t *TURTLE_ANIMATION_DATA[] = {
	&TURTLE_IDLE[0],
};

Error initPlayer(Player *player, const char *player_name, SDL_Renderer *renderer);

Error initAnimationSheets(SDL_Texture ** *animation_sheets, const char *player_name, SDL_Renderer *renderer);
int getNumAnimations(const char *player_name);
int getAnimationIndex(const char *animation_name);

Error initAnimationData(int8_t ** *animation_data, const char *player_name);

Error updatePlayer(Player *player);
Error drawPlayer(Player player, SDL_Renderer *renderer);

void freePlayer(Player *player);

#endif // PLAYER
