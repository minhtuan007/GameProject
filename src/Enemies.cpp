#include "Enemies.h"
#include <cmath>
#include <iostream>
using namespace std;
Enemy::Enemy(int targetX, int targetY, float speed) {
    
    this->targetX = targetX;
    this->targetY = targetY;
    this->speed = speed;

    int centerX = 640;
    int centerY = 360;
    int halfLength = 100;
    int enemyW = 40;
    int enemyH = 40;

    // this->spawnX = 540;
    // this->spawnY = 360;
    

    // do {
        
    //     this->spawnX = rand() % 1280;
    //     this->spawnY = rand() % 720;
    // } while (   this->spawnX > centerX - halfLength && this->spawnY > centerY - halfLength &&
    //             this->spawnX < centerX + halfLength && this->spawnY < centerY + halfLength );
    this->spawnX = 0;
    this->spawnY = 1 * 80 + 20;
    if(rand() % 2){
        this->spawnY = 2 * 80 + 20;
    }


    posX = this->spawnX;
    posY = this->spawnY;
    rect.x = this->spawnX;
    rect.y = this->spawnY;
    rect.w = enemyW;
    rect.h = enemyH;

    float deltaX = targetX - this->spawnX;
    float deltaY = targetY - this->spawnY;
    float length = sqrt(deltaX * deltaX + deltaY * deltaY);

    if(length != 0){
        dirX = deltaX / length;
        dirY = deltaY / length;
    }else{
        dirX = 0;
        dirY = 0;
    }
}

void Enemy::update(Uint32 dT) {
    float timeInSec = dT / 1000.f;
    posX += speed * timeInSec * dirX; 
    posY += speed * timeInSec * dirY; 
    rect.x = static_cast<int>(posX);
    rect.y = static_cast<int>(posY);
    
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Enemy::comeNearTower(SDL_Rect &rectTower, int towerArea) {
    int enemyX = rect.x + rect.w / 2;
    int enemyY = rect.y + rect.h / 2;

    int towerPosX = rectTower.x + rectTower.w / 2;
    int towerPosY = rectTower.y + rectTower.h / 2;
    float distanceX = abs(enemyX - towerPosX);
    float distanceY = abs(enemyY - towerPosY);


    float distance = sqrt(distanceX * distanceX + distanceY * distanceY);
    return (distance < towerArea);
}

bool Enemy::isEnemyOutScreen(){
        return rect.x < -10 || rect.x > 1290 || rect.y < -10 || rect.y > 730;
}

