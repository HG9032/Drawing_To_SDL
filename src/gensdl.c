#include "gensdl.h"

void generate_SDL_code(){
    FILE *file;

    file = fopen("generated_code.txt", "w");
    if(!file){
        printf("File creation failed.\n");
        fclose(file);
    }
    else{
        freopen("generated_code.txt", "a", file);

        const char *renderer = "SDL_Renderer *renderer = {your renderer};\n"
                                "SDL_Rect rectangleStructure;\n"
                                "SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);\n"
                                "SDL_RenderClear(renderer);\n"
                                "SDL_RenderPresent(renderer);\n"
                                "SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);\n\n";

        fwrite(renderer, sizeof(char), strlen(renderer), file);

        const char *circle_function = "void drawCircle(int startX, int startY, int radius){\n"
                                        "for(int y = -radius; y <= radius; y++){\n"
                                        "\tfor(int x = -radius; x <= radius; x++){\n"
                                        "\t\tif(abs((x * x + y * y) - radius * radius) <= radius/1.08) SDL_RenderDrawPoint(renderer, startX + x, startY + y);\n"
                                        "\t\t}\n"
                                        "\t}\n"
                                        "}\n\n";

        fwrite(circle_function, sizeof(char), strlen(circle_function), file);

        for(int i = 0; i < 10; i++){
            switch(arrayOfShapes[i].shape){
                case LINE:{
                    int startX = arrayOfShapes[i].data.line.startX;
                    int startY = arrayOfShapes[i].data.line.startY;
                    int endX = arrayOfShapes[i].data.line.endX;
                    int endY = arrayOfShapes[i].data.line.endY;

                    char *line_code[100];
                    sprintf(line_code, "SDL_RenderDrawLine(renderer, %d, %d, %d, %d);\n", startX, startY, endX, endY);

                    fwrite(line_code, sizeof(char), strlen(line_code), file);
                    break;
                }
                case RECTANGLE:{
                    int startX = arrayOfShapes[i].data.rectangle.x;
                    int startY = arrayOfShapes[i].data.rectangle.y;
                    int width = arrayOfShapes[i].data.rectangle.w;
                    int height = arrayOfShapes[i].data.rectangle.h;

                    char *rectangle_code[100];
                    sprintf(rectangle_code, "rectangleStructure.x = %d;\n"
                                            "rectangleStructure.y = %d;\n"
                                            "rectangleStructure.w = %d;\n"
                                            "rectangleStructure.h = %d;\n"
                                            "SDL_RenderDrawRect(renderer, &rectangleStructure);\n\n", startX, startY, width, height);

                    fwrite(rectangle_code, sizeof(char), strlen(rectangle_code), file);
                }
                    break;
                case CIRCLE:{
                    int startX = arrayOfShapes[i].data.circle.startX;
                    int startY = arrayOfShapes[i].data.circle.startY;
                    int radius = arrayOfShapes[i].data.circle.radius;
                    const char *drawCircle[100];
                    sprintf(drawCircle, "drawCircle(%d, %d, %d);\n", startX, startY, radius);

                    fwrite(drawCircle, sizeof(char), strlen(drawCircle), file);
                    break;
                }
            }
        }

        const char *asd = "\nSDL_RenderPresent(renderer);\n";
        fwrite(asd, sizeof(char), strlen(asd), file);

        fclose(file);
    }
}
