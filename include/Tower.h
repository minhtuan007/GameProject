#pragma once
#include<SDL.h>
#include <SDL2_gfxPrimitives.h>


class Tower{
private:
    SDL_Rect rectTower;
    int posX, posY;
    int towerArea = 100;
public:
    void setTower(int posX, int posY);
    void getTowerRect(int &posX, int &posY, int &towerW, int &towerH);
    void render(SDL_Renderer* renderer);
};