#pragma once
#include <SDL.h>
#include <vector>
#include "bullet.h" 
#include <SDL2_gfxPrimitives.h>
#include <memory>  // Để dùng shared_ptr

using namespace std;

class Tower{
private:
    SDL_Rect rectTower;
    int posX, posY;
    int towerArea = 200;
    vector<Bullet> bullets;
    float bulletSpeed = 500;
    Uint32 lastShoot = 0;
    float damage = 20;
public:
    Uint32 getLastShoot() const {return lastShoot;};
    void setTower(int posX, int posY);
    void getTowerRect(int &posX, int &posY, int &towerW, int &towerH);
    void render(SDL_Renderer* renderer);
    void shootEnemy(shared_ptr<Enemy> target, Uint32 currentTime);
    void renderBullet(SDL_Renderer* renderer);
    void updateBullet(Uint32 dT);
    void removeBulletWithEnemy();
    int getTowerArea() const {return towerArea;};
};