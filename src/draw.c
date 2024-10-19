#include "draw.h"


// Function to draw different shapes on the renderer
void draw(SDL_Renderer *renderer, SHAPE shape, SHAPE_DATA *data, int shapeNumber) {

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
        case MOVE:

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

// Move shape logic
bool moveShape(SDL_Event event, SHAPE_DATA *data, SHAPE shape, int shapeNumber, SDL_Renderer *renderer, SDL_Texture *savedBuffer) {
    static int offsetX = 0, offsetY = 0;  // Store the offset between the mouse and shape's position
    static bool moveIt = false;
    static int x1, y1;

    if (isMouseDown && isLeftButton) {
        // Start the movement by calculating the initial offset
        offsetX = mouseX - arrayOfShapes[shapeNumber].data.line.startX;
        offsetY = mouseY - arrayOfShapes[shapeNumber].data.line.startY;
        moveIt = true;
        return false;
    } else if ((isMouseInMotion || isMouseUP) && isLeftButton && moveIt) {
        int newX = mouseX - offsetX;  // New position for the start X
        int newY = mouseY - offsetY;  // New position for the start Y

        // Update both start and end coordinates of the line
        int deltaX = newX - arrayOfShapes[shapeNumber].data.line.startX;
        int deltaY = newY - arrayOfShapes[shapeNumber].data.line.startY;

        // Update line's start and end points with the delta
        arrayOfShapes[shapeNumber].data.line.startX += deltaX;
        arrayOfShapes[shapeNumber].data.line.startY += deltaY;
        arrayOfShapes[shapeNumber].data.line.endX += deltaX;
        arrayOfShapes[shapeNumber].data.line.endY += deltaY;

         // Render everything again to ensure other shapes remain
        SDL_SetRenderTarget(renderer, savedBuffer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);  // Clear the renderer before redrawing
        for (int i = 0; i < 10; i++) {  // Assuming totalShapes is the total number of shapes
            draw(renderer, arrayOfShapes[i].shape, &arrayOfShapes[i].data, i);
        }
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, &savedBuffer, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Stop moving if the mouse button is released
        if (isMouseUP) {
            moveIt = false;
        }
        return true;
    }
    return false;
}

// Update the shape data based on the current shape and event
SHAPE updateData(SHAPE shape, SHAPE_DATA *data, SDL_Event event, int shapeNumber, SDL_Renderer *renderer, SDL_Texture *savedBuffer) {
    // Switch between different shapes and update their respective data
    switch (shape) {
        case POINT:
            return pointDataUpdate(event, data) == true ? POINT : NONE;  // Handle point update
        case LINE:
            return lineDataUpdate(event, data) == true ? LINE : NONE;   // Handle line update
        case RECTANGLE:
            return rectangleDataUpdate(event, data) == true ? RECTANGLE : NONE;  // Handle rectangle update
        case MOVE:
            return moveShape(event, data, shape, shapeNumber, renderer, savedBuffer) == true ? MOVE : NONE;
        default:
            return NONE;  // No valid shape selected
    }
}
