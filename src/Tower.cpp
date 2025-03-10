#include<tower.h>
#include<iostream>
using namespace std;

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

void Tower::getTowerRect(int &posX, int &posY, int &towerW, int &towerH){
    posX = this->rectTower.x;
    posY = this->rectTower.y;
    towerW = this->rectTower.w;
    towerH = this->rectTower.h;

}
