#include"Tower.h"

Tower::Tower(){};
Tower::Tower(Map& gameMap){
    baseCoin = gameMap.getBasePrice();
    iceTowerCoin = gameMap.getIceTowerPrice();
    normalTowerCoin = gameMap.getNormalTowerPrice();
    iceDamage = gameMap.getIceDamage();
    damage = gameMap.getNormalDamage();
    this->iceType = gameMap.getIceName();
    this->normalType = gameMap.getNormalName();
    this->base = gameMap.getBaseName();
};

void Tower::setTower(int posX, int posY, string type, float slowDown){
    const int towerW = 80 ;
    const int towerH = 80;

    //lấy vị trí là tâm của 1 tile
    posX = (int(posX / tileSize) * tileSize + (tileSize / 2)) ;
    posY = (int(posY / tileSize) * tileSize + (tileSize / 5)) ;
    rectTower = {
        posX, posY, towerW, towerH
    };
    this->posX = posX;
    this->posY = posY;
    this->slowDown = slowDown;
    this->type = type;
}

void Tower::render(SDL_Renderer* renderer, Draw& draw) {
    if (type == normalType) {
        filledCircleColor(renderer, posX, posY, towerArea, 0x2c9ae9b3);
        draw.drawTexture(normalType, posX - (rectTower.w / 2), posY - (rectTower.h / 2),
                            rectTower.w, rectTower.h);
    } else if (type == iceType) {
        filledCircleColor(renderer, posX, posY, towerArea, 0x2c9ae9b3);
        //Tăng kích thước cho tháp băng 1 chút
        draw.drawTexture(iceType, posX - (rectTower.w / 2) - 15, posY - (rectTower.h / 2) - 15,
                            rectTower.w + 30 , rectTower.h + 30 );
    }
}

void Tower::shootEnemy(shared_ptr<Enemy> target, Uint32 currentTime) {
    if (!target) return;
    const Uint32 SHOOT_COOLDOWN = 300;  
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



void Tower::updateBullet(Uint32 dT, Map& gameMap) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(dT);
        if (it->isBulletOutScreen()) {
            it = bullets.erase(it);
        }else if (it->isEnemyFired()){
            //trừ speed
            if(type == iceType){
                float newEnemySpeed = (it->getTarget())->getSpeed();
                newEnemySpeed = newEnemySpeed * slowDown;
                (it->getTarget())->setSpeed(newEnemySpeed);

                float hp = (it->getTarget())->getHp();
                hp = hp - iceDamage;
                (it->getTarget())->setHp(hp);
                it = bullets.erase(it);
            }
            // trừ hp
            else if(type == normalType){
                float hp = (it->getTarget())->getHp();
                hp = hp - damage;
                (it->getTarget())->setHp(hp);
                it = bullets.erase(it);
            }

            if((it->getTarget())->getHp() <= 0){
                (it->getTarget())->kill();
                gameMap.setCoin( gameMap.getCoin() + gameMap.getPrize() );
            }
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
}

void Tower::getTowerRect(int &posX, int &posY, int &towerW, int &towerH){
    posX = this->rectTower.x;
    posY = this->rectTower.y;
    towerW = this->rectTower.w;
    towerH = this->rectTower.h;
}

bool Tower::isEnoughCoin(int coin, string towerType){
    if(towerType == iceType){
        return coin >= iceTowerCoin;
    }else if(towerType == base){
        return coin >= baseCoin;
    }

    return coin >= normalTowerCoin;
}

void Tower::payment(Map& gameMap, string towerType){
    if(towerType == iceType){
        gameMap.setCoin(gameMap.getCoin() - iceTowerCoin);
    }else if(towerType == normalType){
        gameMap.setCoin(gameMap.getCoin() - normalTowerCoin);
    }
    else if(towerType == base){
        gameMap.setCoin(gameMap.getCoin() - baseCoin);
    }
}

bool Tower::buyItem(int x, int y){
    if(x >= 5 && y >= 480 && x <= 100 && y <= 570){
        buying = iceType;
        return true;
    }else if(x >= 5 && y >= 580 && x <= 100 && y <= 675){
        buying = normalType;
        return true;
    }
    else if(x >= 5 && y >= 385 && x <= 100 && y <= 470){
        buying = base;
        return true;
    }
    return false;
}

void Tower::resetBullet(){
    bullets.clear();
}
