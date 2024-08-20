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
	controller_data.dodge = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	controller_data.parry = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	controller_data.pause = SDL_CONTROLLER_BUTTON_START;

	*controller = controller_data;
	return SUCCESS;
}

Error getPlayerAxisInput(ControlStatus *status, Controller controller, SDL_Event event) {
	if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		status->movement_horizontal = event.caxis.value / 0x4001;
	if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		status->movement_vertical = event.caxis.value / 0x401;

	if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		status->dodge = event.caxis.value > 0x401;
	if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		status->parry = event.caxis.value > 0x401;

	return SUCCESS;
}

Error getPlayerButtonInput(ControlStatus *status, Controller controller, SDL_Event event, bool button_state) {
	if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		status->movement_horizontal = -button_state;
	if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		status->movement_horizontal = button_state;
	if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		status->movement_vertical = button_state;
	if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		status->movement_vertical = -button_state;

	if (event.cbutton.button == controller.strong_attack)
		status->strong_attack = button_state;
	if (event.cbutton.button == controller.quick_attack)
		status->quick_attack = button_state;
	if (event.cbutton.button == controller.jump)
		status->jump = button_state;

	if (event.cbutton.button == controller.dodge)
		status->dodge = button_state;
	if (event.cbutton.button == controller.parry)
		status->parry = button_state;

	if (event.cbutton.button == controller.pause)
		status->pause = button_state;

	return SUCCESS;
}
