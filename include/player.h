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
	int16_t x, y, xvel, yvel;
	uint8_t dir, xaccel, yaccel, max_xvel, max_yvel;

	SDL_Texture *textures;

	bool attached_to_gamepad;
	Controller controller;
	ControlStatus control_status;
} Player;

#define CAT 0
typedef const uint32_t PlayerId;

Error initPlayer(Player *player, PlayerId player_name, SDL_Renderer *renderer);

Error initPlayerConstants(Player *player, PlayerId player_id);

Error updatePlayer(Player *player);
Error drawPlayer(Player player, SDL_Renderer *renderer);

void freePlayer(Player *player);

#endif // PLAYER
