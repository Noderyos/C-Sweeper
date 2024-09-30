#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define WIDTH 9
#define HEIGHT 9
#define BOMBS 5

#define SIZE 80

#define MAP_AT(x, y) map[(y)*WIDTH+(x)]
#define RAND(max) (rand()%(max))

struct Case{
    int bomb:1;
    int reveal:1;
    int value:6;
} map[WIDTH*HEIGHT];

Color colors[] = {WHITE, BLUE, SKYBLUE, GREEN, YELLOW, ORANGE, RED, PURPLE, BROWN};

int count(int cx, int cy){
    int count = 0;
    for(int dy = -1; dy <= 1; dy++){
        for(int dx = -1; dx <= 1; dx++){
            if(cx+dx < 0 || cy+dy < 0 || cx+dx >= WIDTH || cy+dy >= HEIGHT) continue;
            if(dx == 0 && dy == 0) continue;
            if(MAP_AT(cx+dx, cy+dy).bomb) count++;
        }
    }
    return count;
}

void reveal(int x, int y){
    if(MAP_AT(x, y).reveal) return;
    MAP_AT(x, y).reveal = 1;
    if(MAP_AT(x, y).value == 0){
        if(x > 0) reveal(x-1, y);
        if(y > 0) reveal(x, y-1);
        if(x < WIDTH-1) reveal(x+1, y);
        if(y < HEIGHT-1) reveal(x, y+1);
    }
}
int main(void){
    srand(time(NULL));
    int i = 0;
    while(i < BOMBS){
        int x = RAND(WIDTH);
        int y = RAND(HEIGHT);
        if(!MAP_AT(x, y).bomb){
            MAP_AT(x, y).bomb = 1;
            i++;
        }
    }

    for(int y = 0; y < HEIGHT; y++)
        for(int x = 0; x < WIDTH; x++)
            if(!MAP_AT(x, y).bomb) MAP_AT(x, y).value = count(x, y);


    InitWindow(WIDTH*SIZE, HEIGHT*SIZE, "Mine");
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int x = GetMouseX()/SIZE;
            int y = GetMouseY()/SIZE;
            if(MAP_AT(x, y).bomb) MAP_AT(x, y).reveal = 1;
            else reveal(x, y);
        }

        for(int y = 0; y < HEIGHT; y++){
            for(int x = 0; x < WIDTH; x++){
                struct Case c = MAP_AT(x, y);
                if(c.reveal){
                    if(c.bomb){
                        char text[2] = "B";
                        DrawText(text, x*SIZE, y*SIZE, SIZE, RED);
                    }else if(c.value != 0){
                        char text[2] = {c.value + '0', 0};
                        DrawText(text, x*SIZE, y*SIZE, SIZE, colors[c.value]);
                    }
                }else {
                    DrawRectangle(x*SIZE + 2, y*SIZE + 2, SIZE - 4, SIZE - 4, GRAY);
                }
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}


