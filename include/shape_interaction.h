#ifndef SHAPE_INTERACTION_H
#define SHAPE_INTERACTION_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "shape_draw.h"

// Function declarations for cursor and shape interactions
SDL_Cursor *LoadCustomCursor(const char *iconPath);
void SaveShape(SHAPE shape, SHAPE_DATA *data);
int IsMouseOverShape(SDL_Event event);

#endif // SHAPE_INTERACTION_H
