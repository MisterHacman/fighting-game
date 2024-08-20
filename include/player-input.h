#ifndef _PLAYER_INPUT_H
#define _PLAYER_INPUT_H

#include "error.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL.h>

#define BUTTON 0
#define JOYSTICK 1
#define DPAD 2

typedef uint8_t bool;
typedef uint8_t InputType;

typedef struct {
	SDL_GameController *gamepad;
	/*SDL_GameControllerButton*/ uint8_t strong_attack, quick_attack, jump, pause;
} Controller;

typedef struct {
	int8_t movement_horizontal, movement_vertical;
	bool strong_attack, quick_attack, jump, dodge, parry, pause;
} ControlStatus;

Error initController(Controller *controller, int gamepad_index);

Error getPlayerInput(ControlStatus *status, Controller controller);

#endif // PLAYER_INPUT
