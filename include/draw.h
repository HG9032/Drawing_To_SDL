#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Macros to handle mouse events
#define isMouseDown event.type == SDL_MOUSEBUTTONDOWN
#define isMouseUP event.type == SDL_MOUSEBUTTONUP
#define isMouseInMotion event.type == SDL_MOUSEMOTION

// Macros to track mouse position and button state
#define mouseX event.button.x
#define mouseY event.button.y
#define isLeftButton event.button.button == SDL_BUTTON_LEFT

// Enum to define different shapes that can be drawn
typedef enum {
    NONE,       // Nothing selected
    ERASE,      // Erase mode (not implemented)
    POINT,      // Point shape
    LINE,       // Line shape
    RECTANGLE,  // Rectangle shape
    MOVE        // Move shape
} SHAPE;

// Structure to hold line coordinates
typedef struct {
    int startX;  // Starting X coordinate
    int startY;  // Starting Y coordinate
    int endX;    // Ending X coordinate
    int endY;    // Ending Y coordinate
} LINE_STRUCT;

// Structure to hold point coordinates
typedef struct {
    int startX;  // X coordinate of the point
    int startY;  // Y coordinate of the point
} POINT_STRUCT;

// Union to hold data for different shapes
typedef union {
    SDL_Rect rectangle;  // Rectangle shape
    LINE_STRUCT line;    // Line shape
    POINT_STRUCT point;  // Point shape
} SHAPE_DATA;

typedef struct{
    SHAPE shape;
    SHAPE_DATA data;
} SHAPE_AND_DATA;

SHAPE_AND_DATA arrayOfShapes[10];

// Function declarations for drawing shapes
void draw(SDL_Renderer *renderer, SHAPE shape, SHAPE_DATA *data, int shapeNumber);

// Functions for updating SHAPE_DATA for specific SHAPE types
bool pointDataUpdate(SDL_Event event, SHAPE_DATA *data);
bool lineDataUpdate(SDL_Event event, SHAPE_DATA *data);
bool rectangleDataUpdate(SDL_Event event, SHAPE_DATA *data);
bool moveShape(SDL_Event event, SHAPE_DATA *data, SHAPE shape, int shapeNumber, SDL_Renderer *renderer, SDL_Texture *savedBuffer);

// Function to update shape data based on the current event
SHAPE updateData(SHAPE shape, SHAPE_DATA *data, SDL_Event event, int shapeNumber, SDL_Renderer *renderer, SDL_Texture *savedBuffer);

#endif // DRAW_H
