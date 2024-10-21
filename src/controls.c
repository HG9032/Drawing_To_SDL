#include "controls.h"

SDL_Window *ControlWindow = NULL;
SDL_Renderer *ControlRenderer = NULL;

Uint32 InitializeControlWindow(){
    // Create Control Window
    ControlWindow = SDL_CreateWindow("Controls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_SHOWN);
    if(!ControlWindow){
        printf("Control Window creation failed. Error: %s\n", SDL_GetError());
        return 0;
    }

    // Create Control Renderer
    ControlRenderer = SDL_CreateRenderer(ControlWindow, -1, 0);
    if(!ControlRenderer){
        printf("Control Renderer creation failed. Error: %s\n", SDL_GetError());
        return 0;
    }

    DrawControls();

    return SDL_GetWindowID(ControlWindow);
}

void drawCircle(int startX, int startY, int radius){
    for(int y = -radius; y <= radius; y++){
        for(int x = -radius; x <= radius; x++){
            if(abs((x * x + y * y) - radius * radius) <= radius/1.08) SDL_RenderDrawPoint(ControlRenderer, startX + x, startY + y);
            }
        }
    }

void DrawControls(){
    SDL_Rect rectangleStructure;
    SDL_SetRenderDrawColor(ControlRenderer, 255, 255, 255, 255);
    SDL_RenderClear(ControlRenderer);
    SDL_RenderPresent(ControlRenderer);
    SDL_SetRenderDrawColor(ControlRenderer, 0, 0, 0, 255);

    // Border lines
    SDL_RenderDrawLine(ControlRenderer, 1, 60, 215, 59);    // Horizontal Line
    SDL_RenderDrawLine(ControlRenderer, 54, 60, 54, -34);   // First Vertical Line
    SDL_RenderDrawLine(ControlRenderer, 130, 59, 131, -65); // Second Vertical Line


    // Control drawings
    SDL_RenderDrawLine(ControlRenderer, 13, 35, 39, 18);    // Draw Line

    drawCircle(161, 27, 15);    // Draw Circle

    // Rectangle structure
    rectangleStructure.x = 79;
    rectangleStructure.y = 14;
    rectangleStructure.w = 29;
    rectangleStructure.h = 28;

    SDL_RenderDrawRect(ControlRenderer, &rectangleStructure);   // Draw Rectangle With Previously Defined Structure

    SDL_RenderPresent(ControlRenderer);

}

SHAPE ChangeControl(SDL_Event *event){
    int mX = event->motion.x;  // Get mouse X position from event
    int mY = event->motion.y;  // Get mouse Y position from event
    bool isDown = (event->type == SDL_MOUSEBUTTONDOWN);
    bool isButton = (event->button.button == SDL_BUTTON_LEFT);

    if (isDown && isButton) {
        if (mY >= 0 && mY <= 60) {
            if (mX >= 0 && mX <= 54) return LINE;
            else if (mX >= 54 && mX <= 130) return RECTANGLE;
            else if (mX >= 130) return CIRCLE;
        }
    }
    else return LINE;
}

void ControlCleanUp(){
    if(ControlRenderer) SDL_DestroyRenderer(ControlRenderer);
    if(ControlWindow) SDL_DestroyWindow(ControlWindow);
}
