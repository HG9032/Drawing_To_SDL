#include "draw.h"

// Function to draw different shapes on the renderer
void draw(SDL_Renderer *renderer, SHAPE shape, SHAPE_DATA *data) {
    // Set the draw color to black (RGBA)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Switch based on the shape type to draw
    switch (shape) {
        case POINT:  // Draw a single point
            SDL_RenderDrawPoint(renderer, data->point.startX, data->point.startY);
            break;
        case LINE:   // Draw a line between two points
            SDL_RenderDrawLine(renderer,
                               data->line.startX, data->line.startY,  // Start coordinates
                               data->line.endX, data->line.endY);    // End coordinates
            break;
        // Additional shapes (like RECTANGLE) can be handled here in the future
    }
}
