#pragma once
#include <SDL.h>
#include <vector>
#include <SDL2_gfxPrimitives.h>
#include <memory>  // Để dùng shared_ptr

#include "bullet.h" 
// #include "drawElement.h" 
// #include "Map.h"

using namespace std;

class Tower{
protected:
    SDL_Rect rectTower;
    int posX, posY;
    int towerArea = 200;
    vector<Bullet> bullets;
    float bulletSpeed = 500;
    Uint32 lastShoot = 0;
    string type;
    string iceType;
    string normalType;
    string base;
    float damage;
    float iceDamage;
    int baseCoin;
    int iceTowerCoin;
    int normalTowerCoin;
    string buying;
    float slowDown = 1;
public:
    Tower();
    Tower(Map& gameMap);
    Uint32 getLastShoot() const {return lastShoot;};
    void setTower(int posX, int posY, string type, float slowDown = 1);//type = normal is default
    void getTowerRect(int &posX, int &posY, int &towerW, int &towerH);
    void render(SDL_Renderer* renderer, Draw& draw);
    void shootEnemy(shared_ptr<Enemy> target, Uint32 currentTime);
    void renderBullet(SDL_Renderer* renderer);
    void updateBullet(Uint32 dT, Map& gameMap);
    void removeBulletWithEnemy(shared_ptr<Enemy> destroyedEnemy);
    int getTowerArea() const {return towerArea;};
    bool isEnoughCoin(int coin, string towerType);
    void payment(Map& gameMap, string towerType);
    bool buyItem(int x, int y);
    string getBuying() const {return buying;};
    string geticeType() const {return iceType;};
    string getnormalType() const {return normalType;};
    string getBaseType() const {return base;};
    void resetBullet();
};