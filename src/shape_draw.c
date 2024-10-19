#include "shape_draw.h"

// Function to draw different shapes on the renderer
void DrawShape(SDL_Renderer *renderer, SHAPE shape, SHAPE_DATA *data, int shapeIndex) {
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
bool UpdatePointData(SDL_Event event, SHAPE_DATA *data) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        data->point.startX = event.button.x;  // Set X coordinate of the point
        data->point.startY = event.button.y;  // Set Y coordinate of the point
        return true;  // Data updated, return true
    }
    return false;  // No data update, return false
}

// Update line data as the mouse is pressed, moved, or released
bool UpdateLineData(SDL_Event event, SHAPE_DATA *data) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {  // Set start coordinates when mouse button is down
        data->line.startX = event.button.x;
        data->line.startY = event.button.y;
        return false;  // Initial click does not complete the line
    } else if ((event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONUP) && event.button.button == SDL_BUTTON_LEFT) {  // Update end coordinates while mouse moves or when button is released
        data->line.endX = event.button.x;
        data->line.endY = event.button.y;
        return true;  // Line is completed
    }
    return false;  // No data update
}

// Update rectangle data as the mouse is pressed, moved, or released
bool UpdateRectangleData(SDL_Event event, SHAPE_DATA *data) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {  // Set initial rectangle corner when mouse button is down
                data->rectangle.x = event.button.x;
        data->rectangle.y = event.button.y;
        return false;  // Initial click does not complete the rectangle
    } else if ((event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONUP) && event.button.button == SDL_BUTTON_LEFT) {  // Update rectangle dimensions while mouse moves or button is released
        data->rectangle.w = event.button.x - data->rectangle.x;  // Calculate width
        data->rectangle.h = event.button.y - data->rectangle.y;  // Calculate height
        return true;  // Rectangle is completed
    }
    return false;  // No data update
}

// Move shape logic
bool MoveShape(SDL_Event event, SHAPE_DATA *data, int shapeIndex, SDL_Renderer *renderer, SDL_Texture *savedBuffer) {
    static int offsetX = 0, offsetY = 0;  // Store the offset between the mouse and shape's position
    static bool isMoving = false;

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        // Start the movement by calculating the initial offset
        offsetX = event.button.x - arrayOfShapes[shapeIndex].data.line.startX;
        offsetY = event.button.y - arrayOfShapes[shapeIndex].data.line.startY;
        isMoving = true;
        return false;
    } else if ((event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONUP) && event.button.button == SDL_BUTTON_LEFT && isMoving) {
        int newX = event.button.x - offsetX;  // New position for the start X
        int newY = event.button.y - offsetY;  // New position for the start Y

        // Update both start and end coordinates of the line
        int deltaX = newX - arrayOfShapes[shapeIndex].data.line.startX;
        int deltaY = newY - arrayOfShapes[shapeIndex].data.line.startY;

        switch (arrayOfShapes[shapeIndex].shape) {
            case LINE:
                // Update line's start and end points with the delta
                arrayOfShapes[shapeIndex].data.line.startX += deltaX;
                arrayOfShapes[shapeIndex].data.line.startY += deltaY;
                arrayOfShapes[shapeIndex].data.line.endX += deltaX;
                arrayOfShapes[shapeIndex].data.line.endY += deltaY;
                break;
            case RECTANGLE:
                // Update rectangle's start points with the delta
                arrayOfShapes[shapeIndex].data.rectangle.x += deltaX;
                arrayOfShapes[shapeIndex].data.rectangle.y += deltaY;
                break;
            default:
                return false;
        }

        // Render everything again to ensure other shapes remain
        SDL_SetRenderTarget(renderer, savedBuffer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Clear to white
        SDL_RenderClear(renderer);  // Clear the renderer before redrawing

        for (int i = 0; i < 10; i++) {  // Assuming totalShapes is the total number of shapes
            DrawShape(renderer, arrayOfShapes[i].shape, &arrayOfShapes[i].data, i);
        }

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Stop moving if the mouse button is released
        if (event.type == SDL_MOUSEBUTTONUP) {
            isMoving = false;
        }
        return true;
    }
    return false;
}

// Update the shape data based on the current shape and event
SHAPE UpdateShapeData(SHAPE shape, SHAPE_DATA *data, SDL_Event event, int shapeIndex, SDL_Renderer *renderer, SDL_Texture *savedBuffer) {
    // Switch between different shapes and update their respective data
    switch (shape) {
        case POINT:
            return UpdatePointData(event, data) ? POINT : NONE;  // Handle point update
        case LINE:
            return UpdateLineData(event, data) ? LINE : NONE;   // Handle line update
        case RECTANGLE:
            return UpdateRectangleData(event, data) ? RECTANGLE : NONE;  // Handle rectangle update
        case MOVE:
            return MoveShape(event, data, shapeIndex, renderer, savedBuffer) ? MOVE : NONE;
        default:
            return NONE;  // No valid shape selected
    }
}
