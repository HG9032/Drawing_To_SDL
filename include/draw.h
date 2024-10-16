#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <stdio.h>

// Enum to define different shapes that can be drawn
typedef enum {
    ERASE,      // Erase mode
    POINT,      // Point shape
    LINE,       // Line shape
    RECTANGLE   // Rectangle shape (not implemented yet)
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

// Structure to hold data for different shapes
typedef struct {
    SDL_Rect rectangle;  // Rectangle shape (not used yet)
    LINE_STRUCT line;    // Line data
    POINT_STRUCT point;  // Point data
} SHAPE_DATA;

// Function declaration for drawing shapes
void draw(SDL_Renderer *renderer, SHAPE shape, SHAPE_DATA *data);

#endif // DRAW_H
