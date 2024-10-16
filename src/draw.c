#include "draw.h"

// Function to draw different shapes on the renderer
void draw(SDL_Renderer *renderer, SHAPE shape, SHAPE_DATA *data) {
    // Set the draw color to black (RGBA)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw the appropriate shape based on the shape type
    switch (shape) {
        case POINT:  // Draw a single point
            SDL_RenderDrawPoint(renderer, data->point.startX, data->point.startY);
            break;
        case LINE:   // Draw a line between two points
            SDL_RenderDrawLine(renderer,
                               data->line.startX, data->line.startY,  // Start coordinates
                               data->line.endX, data->line.endY);      // End coordinates
            break;
        case RECTANGLE:  // Draw a rectangle
            SDL_RenderDrawRect(renderer, &(data->rectangle));
            break;
        // Additional shapes can be added here in the future
    }
}

// Functions for updating data for specific shape types

// Update point data when mouse button is pressed
bool pointDataUpdate(SDL_Event event, SHAPE_DATA *data) {
    if (isMouseDown && isLeftButton) {
        data->point.startX = mouseX;  // Set X coordinate of the point
        data->point.startY = mouseY;  // Set Y coordinate of the point
        return true;  // Data updated, return true
    }
    return false;  // No data update, return false
}

// Update line data as the mouse is pressed, moved, or released
bool lineDataUpdate(SDL_Event event, SHAPE_DATA *data) {
    if (isMouseDown && isLeftButton) {  // Set start coordinates when mouse button is down
        data->line.startX = mouseX;
        data->line.startY = mouseY;
        return false;  // Initial click does not complete the line
    } else if ((isMouseInMotion || isMouseUP) && isLeftButton) {  // Update end coordinates while mouse moves or when button is released
        data->line.endX = mouseX;
        data->line.endY = mouseY;
        return true;  // Line is completed
    }
    return false;  // No data update
}

// Update rectangle data as the mouse is pressed, moved, or released
bool rectangleDataUpdate(SDL_Event event, SHAPE_DATA *data) {
    if (isMouseDown && isLeftButton) {  // Set initial rectangle corner when mouse button is down
        data->rectangle.x = mouseX;
        data->rectangle.y = mouseY;
        return false;  // Initial click does not complete the rectangle
    } else if ((isMouseInMotion || isMouseUP) && isLeftButton) {  // Update rectangle dimensions while mouse moves or button is released
        data->rectangle.w = mouseX - data->rectangle.x;  // Calculate width
        data->rectangle.h = mouseY - data->rectangle.y;  // Calculate height
        return true;  // Rectangle is completed
    }
    return false;  // No data update
}

// Update the shape data based on the current shape and event
bool updateData(SHAPE shape, SHAPE_DATA *data, SDL_Event event) {
    // Switch between different shapes and update their respective data
    switch (shape) {
        case POINT:
            return pointDataUpdate(event, data);  // Handle point update
        case LINE:
            return lineDataUpdate(event, data);   // Handle line update
        case RECTANGLE:
            return rectangleDataUpdate(event, data);  // Handle rectangle update
        default:
            return false;  // No valid shape selected
    }
}
