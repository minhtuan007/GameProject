#pragma once
#include <SDL.h>
#include <vector>
#include "bullet.h" 
#include "drawElement.h" 
#include "Map.h" 
#include <SDL2_gfxPrimitives.h>
#include <memory>  // Để dùng shared_ptr

using namespace std;

class Tower{
protected:
    SDL_Rect rectTower;
    int posX, posY;
    int towerArea = 200;
    vector<Bullet> bullets;
    float bulletSpeed = 500;
    Uint32 lastShoot = 0;
    float damage = 20;
    string type;
    string iceType = "ice";
    string normalType = "normal";
    int iceTowerCoin = 80;
    int normalTowerCoin = 50;
    string buying;
    float slowDown = 1;
public:
    Tower();
    Uint32 getLastShoot() const {return lastShoot;};
    void setTower(int posX, int posY, float slowDown = 1, string type = "normal");
    void getTowerRect(int &posX, int &posY, int &towerW, int &towerH);
    void render(SDL_Renderer* renderer, Draw& draw);
    void shootEnemy(shared_ptr<Enemy> target, Uint32 currentTime);
    void renderBullet(SDL_Renderer* renderer);
    void updateBullet(Uint32 dT, Map& gameMap);
    void removeBulletWithEnemy(shared_ptr<Enemy> destroyedEnemy);
    int getTowerArea() const {return towerArea;};
    bool isEnoughCoin(int coin, string towerType);
    void payment(Map& gameMap, string towerType);
    bool buyTower(int x, int y);
    string getBuying() const {return buying;};
    string geticeType() const {return iceType;};
    string getnormalType() const {return normalType;};
    void resetBullet();
};