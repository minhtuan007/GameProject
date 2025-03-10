#include<iostream>
#include<cstring>
#include<string>
#include <vector>
#include <ctime>
#include <cstdlib>


#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>

#include "Enemies.h"
#include "Tower.h"
#include "map.h"

using namespace std;

vector<Enemy> enemies;
vector<Tower> towers;
Map map;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
bool isRunning = true;

bool init();
void GameIsRunning();
void kill();
bool handleEvents();
void updateFunction();
void renderFunction();
void renderText(string text, SDL_Rect destination);
string fileAssets(string file);

int main(int argc, char** argv){
    if(!init()){
        system("pause");
        return -1;
    }

    srand(time(0));

    GameIsRunning();

    cout << "GameOver!!!" << endl;
    kill();
    return 0;
}


void GameIsRunning() {

    while (isRunning) {

        SDL_RenderClear(renderer);
        // SDL_Delay(30); //16 ~ 60 FPS;

        handleEvents();
        updateFunction();
        renderFunction();

        SDL_RenderPresent(renderer);
    }
}



bool handleEvents() {
 
    SDL_Event evt;
    while (SDL_PollEvent( &evt)){
        if(evt.type == SDL_QUIT){
            isRunning = false;
            return false;
        }
        if(evt.key.keysym.sym == SDLK_F1){
            system("pause");
        }
		if(evt.type == SDL_MOUSEBUTTONDOWN){
			Tower tower;
			tower.setTower(evt.button.x, evt.button.y);
			towers.push_back(tower);
		}
    }
    return true;
}


Uint32 lastTime = SDL_GetTicks();
void updateFunction() {

    if (rand() % 100 < 20) { // % cơ hội spawn enemy mỗi frame
        enemies.push_back(Enemy(640, 360, 1)); 
    }
    Uint32 currentTime = SDL_GetTicks();
    Uint32 dT = (currentTime - lastTime); 
    lastTime = currentTime;

    int towerPosX = 0, towerPosY = 0, towerW = 0, towerH = 0;
    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i].update(dT);

        //ra ngoài màn hình
        if (enemies[i].isEnemyOutScreen()) {
            enemies.erase(enemies.begin() + i); 
            i--; 
        }

        for(size_t indexTower = 0; indexTower < towers.size(); indexTower++){

            //Bị tiêu diệt khi vào vùng hoạt động của tháp canh
            towers[indexTower].getTowerRect(towerPosX, towerPosY, towerW, towerH);
            SDL_Rect rectTower = {towerPosX, towerPosY, towerW, towerH};
            if(enemies[i].comeNearTower(rectTower)){
                enemies.erase(enemies.begin() + i);
                i--;
            }
        }
    }
}

void renderFunction() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    map.renderMap(renderer);

    for (auto& enemy : enemies) {
        enemy.render(renderer);  
    }

	for(size_t i = 0; i < towers.size(); i++){
		towers[i].render(renderer);
	}
}



bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cerr << "Khong the khoi tao SDL: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
    if (!window) {
        cerr << "Khong the tao window: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Khong the tao renderer: " << SDL_GetError() << endl;
        return false;
    }

    if (TTF_Init() < 0) {
        cerr << "Khong the khoi tao ttf: " << TTF_GetError() << endl;
        return false;
    }
    font = TTF_OpenFont(fileAssets("tahoma.ttf").c_str(), 30);
    if (!font) {
        cerr << "Khong the load font chu: " << TTF_GetError() << endl;
        return false;
    }
    
    if(IMG_Init(IMG_INIT_PNG) < 0){
        cerr << "Khong the khoi tao anh: " << IMG_GetError() << endl;
        return false;
    }


    map.getMap(fileAssets("map.txt"), fileAssets("outputMap.txt"));
    

    return true;
}



void kill(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    font = NULL;
    renderer = NULL;
    window = NULL;
    TTF_Quit();
    SDL_Quit();
}



void renderText(string text, SDL_Rect destination){
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSur = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSur);
    destination.w = textSur->w;
    destination.h = textSur->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &destination);
    SDL_FreeSurface(textSur);
    SDL_DestroyTexture(textTexture);
    textTexture = NULL;
    textSur = NULL;

}

string fileAssets(string file){
    return "D:/laptrinh/LTNC/Code/GameProject/assets/" + file;
}