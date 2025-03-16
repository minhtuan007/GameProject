#pragma once
#include <SDL.h>
// #include "Tower.h"

class Enemy {
private:
    float speed;
    SDL_Rect rect;
    float posX, posY;
    int targetX, targetY;
    float dirX, dirY;
    int spawnX, spawnY;
    float hp = 100;
    bool alive = true;
public:    
    // Enemy();       
    Enemy(int targetX, int targetY, float speed = 2.0f); 
    void update(Uint32 dT); 
    void render(SDL_Renderer* renderer); 
    bool comeNearTower(SDL_Rect &rectTower, int towerArea);
    bool isEnemyOutScreen();
    int getPosX(){return this->rect.x;};
    int getPosY(){return this->rect.y;};
    int getPosW(){return this->rect.w;};
    int getPosH(){return this->rect.h;};
    float getHp(){return this->hp;};
    void  setHp(float newHp){this->hp = newHp;};
    bool  isAlive(){return alive;};
    void  kill(){alive = false;};
};

