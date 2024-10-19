#include "shape_interaction.h"

int i = 0;

// Load cursor icon from a bitmap file
SDL_Cursor* LoadCustomCursor(const char *iconPath) {
    // Load the cursor image as a surface
    SDL_Surface *cursorSurface = SDL_LoadBMP(iconPath);
    if (!cursorSurface) {
        printf("Failed to load cursor image: %s\n", SDL_GetError());
        return NULL;
    }

    // Create a color cursor from the surface
    SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    SDL_FreeSurface(cursorSurface);  // Free the surface after creating the cursor
    return cursor;
}

// Save drawn shape into the global shapes array
void SaveShape(SHAPE shape, SHAPE_DATA *data) {
    arrayOfShapes[i].shape = shape;
    arrayOfShapes[i].data = *data;
    i++;
}

// Check if the mouse is over any shape
int IsMouseOverShape(SDL_Event event) {
    for (int i = 0; i < 10; i++) {
        switch (arrayOfShapes[i].shape) {
            case LINE: {
                // Get the line data from the shape array
                int x1 = arrayOfShapes[i].data.line.startX;
                int y1 = arrayOfShapes[i].data.line.startY;
                int x2 = arrayOfShapes[i].data.line.endX;
                int y2 = arrayOfShapes[i].data.line.endY;

                // Calculate the distance from the mouse to the line
                double numerator = fabs((y2 - y1) * mouseX - (x2 - x1) * mouseY + x2 * y1 - y2 * x1);
                double denominator = sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
                double distance = numerator / denominator;

                // Set a threshold for "hover" detection (e.g., 5 pixels)
                double threshold = 5.0;

                // Check if the mouse is within the distance threshold of the line
                if (distance <= threshold) {
                    // Ensure the mouse is within the line segment bounds
                    if ((mouseX >= fmin(x1, x2) && mouseX <= fmax(x1, x2)) &&
                        (mouseY >= fmin(y1, y2) && mouseY <= fmax(y1, y2))) {
                        return i;  // Mouse is over the line
                    }
                }
                break;
            }
            case RECTANGLE: {
                int x1 = arrayOfShapes[i].data.rectangle.x;
                int y1 = arrayOfShapes[i].data.rectangle.y;
                int x2 = x1 + arrayOfShapes[i].data.rectangle.w;
                int y2 = y1 + arrayOfShapes[i].data.rectangle.h;

                // Correct the coordinates in case the rectangle was drawn in reverse
                int left = (x1 < x2) ? x1 : x2;
                int right = (x1 > x2) ? x1 : x2;
                int top = (y1 < y2) ? y1 : y2;
                int bottom = (y1 > y2) ? y1 : y2;

                // Check if the mouse is near the edges of the rectangle
                if ((abs(mouseX - left) < 5 || abs(mouseX - right) < 5) && (top <= mouseY && mouseY <= bottom)) {
                    return i;  // Mouse is over a vertical edge
                }
                if ((abs(mouseY - top) < 5 || abs(mouseY - bottom) < 5) && (left <= mouseX && mouseX <= right)) {
                    return i;  // Mouse is over a horizontal edge
                }
                break;
            }
            case POINT:
                // Check if the mouse is over a point (optional)
                break;
            default:
                return -1;  // No shape to check
            }
        }
    return -1;  // No shape found under the mouse
}
