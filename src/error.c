#include "error.h"

void SDLError() {
	fprintf(stderr, "%s", SDL_GetError());
}
