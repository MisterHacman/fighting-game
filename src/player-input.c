#include "player-input.h"

Error initController(Controller *controller, int gamepad_index) {
	printf("opening gamepad\n\n");
	controller->gamepad = SDL_GameControllerOpen(gamepad_index);
	if (controller->gamepad == NULL) return SDL_ERROR;
	return SUCCESS;
}

#define CLAMP(x) ((x) > 0 ? (x) : 0)

Error getPlayerAxisInput(ControlStatus *status, Controller controller, SDL_Event event) {
	uint16_t left = 0, right = 0, up = 0, down = 0;

	if (controller.left.type == JOYSTICK && event.caxis.axis == controller.left.id) left = CLAMP(-event.caxis.value);
	if (controller.right.type == JOYSTICK && event.caxis.axis == controller.right.id) right = CLAMP(event.caxis.value);
	status->horizontal = (left + right) >> 8;

	if (controller.up.type == JOYSTICK && event.caxis.axis == controller.left.id) up = CLAMP(event.caxis.value);
	if (controller.down.type == JOYSTICK && event.caxis.axis == controller.right.id) down = CLAMP(-event.caxis.value);
	status->vertical = (up + down) >> 8;

	if (controller.jump.type == JOYSTICK && event.caxis.axis == controller.jump.id) status->jump = event.caxis.value >> 8;
	if (controller.grab1.type == JOYSTICK && event.caxis.axis == controller.grab1.id) status->grab1 = event.caxis.value >> 8;
	if (controller.grab2.type == JOYSTICK && event.caxis.axis == controller.grab2.id) status->grab2 = event.caxis.value >> 8;

	return SUCCESS;
}

Error getPlayerButtonInput(ControlStatus *status, Controller controller, SDL_Event event, bool button_state) {
	if (controller.left.type == BUTTON && event.cbutton.button == controller.left.id)
		status->horizontal = button_state * 0x80;
	if (controller.right.type == BUTTON && event.cbutton.button == controller.right.id)
		status->horizontal = button_state * 0x7f;
	if (controller.up.type == BUTTON && event.cbutton.button == controller.up.id)
		status->vertical = button_state * 0x7f;
	if (controller.down.type == BUTTON && event.cbutton.button == controller.down.id)
		status->vertical = button_state * 0x80;

	if (controller.jump.type == BUTTON && event.cbutton.button == controller.jump.id) status->jump = button_state;
	if (controller.grab1.type == BUTTON && event.cbutton.button == controller.grab1.id) status->grab1 = button_state;
	if (controller.grab2.type == BUTTON && event.cbutton.button == controller.grab2.id) status->grab2 = button_state;

	if (event.cbutton.button == controller.pause) status->pause = button_state;

	// Check alternate controls
	if (event.cbutton.button == controller.left.alternate) status->horizontal = button_state * 0x80;
	if (event.cbutton.button == controller.right.alternate) status->horizontal = button_state * 0x7f;
	if (event.cbutton.button == controller.up.alternate) status->vertical = button_state * 0x7f;
	if (event.cbutton.button == controller.down.alternate) status->vertical = button_state * 0x80;

	if (event.cbutton.button == controller.jump.alternate) status->jump = button_state;
	if (event.cbutton.button == controller.grab1.alternate) status->grab1 = button_state;
	if (event.cbutton.button == controller.grab2.alternate) status->grab2 = button_state;

	//printf("%d %d %d %d %d\n", status->horizontal, status->vertical, status->jump, status->grab1, status->grab2);
	return SUCCESS;
}
