#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

#define WIDTH 9
#define HEIGHT 9
#define BOMBS 10

#define SIZE 80

#define MAP_AT(x, y) map[(y)*WIDTH+(x)]
#define RAND(max) (rand()%(max))

typedef unsigned int u8;

struct Case{
    u8 bomb:1;
    u8 reveal:1;
    u8 flag:1;
    u8 value:5;
} map[WIDTH*HEIGHT];

Color colors[] = {WHITE, BLUE, SKYBLUE, GREEN, YELLOW, ORANGE, RED, PURPLE, BROWN};

int count(int cx, int cy){
    int count = 0;
    for(int dy = -1; dy <= 1; dy++){
        for(int dx = -1; dx <= 1; dx++){
            if(cx+dx < 0 || cy+dy < 0 || cx+dx >= WIDTH || cy+dy >= HEIGHT) continue;
            if(dx == 0 && dy == 0) continue;
            count += MAP_AT(cx+dx, cy+dy).bomb;
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

void init(){
    memset(map, 0, WIDTH*HEIGHT*sizeof(struct Case));
    srand(time(NULL));
    int i = 0;
    while(i < BOMBS){
        int x = rand() % WIDTH,
            y = rand() % HEIGHT;
        if(!MAP_AT(x, y).bomb){
            MAP_AT(x, y).bomb = 1;
            i++;
        }
    }
    for(int y = 0; y < HEIGHT; y++)
        for(int x = 0; x < WIDTH; x++)
            if(!MAP_AT(x, y).bomb) MAP_AT(x, y).value = count(x, y);
}

int main(void){
    init();

    InitWindow(WIDTH*SIZE, HEIGHT*SIZE, "Mine");

    int started = 0;
    double start_time = 0, end_time = 0;
    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_R)){
            init();
            started = 0;
        }

        int count = 0, lost = 0;
        for(int y = 0; y < HEIGHT; y++){
            for(int x = 0; x < WIDTH; x++){
                count += !MAP_AT(x, y).reveal;
                lost |= MAP_AT(x, y).bomb && MAP_AT(x, y).reveal;
            }
        }


        if(!lost && count > BOMBS){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                int x = GetMouseX()/SIZE;
                int y = GetMouseY()/SIZE;
                if(!MAP_AT(x, y).flag){
                    if(MAP_AT(x, y).bomb) MAP_AT(x, y).reveal = 1;
                    else reveal(x, y);
                }
                if(!started){
                    started = 1;
                    start_time = GetTime();
                }
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                int x = GetMouseX()/SIZE;
                int y = GetMouseY()/SIZE;
                if(MAP_AT(x, y).reveal) continue;
                MAP_AT(x, y).flag = !MAP_AT(x, y).flag;
            }
        }else if(started){
            started = 0;
            end_time = GetTime();
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        for(int y = 0; y < HEIGHT; y++){
            for(int x = 0; x < WIDTH; x++){
                struct Case c = MAP_AT(x, y);
                if(c.reveal){
                    DrawRectangle(x*SIZE + 2, y*SIZE + 2, SIZE - 4, SIZE - 4, BLACK);
                    if(c.bomb){
                        DrawText("B", x*SIZE, y*SIZE, SIZE, RED);
                    }else if(c.value != 0){
                        DrawText(TextFormat("%c", c.value + '0'), x*SIZE, y*SIZE, SIZE, colors[c.value]);
                    }
                }else{
                    DrawRectangle(x*SIZE + 2, y*SIZE + 2, SIZE - 4, SIZE - 4, GRAY);
                    if(c.flag) DrawText("F", x*SIZE, y*SIZE, SIZE, RED);
                }
            }
        }
        if(lost){
            DrawText("YOU LOST", 10, 10, 150, RED);
        }else if(count == BOMBS){
            DrawText("YOU WON", 10, 10, 150, GREEN);
            DrawText(TextFormat("in %.3f", end_time - start_time), 10, 200, 150, GREEN);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}


