#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "shape_draw.h"
#include "shape_interaction.h"
#include "gensdl.h"

// Global window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Function declarations
bool InitializeSDL(SDL_Window **window, SDL_Renderer **renderer);
void CleanupSDL(SDL_Window *window, SDL_Renderer *renderer);
void MainLoop(SDL_Renderer *renderer);
SDL_Texture* CreateClearTexture(SDL_Renderer *renderer, int width, int height);

// Main function to initialize, run the loop, and clean up
int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL and create window/renderer
    if (!InitializeSDL(&window, &renderer)) return 1;

    // Main application loop
    MainLoop(renderer);

    // Clean up and shut down SDL
    CleanupSDL(window, renderer);

    return 0;
}

// Initializes SDL, creates window and renderer
bool InitializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed. Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window with specified dimensions
    *window = SDL_CreateWindow("My Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
void CleanupSDL(SDL_Window *window, SDL_Renderer *renderer) {
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
void MainLoop(SDL_Renderer *renderer) {
    bool running = true;  // Controls the loop execution
    SDL_Event event;      // Stores event data
    SHAPE currentShape = NONE;  // Initial shape set to NONE
    SDL_Texture *savedBuffer = CreateClearTexture(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!savedBuffer) return;  // Exit if texture creation fails

    // Render the cleared texture onto the screen
    SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
    SDL_RenderPresent(renderer);

    SHAPE_DATA shapeData;  // Structure to hold shape data
    int shapeIndex = -1;   // Index of the shape being interacted with

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
                        case SDLK_m: currentShape = MOVE; break;
                        case SDLK_l: currentShape = LINE; break;
                        case SDLK_r: currentShape = RECTANGLE; break;
                        case SDLK_p: currentShape = POINT; break;
                        case SDLK_c: currentShape = CIRCLE; break;
                        case SDLK_g: generate_SDL_code(); break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        shapeIndex = IsMouseOverShape(event);
                    }
                    if (event.type == SDL_MOUSEBUTTONUP) {
                        shapeIndex = -1;  // Reset shape index on mouse button release
                    }

                    // Update shape data and draw if necessary
                    if (UpdateShapeData(currentShape, &shapeData, event, shapeIndex, renderer, savedBuffer) != NONE) {
                        // Draw on savedBuffer texture
                        SDL_SetRenderTarget(renderer, savedBuffer);
                        DrawShape(renderer, currentShape, &shapeData, shapeIndex);

                        // If not moving, save the shape and render the updated buffer
                        if (currentShape != MOVE) {
                            SaveShape(currentShape, &shapeData);
                            SDL_SetRenderTarget(renderer, NULL);
                            SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
                            SDL_RenderPresent(renderer);
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    // Change cursor if mouse is over a shape
                    if (IsMouseOverShape(event) >= 0) {
                        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));  // Set to hand cursor
                    } else {
                        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));  // Reset to arrow cursor
                    }
                    // Update shape data for drawing preview
                    if (UpdateShapeData(currentShape, &shapeData, event, shapeIndex, renderer, savedBuffer) != NONE) {
                        SDL_RenderCopy(renderer, savedBuffer, NULL, NULL);
                        DrawShape(renderer, currentShape, &shapeData, shapeIndex);
                        SDL_RenderPresent(renderer);
                    }
                    break;
            }
        }
    }
    // Clean up textures when the loop ends
    SDL_DestroyTexture(savedBuffer);
}
