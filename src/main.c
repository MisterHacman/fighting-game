#include "main.h"

Error main() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
		fprintf(stderr, "%s", SDL_GetError());
		return SDL_ERROR;
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		SDLError();
		SDL_Quit();
		return SDL_IMG_ERROR;
	}

	SDL_DisplayMode display_mode;
	if (SDL_GetDesktopDisplayMode(0, &display_mode) < 0) {
		SDLError();
		SDL_Quit();
		IMG_Quit();
		return SDL_ERROR;
	}

	int window_scale = display_mode.h / SCREEN_HEIGHT;
	int window_width = SCREEN_WIDTH * window_scale, window_height = SCREEN_HEIGHT * window_scale;

	SDL_Window *window = SDL_CreateWindow(
		WIN_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height,
		WIN_FLAGS);
	if (window == NULL) {
		SDLError();
		SDL_Quit();
		IMG_Quit();
		return SDL_ERROR;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, REND_FLAGS);
	if (renderer == NULL) {
		SDLError();
		SDL_DestroyWindow(window);
		SDL_Quit();
		IMG_Quit();
		return SDL_ERROR;
	}

	int error;
	Game game;
	if ((error = initGame(&game, renderer))) {
		if (error == SDL_ERROR) SDLError();
		cleanup(game, window, renderer);
		return error;
	}

	if (SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0) {
		SDLError();
		cleanup(game, window, renderer);
		return SDL_ERROR;
	}

	uint64_t start_time = SDL_GetTicks64(), end_time, frame_time, longest = 0, frame_time_sum, frames = 0;
	while (game.running) {
		if ((error = update(&game))) {
			if (error == SDL_ERROR) SDLError();
			cleanup(game, window, renderer);
			return error;
		}
		if ((error = draw(game, renderer))) {
			if (error == SDL_ERROR) SDLError();
			if (error == FILESYSTEM_ERROR) fprintf(stderr, "fs error\n");
			if (error == LOGIC_ERROR) fprintf(stderr, "logic error\n");
			cleanup(game, window, renderer);
			return error;
		}

		end_time = SDL_GetTicks64();
		frame_time = end_time - start_time;
		if (frame_time > longest) longest = frame_time;
		frame_time_sum += frame_time;
		frames++;

		start_time = SDL_GetTicks64();
	}

	printf("longest frame took %ld ms\n", longest);
	printf("average fps was %f\n", 1000.0 * frames / frame_time_sum);
	cleanup(game, window, renderer);
	return SUCCESS;
}

Error initGame(Game *game, SDL_Renderer *renderer) {
	printf("initializing game data\n\n");
	Game game_data = {
		.running = 1,
	};
	*game = game_data;

	Error error;
	if ((error = initPlayer(&game->player, CAT, renderer))) return error;
	return SUCCESS;
}

Error update(Game *game) {
	//printf("updating, time passed: %ldms\n", SDL_GetTicks64());
	Error error;

	SDL_Event event;
	while (SDL_PollEvent(&event))
		handleEvents(game, event);

	if ((error = updatePlayer(&game->player))) return error;
	return SUCCESS;
}


Error draw(Game game, SDL_Renderer *renderer) {
	Error error;

	if (SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255) < 0) return SDL_ERROR;
	if (SDL_RenderClear(renderer) < 0) return SDL_ERROR;

	if ((error = drawPlayer(game.player, renderer))) return error;

	SDL_RenderPresent(renderer);
	return SUCCESS;
}

void cleanup(Game game, SDL_Window *window, SDL_Renderer *renderer) {
	printf("\nfreeing resources\n");
	freePlayer(&game.player);
	printf("freeing SDL objects\n");
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	printf("freeing SDL subsystems\n");
	SDL_Quit();
	IMG_Quit();
}
