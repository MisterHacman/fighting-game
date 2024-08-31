#include "event-handler.h"

Error handleEvents(Game *game, SDL_Event event) {
	Error error;

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

	return SUCCESS;
}
