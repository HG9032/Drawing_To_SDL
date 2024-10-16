#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "draw.h"

// Global window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Function declarations
bool Init(SDL_Window **window, SDL_Renderer **renderer);
void ShutDown(SDL_Window *window, SDL_Renderer *renderer);
void Loop(SDL_Renderer *renderer);
SDL_Texture* CreateClearTexture(SDL_Renderer *renderer, int width, int height);

// Main function
int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL and create window/renderer
    if (!Init(&window, &renderer)) return 1;

    // Main application loop
    Loop(renderer);

    // Cleanup and shutdown
    ShutDown(window, renderer);

    return 0;
}

// Initializes SDL, creates window and renderer
bool Init(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed. Error: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("My Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!*window) {
        printf("Window creation failed. Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        printf("Renderer creation failed. Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

// Cleans up and shuts down SDL
void ShutDown(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Creates a texture and clears it to a white background
SDL_Texture* CreateClearTexture(SDL_Renderer *renderer, int width, int height) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture) {
        printf("Texture creation failed. Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White background
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);  // Reset to default render target

    return texture;
}

// Main event loop to handle input and rendering
void Loop(SDL_Renderer *renderer) {
    bool running = true;  // Controls the loop execution
    SDL_Event event;      // Stores event data
    SHAPE shape = ERASE;  // Initial shape set to ERASE
    SDL_Texture *savedBuffer = NULL;

    // Create texture for saving the drawing state
    savedBuffer = CreateClearTexture(renderer, windowWidth, windowHeight);
    if (!savedBuffer) return;  // If texture creation failed, exit the loop

    // Render the cleared texture onto the screen
    SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
    SDL_RenderPresent(renderer);

    bool down = false;  // Keeps track if the mouse button is held down
    SHAPE_DATA data;

    // Event loop to process input
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:  // Handle window close event
                    running = false;
                    break;
                case SDL_KEYDOWN:  // Handle keyboard input
                    switch (event.key.keysym.sym) {
                        case SDLK_x:  // Exit on 'x' key
                            running = false;
                            break;
                        case SDLK_l:  // Switch to line shape
                            shape = LINE;
                            break;
                        case SDLK_r:  // Switch to rectangle shape
                            shape = RECTANGLE;
                            break;
                        case SDLK_p:  // Switch to point shape
                            shape = POINT;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:  // Handle mouse button press
                    switch (shape) {
                        case POINT:  // Draw a point at the mouse position
                            data.point.startX = event.button.x;
                            data.point.startY = event.button.y;
                            break;
                        case LINE:  // Start drawing a line
                            down = true;
                            data.line.startX = event.button.x;
                            data.line.startY = event.button.y;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:  // Handle mouse button release
                    if (shape == LINE && down) {
                        down = false;  // Stop drawing the line
                        data.line.endX = event.button.x;
                        data.line.endY = event.button.y;
                        draw(renderer, shape, &data);
                    }
                    // Save the drawn line to the texture
                    SDL_SetRenderTarget(renderer, savedBuffer);
                    draw(renderer, shape, &data);
                    SDL_SetRenderTarget(renderer, NULL);

                    // Copy the texture to the screen
                    SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
                    SDL_RenderPresent(renderer);
                    break;
                case SDL_MOUSEMOTION:  // Handle mouse movement
                    if (shape == LINE && down) {  // If drawing a line
                        // Restore previous frame
                        SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);

                        // Update the end coordinates of the line
                        data.line.endX = event.motion.x;
                        data.line.endY = event.motion.y;
                        draw(renderer, shape, &data);  // Draw the current line

                        SDL_RenderPresent(renderer);  // Update the renderer
                    }
                    break;
            }
        }
    }

    // Cleanup textures after loop ends
    SDL_DestroyTexture(savedBuffer);
}
