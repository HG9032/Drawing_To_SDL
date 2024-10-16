#include "test.h"

void test(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = 14;
    rect.y = 53;
    rect.w = 100;
    rect.h = 100;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);
}
