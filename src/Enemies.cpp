#include "Enemies.h"

using namespace std;
Enemy::Enemy(string enemyPathFile1, string enemyPathFile2, Map& gameMap) {
   
    this->initSpeed = gameMap.getInitEnemySpeed();
    currentSpeed = initSpeed;
    hp = gameMap.getInitEnemyHP();
    
    int enemyW = 90;
    int enemyH = 90;

    isPath1 = true;
    if(rand() % 2){
        isPath1 = false;
    }
    ifstream file;
    if(isPath1){
        file.open(enemyPathFile1);        
    }else{
        file.open(enemyPathFile2);
    }
    if (!file) {
        cerr << "Không thể mở file!\n";
        return;
    }
    file >> this->spawnX >> this->spawnY;
    this->spawnX = this->spawnX * 80 - 15;
    this->spawnY = this->spawnY * 80 - 15;
    posPath target;
        while (file >> target.x >> target.y){
            target.x = target.x * 80 - 15;
            target.y = target.y * 80 - 15;
            path.push_back(target);
        }
    file.close();


    posX = this->spawnX;
    posY = this->spawnY;
    rect.x = this->spawnX;
    rect.y = this->spawnY;
    rect.w = enemyW;
    rect.h = enemyH;
}

void Enemy::update(Uint32 dT, float& fortressHP) {
    float deltaX = path[pathIndex].x - rect.x;
    float deltaY = path[pathIndex].y - rect.y;
    float length = sqrtf(deltaX * deltaX + deltaY * deltaY);

    if(length < 5.0f && pathIndex < int(path.size())){
        pathIndex++;
    }
    if(pathIndex == int(path.size()) && !startExploding){
        fortressHP -= enemyDamage;
        explode = true;
        currentSpeed = 0;
        return;
    }
    deltaX = path[pathIndex].x - rect.x;
    deltaY = path[pathIndex].y - rect.y;
    length = sqrtf(deltaX * deltaX + deltaY * deltaY);

    if(length != 0){
        dirX = deltaX / length;
        dirY = deltaY / length;
    }else{
        dirX = 0;
        dirY = 0;
    }


    float timeInSec = dT / 1000.f;
    posX += currentSpeed * timeInSec * dirX; 
    posY += currentSpeed * timeInSec * dirY; 
    if(rect.x > static_cast<int>(posX)){
        forward = false;
    }
    if(rect.x <= static_cast<int>(posX)){
        forward = true;
    }
    rect.x = static_cast<int>(posX);
    rect.y = static_cast<int>(posY);
    
}

void Enemy::render(Uint32 runningTime, Draw& draw) {
    int charX = rect.x;
    int charY = rect.y;
    int charW = rect.h;
    int charH = rect.w;
    runningTime = runningTime / 100;
    if(forward && !explode){
        switch (runningTime % 6)
        {
        case 0:
            draw.drawTexture("bomb_character_o_walk_1", charX, charY, charW, charH);
            break;
        case 1:
            draw.drawTexture("bomb_character_o_walk_2", charX, charY, charW, charH);
            break;
        case 2:
            draw.drawTexture("bomb_character_o_walk_3", charX, charY, charW, charH);
            break;
        case 3:
            draw.drawTexture("bomb_character_o_walk_4", charX, charY, charW, charH);
            break;
        case 4:
            draw.drawTexture("bomb_character_o_walk_5", charX, charY, charW, charH);
            break;
        
        default:
            draw.drawTexture("bomb_character_o_walk_6", charX, charY, charW, charH);
            break;
        }
    }
    else if(!forward && !explode){
        switch (runningTime % 6)
        {
        case 0:
            draw.drawTexture("bomb_character_o_walk_flip_1", charX, charY, charW, charH);
            break;
        case 1:
            draw.drawTexture("bomb_character_o_walk_flip_2", charX, charY, charW, charH);
            break;
        case 2:
            draw.drawTexture("bomb_character_o_walk_flip_3", charX, charY, charW, charH);
            break;
        case 3:
            draw.drawTexture("bomb_character_o_walk_flip_4", charX, charY, charW, charH);
            break;
        case 4:
            draw.drawTexture("bomb_character_o_walk_flip_5", charX, charY, charW, charH);
            break;
        
        default:
            draw.drawTexture("bomb_character_o_walk_flip_6", charX, charY, charW, charH);
            break;
        }
    }else if(explode){
        startExploding = true;
        const int ANIMATION_SPEED = 80;

        // Khởi tạo lastUpdate khi mới bắt đầu nổ
        if (lastUpdate == 0) {
            lastUpdate = runningTime * 100; // Chuyển lại về ms
        }

        // Tính thời gian trôi qua kể từ lần cập nhật cuối
        explodeTimer = runningTime * 100 - lastUpdate;
        int frameIndex = explodeTimer / ANIMATION_SPEED;

        // Hiển thị frame theo thứ tự
        if (frameIndex == 0) {
            draw.drawTexture("bomb_character_o_explode_1", charX, charY, charW, charH);
        }
        else if (frameIndex == 1) {
            draw.drawTexture("bomb_character_o_explode_2", charX, charY, charW, charH);
        }
        else if (frameIndex >= 2) {
            draw.drawTexture("bomb_character_o_explode_3", charX, charY, charW, charH);
            alive = false;
            explode = false;
            explodeTimer = 0; 
            lastUpdate = 0;
        }
    }
}

bool Enemy::comeNearTower(SDL_Rect &rectTower, int towerArea) {
    int enemyX = rect.x + rect.w / 2;
    int enemyY = rect.y + rect.h / 2;
    int towerPosX = rectTower.x;
    int towerPosY = rectTower.y;
    float distanceX = abs(enemyX - towerPosX);
    float distanceY = abs(enemyY - towerPosY);


    float distance = sqrt(distanceX * distanceX + distanceY * distanceY);
    return (distance < towerArea);
}

bool Enemy::isEnemyOutScreen(){
    
        if( rect.x < -15 || rect.x > 1290 || rect.y < -15 || rect.y > 730){
            cout << "out" << endl;
            cout << rect.x << " " << rect.y << endl;
            return true;
        }
        return false;
}

