#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "shape_draw.h"

// Initialize Control Window
Uint32 InitializeControlWindow();

// Draw Contorols
void DrawControls();

//Change Shape And Color
SHAPE ChangeControl(SDL_Event *event);

// Clean UP Controls
void ControlCleanUp();

#endif // CONTROLS_H
