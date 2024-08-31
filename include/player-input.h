#ifndef _PLAYER_INPUT_H
#define _PLAYER_INPUT_H

#include "error.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL.h>

#define BUTTON 0
#define JOYSTICK 1

typedef uint8_t bool;
typedef uint8_t InputType;
typedef uint8_t InputId;
typedef InputId Button;

typedef struct {
	InputType type;
	InputId id;
	Button alternate;
} Input;

typedef struct {
	SDL_GameController *gamepad;
	Input left, right, up, down;
	Input jump, grab1, grab2;
	Button pause;
} Controller;

static const Controller DEFAULT_CONTROLLER = {
	.gamepad = NULL,
	.left = {
		.type = JOYSTICK, .id = SDL_CONTROLLER_AXIS_LEFTX,
		.alternate = SDL_CONTROLLER_BUTTON_DPAD_LEFT
	}, .right = {
		.type = JOYSTICK, .id = SDL_CONTROLLER_AXIS_LEFTX,
		.alternate = SDL_CONTROLLER_BUTTON_DPAD_RIGHT
	}, .up = {
		.type = JOYSTICK, .id = SDL_CONTROLLER_AXIS_LEFTY,
		.alternate = SDL_CONTROLLER_BUTTON_DPAD_UP
	}, .down = {
		.type = JOYSTICK, .id = SDL_CONTROLLER_AXIS_LEFTY,
		.alternate = SDL_CONTROLLER_BUTTON_DPAD_DOWN
	}, .jump = {
		.type = BUTTON, .id = SDL_CONTROLLER_BUTTON_A,
		.alternate = SDL_CONTROLLER_BUTTON_B
	}, .grab1 = {
		.type = JOYSTICK, .id = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		.alternate = SDL_CONTROLLER_BUTTON_LEFTSHOULDER
	}, .grab2 = {
		.type = JOYSTICK, .id = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
		.alternate = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
	}, .pause = SDL_CONTROLLER_BUTTON_START
};

typedef struct {
	int8_t horizontal, vertical;
	bool jump, grab1, grab2, pause;
} ControlStatus;

Error initController(Controller *controller, int gamepad_index);

Error getPlayerAxisInput(ControlStatus *status, Controller controller, SDL_Event event);
Error getPlayerButtonInput(ControlStatus *status, Controller controller, SDL_Event event, bool button_state);

#endif // PLAYER_INPUT
