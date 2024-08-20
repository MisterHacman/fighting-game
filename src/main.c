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

	uint64_t start_time = SDL_GetTicks64(), end_time, new_time, delay, lowest = 0xffffffffffffffff;
	float fps;
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
		delay = FRAME_RATE - end_time + start_time;
		SDL_Delay(delay * !(delay & 0x8000000000000000));

		new_time = SDL_GetTicks64();
		fps = 1000. / (new_time - start_time);
		//printf("running in %f fps\n\n", fps);
		if (fps > lowest) lowest = fps;
		start_time = new_time;
	}

	printf("lowest fps was %f\n", fps);
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
	if ((error = initPlayer(&game->player, "turtle", renderer))) return error;
	return SUCCESS;
}

Error update(Game *game) {
	//printf("updating, time passed: %ldms\n", SDL_GetTicks64());
	Error error;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->running = 0;
				return SUCCESS;
			
			case SDL_CONTROLLERDEVICEADDED:
				if (!game->player.attached_to_gamepad && SDL_IsGameController(event.cdevice.which)) {
					if ((error = initController(&game->player.controller, event.cdevice.which))) return error;
					game->player.attached_to_gamepad = 1;
				}
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				if (event.cdevice.which ==
						SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(game->player.controller.gamepad))) {
					game->player.attached_to_gamepad = 0;
					game->player.controller.gamepad = NULL;
				}
				break;

			case SDL_CONTROLLERAXISMOTION:
				if (!game->player.attached_to_gamepad && SDL_IsGameController(event.cdevice.which)) {
					if ((error = initController(&game->player.controller, event.cdevice.which))) return error;
					game->player.attached_to_gamepad = 1;
				}
				if (event.cdevice.which ==
						SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(game->player.controller.gamepad))) {
					if ((error = getPlayerAxisInput(&game->player.control_status, game->player.controller, event)))
						return error;
				}
				break;

			case SDL_CONTROLLERBUTTONDOWN:
				if (!game->player.attached_to_gamepad && SDL_IsGameController(event.cdevice.which)) {
					if ((error = initController(&game->player.controller, event.cdevice.which))) return error;
					game->player.attached_to_gamepad = 1;
				}
				if (event.cdevice.which ==
						SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(game->player.controller.gamepad))) {
					if ((error = getPlayerButtonInput(&game->player.control_status, game->player.controller, event, 1)))
						return error;
				}
				break;

			case SDL_CONTROLLERBUTTONUP:
				if (!game->player.attached_to_gamepad && SDL_IsGameController(event.cdevice.which)) {
					if ((error = initController(&game->player.controller, event.cdevice.which))) return error;
					game->player.attached_to_gamepad = 1;
				}
				if (event.cdevice.which ==
						SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(game->player.controller.gamepad))) {
					if ((error = getPlayerButtonInput(&game->player.control_status, game->player.controller, event, 0)))
						return error;
				}
				break;

			default:
				break;
		}
	}
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
