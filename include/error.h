#ifndef _ERROR_H
#define _ERROR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL.h>

#define SUCCESS 0
#define SDL_ERROR 1
#define SDL_IMG_ERROR 2
#define FILESYSTEM_ERROR 3
#define LOGIC_ERROR 4
typedef int Error;

void SDLError();

#endif // ERROR
