#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "draw.h"
#include "test.h"

// Global window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Function declarations
bool Init(SDL_Window **window, SDL_Renderer **renderer);
void ShutDown(SDL_Window *window, SDL_Renderer *renderer);
void Loop(SDL_Renderer *renderer);
SDL_Texture* CreateClearTexture(SDL_Renderer *renderer, int width, int height);

// Main function to initialize, run the loop, and clean up
int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL and create window/renderer
    if (!Init(&window, &renderer)) return 1;

    // Main application loop
    Loop(renderer);
    // test(renderer);  // Uncomment if you want to run test functionality

    // Clean up and shut down SDL
    ShutDown(window, renderer);

    return 0;
}

// Initializes SDL, creates window and renderer
bool Init(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed. Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window with specified dimensions
    *window = SDL_CreateWindow("My Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!*window) {
        printf("Window creation failed. Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Create renderer for the window with hardware acceleration and VSync
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

    // Clear the texture to a white background
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
    if (!savedBuffer) return;  // Exit if texture creation fails

    // Render the cleared texture onto the screen
    SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
    SDL_RenderPresent(renderer);

    bool down = false;  // Tracks whether the mouse button is held down
    SHAPE_DATA data;

    // Event loop to process user input
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:  // Quit the application
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
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    if (updateData(shape, &data, event)) {
                        // Draw on savedBuffer texture
                        SDL_SetRenderTarget(renderer, savedBuffer);
                        draw(renderer, shape, &data);

                        // Render savedBuffer with the new shape
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
                        SDL_RenderPresent(renderer);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (updateData(shape, &data, event)) {
                        // Draw the savedBuffer texture and preview the new shape
                        SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
                        draw(renderer, shape, &data);
                        SDL_RenderPresent(renderer);
                    }
                    break;
            }
        }
    }

    // Clean up textures when the loop ends
    SDL_DestroyTexture(savedBuffer);
}
