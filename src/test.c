#include "test.h"

void test(SDL_Renderer *renderer) {
    printf("nista 1\n");
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    printf("linija 1\n");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 100, 100, 500, 400);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    // Create a texture for off-screen rendering
    SDL_Texture *saveBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);

    // Read the pixels from the renderer (after the line is drawn) into a surface
    SDL_Surface *screenSurface = SDL_CreateRGBSurfaceWithFormat(0, 800, 600, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, screenSurface->pixels, screenSurface->pitch);

    // Create a texture from the surface
    SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    SDL_FreeSurface(screenSurface);

    // Now copy the screen texture into the saveBuffer
    SDL_SetRenderTarget(renderer, saveBuffer);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    printf("nista 2\n");
    SDL_RenderClear(renderer);  // Clear the window
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    printf("linija kopirana\n");
    // Copy the saveBuffer texture onto the renderer
    SDL_RenderCopy(renderer, saveBuffer, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    // Clean up
    SDL_DestroyTexture(saveBuffer);
    SDL_DestroyTexture(screenTexture);
}
