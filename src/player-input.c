#include "player-input.h"

Error initController(Controller *controller, int gamepad_index) {
	printf("initializing controller\n");
	Controller controller_data;

	printf("opening gamepad\n");
	controller_data.gamepad = SDL_GameControllerOpen(gamepad_index);
	if (controller_data.gamepad == NULL) return SDL_ERROR;

	printf("attaching control scheme\n\n");
	controller_data.strong_attack = SDL_CONTROLLER_BUTTON_A;
	controller_data.quick_attack = SDL_CONTROLLER_BUTTON_B;
	controller_data.jump = SDL_CONTROLLER_BUTTON_Y;
	controller_data.pause = SDL_CONTROLLER_BUTTON_START;

	*controller = controller_data;
	return SUCCESS;
}

Error getPlayerInput(ControlStatus *status, Controller controller) {
	status->movement_horizontal = SDL_GameControllerGetAxis(controller.gamepad, SDL_CONTROLLER_AXIS_LEFTX) > 0x7fff;
	status->movement_vertical = SDL_GameControllerGetAxis(controller.gamepad, SDL_CONTROLLER_AXIS_LEFTY) > 0x7fff;
	if (!status->movement_horizontal)
		status->movement_horizontal =
			SDL_GameControllerGetButton(controller.gamepad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
			- SDL_GameControllerGetButton(controller.gamepad, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	if (!status->movement_vertical)
		status->movement_vertical =
			SDL_GameControllerGetButton(controller.gamepad, SDL_CONTROLLER_BUTTON_DPAD_UP)
			- SDL_GameControllerGetButton(controller.gamepad, SDL_CONTROLLER_BUTTON_DPAD_DOWN);

	status->strong_attack = SDL_GameControllerGetButton(controller.gamepad, controller.strong_attack);
	status->quick_attack = SDL_GameControllerGetButton(controller.gamepad, controller.quick_attack);
	status->jump = SDL_GameControllerGetButton(controller.gamepad, controller.jump);
	status->pause = SDL_GameControllerGetButton(controller.gamepad, controller.pause);

	if (!(status->dodge = SDL_GameControllerGetAxis(controller.gamepad, SDL_CONTROLLER_AXIS_TRIGGERLEFT)))
		status->dodge = SDL_GameControllerGetButton(controller.gamepad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	if (!(status->parry = SDL_GameControllerGetAxis(controller.gamepad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)))
		status->parry = SDL_GameControllerGetButton(controller.gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

	return SUCCESS;
}
