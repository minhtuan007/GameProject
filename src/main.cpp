#include<iostream>
#include<cstring>
#include<string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <memory>


#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>

#include "Enemies.h"
#include "Tower.h"
#include "map.h"
#include "bullet.h"

using namespace std;

vector<shared_ptr<Enemy>> enemies;
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
void renderFPS();
string fileAssets(string file);
float enemySpeed = 500;
Uint32 lastShoot = SDL_GetTicks();

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
    const Uint32 TARGET_FPS = 60; // Mục tiêu 60 FPS
    const Uint32 FRAME_TIME = 1000 / TARGET_FPS; // Thời gian mỗi frame (mili-giây)
    Uint32 frameStart, frameTime;

    while (isRunning) {

        frameStart = SDL_GetTicks();

        SDL_RenderClear(renderer);
        // SDL_Delay(30); //16 ~ 60 FPS;

        handleEvents();
        updateFunction();
        renderFunction();
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameTime);
        }
    }
}


bool handleEvents() {
 
    SDL_Event evt;
    while (SDL_PollEvent( &evt)){
        if(evt.type == SDL_QUIT){
            isRunning = false;
            break;
        }
        if(evt.key.keysym.sym == SDLK_F1 && evt.type == SDL_KEYDOWN){
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
    // Sinh kẻ thù với giới hạn tối đa
    const int MAX_ENEMIES = 5; // Giới hạn số lượng kẻ thù
    if (
        enemies.size() < MAX_ENEMIES && 
        rand() % 100 < 2) { // 2% cơ hội sinh kẻ thù mỗi frame
            enemies.push_back(make_shared<Enemy>(
                // 640, 360, 
                enemySpeed)); // Sử dụng std::make_shared
        }

    Uint32 currentTime = SDL_GetTicks();
    Uint32 dT = currentTime - lastTime;
    lastTime = currentTime;

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
        (*enemyIt)->update(dT);

        if ( (*enemyIt)->isEnemyOutScreen() || !((*enemyIt)->isAlive()) ) {
            shared_ptr<Enemy> destroyedEnemy = *enemyIt;
            for(auto &tower : towers){
                tower.removeBulletWithEnemy(destroyedEnemy);
            }
            enemyIt = enemies.erase(enemyIt); // Xóa an toàn với iterator
        } else {
            // Kiểm tra từng tháp
            for (auto& tower : towers) {
                int towerPosX, towerPosY, towerW, towerH;
                tower.getTowerRect(towerPosX, towerPosY, towerW, towerH);
                SDL_Rect rectTower = {towerPosX, towerPosY, towerW, towerH};
                const Uint32 SHOOT_COOLDOWN = 300; // ms giữa các lần bắn
                Uint32 currentTime = SDL_GetTicks();
                if ((*enemyIt)->comeNearTower(rectTower, tower.getTowerArea()) && currentTime - tower.getLastShoot() >= SHOOT_COOLDOWN) {
                    tower.shootEnemy(*enemyIt, currentTime);
                }
            }
            ++enemyIt; // Tăng iterator nếu không xóa
        }
    }
    for (auto& tower : towers) {
        tower.updateBullet(dT);
    }
}
void renderFunction() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    map.renderMap(renderer);

    for (auto& enemy : enemies) {
        (*enemy).render(renderer);  
    }

	for(size_t i = 0; i < towers.size(); i++){
		towers[i].render(renderer);
        towers[i].renderBullet(renderer);
	}

    renderFPS();
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


    map.loadMap(fileAssets("map.txt"), renderer);
    

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

void renderFPS() {
    static Uint32 lastFrame = SDL_GetTicks();
    static int frameCount = 0;
    static float fps = 0.0f;

    frameCount++;
    Uint32 currentFrame = SDL_GetTicks();
    if (currentFrame - lastFrame >= 1000) { // Cập nhật mỗi giây
        fps = frameCount / ((currentFrame - lastFrame) / 1000.0f);
        frameCount = 0;
        lastFrame = currentFrame;
    }

    SDL_Rect dest = {10, 10, 0, 0};
    renderText("FPS: " + to_string(static_cast<int>(fps)), dest);
}