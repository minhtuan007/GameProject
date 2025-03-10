#pragma once

#include <SDL.h>

class Enemy {
private:
    int speed;
    SDL_Rect rect;
    float posX, posY;
    int targetX, targetY;
    float dirX, dirY;
    int spawnX, spawnY;
    int towerArea = 100;
public:    
    // Enemy();       
    Enemy(int targetX, int targetY, float speed = 2.0f); 
    void update(Uint32 dT); 
    void render(SDL_Renderer* renderer); 
    bool comeNearTower(SDL_Rect &rectTower);
    bool isEnemyOutScreen();
};

