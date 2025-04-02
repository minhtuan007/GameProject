#pragma once
#include <SDL.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

// #include "drawElement.h" 
#include "map.h"
using namespace std;
class Enemy {
private:
    SDL_Rect rect;
    float posX, posY;
    int targetX, targetY;
    float dirX, dirY;
    int spawnX, spawnY;
    float hp;
    bool alive = true;
    bool isPath1 = true;
    struct posPath{
        int x;
        int y;
    };
    vector<posPath> path;
    int pathIndex = 0;
    float initSpeed;
    float currentSpeed = 0;
    bool forward;
    float enemyDamage = 20;
    bool explode = false;
    bool startExploding = false;
    int exploreFrame = 0;
    Uint32 explodeTimer = 0; // Biến thành viên để đếm thời gian nổ
    Uint32 lastUpdate = 0; // Biến thành viên để lưu thời điểm cập nhật cuối
public:    
    // Enemy();       
    Enemy(string enemyPathFile1, string enemyPathFile2, Map& gameMap); 
    void update(Uint32 dT, float& fortressHP); 
    void render(Uint32 runningTime, Draw& draw); 
    bool comeNearTower(SDL_Rect &rectTower, int towerArea);
    bool isEnemyOutScreen();
    int getPosX(){return this->rect.x;};
    int getPosY(){return this->rect.y;};
    int getPosW(){return this->rect.w;};
    int getPosH(){return this->rect.h;};
    float getHp(){return this->hp;};
    void  setHp(float newHp){this->hp = newHp;};
    float getSpeed(){return this->initSpeed;};
    void  setSpeed(float newSpeed){this->currentSpeed = newSpeed;};
    bool  isAlive(){return alive;};
    void  kill(){alive = false;};
};

