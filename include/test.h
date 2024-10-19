#ifndef TEST_H
#define TEST_H

#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Cursor* LoadCustomCursorer(SDL_Renderer *renderer, const char *imagePath){
        SDL_Surface *cursorSurface = SDL_LoadBMP(imagePath);
    if (!cursorSurface) {
        printf("Failed to load cursor image: %s\n", SDL_GetError());
        return NULL;
    }

    // Create a color cursor from the surface
    SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    SDL_FreeSurface(cursorSurface);  // Free the surface after creating the cursor
    return cursor;
}



#endif // TEST_H
