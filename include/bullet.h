#pragma once
#include<SDL.h>
#include<cmath>
#include"Enemies.h"
#include<iostream>
#include <memory>
using namespace std;
class Bullet{
private:
    SDL_Rect rectBullet;
    int widthBullet = 5;
    int heightBullet = 15;
    float dirX, dirY;
    float posX, posY;
    float targetX, targetY;
    float speed;
    shared_ptr<Enemy> target = nullptr;
    float damage = 10;
public:
    Bullet(int startX, int startY, float speed, shared_ptr<Enemy> target);
    void update(Uint32 dT);
    void render(SDL_Renderer* renderer);
    bool isBulletOutScreen();
    bool isEnemyFired();
    // float getTargetSpeed() const {return targetSpeed;};
    shared_ptr<Enemy> getTarget(){return target;};
};

