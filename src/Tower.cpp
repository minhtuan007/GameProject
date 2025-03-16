#include"Tower.h"

void Tower::setTower(int posX, int posY){
    const int towerW = 20 ;
    const int towerH = 40;
    rectTower = {
        posX - (towerW / 2), posY - (towerH / 2), // Spawn tháp sao cho con trỏ chuột trùng tâm tháp
        towerW, towerH
    };
    this->posX = posX;
    this->posY = posY;
}

void Tower::render( SDL_Renderer *renderer){
    filledCircleColor(renderer, posX, posY, towerArea, 0x11BDF1CC);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rectTower);
}

void Tower::shootEnemy(shared_ptr<Enemy> target, Uint32 currentTime) {
    if (!target) return;
    const Uint32 SHOOT_COOLDOWN = 300;  // ms giữa các lần bắn
    if (currentTime - lastShoot >= SHOOT_COOLDOWN) {
        int posBulletX = rectTower.x + rectTower.w / 2;
        int posBulletY = rectTower.y;
        bullets.emplace_back(posBulletX, posBulletY, bulletSpeed, target);
        lastShoot = currentTime;
    }
}

void Tower::renderBullet(SDL_Renderer* renderer){
    for(auto& bullet : bullets){
        bullet.render(renderer);
    }
}



void Tower::updateBullet(Uint32 dT) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(dT);
        if (it->isBulletOutScreen()) {
            it = bullets.erase(it);
        }else if (it->isEnemyFired()){
            float hp = (it->getTarget())->getHp();
            hp = hp - damage;
            (it->getTarget())->setHp(hp);
            if((it->getTarget())->getHp() <= 0){
                (it->getTarget())->kill();
            }
            it = bullets.erase(it);
        } 
        else {
            ++it;
        }
    }
}

void Tower::removeBulletWithEnemy(shared_ptr<Enemy> destroyedEnemy) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        if (it->getTarget() == destroyedEnemy) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    // bullets.clear();
}

void Tower::getTowerRect(int &posX, int &posY, int &towerW, int &towerH){
    posX = this->rectTower.x;
    posY = this->rectTower.y;
    towerW = this->rectTower.w;
    towerH = this->rectTower.h;
}
