#ifndef _MAIN_H
#define _MAIN_H

#include "player.h"
#include "error.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#define WIN_TITLE "Fighting Game"
#define WIN_FLAGS SDL_WINDOW_SHOWN
#define REND_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define FRAME_RATE 16

typedef struct {
	int running;
	int win_width, win_height;
	Player player;
} Game;

Error main();
Error initGame(Game *game, SDL_Renderer *renderer);
Error update(Game *game);
Error draw(Game game, SDL_Renderer *renderer);
void cleanup(Game game, SDL_Window *window, SDL_Renderer *renderer);

#endif // MAIN
