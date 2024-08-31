#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include "game.h"
#include "error.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL.h>

Error handleEvents(Game *game, SDL_Event event);

#endif // EVENT_HANDLER
