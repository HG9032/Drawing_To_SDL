#ifndef SHAPE_MOVEMENT_H
#define SHAPE_MOVEMENT_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "draw.h"

static int i = 0;

// Load cursor icon
SDL_Cursor *loadCustomCursor(const char *iconePath);

// Save drawn SHAPE
void saveShape(SHAPE shape, SHAPE_DATA *data);

// Detect if mouse is over a shape
int isMouseOverShape(SDL_Event event);

#endif // SHAPE_MOVEMENT_H
