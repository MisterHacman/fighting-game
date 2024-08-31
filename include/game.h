#ifndef _GAME_H
#define _GAME_H

#include "player.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define FRAME_RATE 16

typedef struct {
	int running;
	int win_width, win_height;
	Player player;
} Game;

#endif // GAME
