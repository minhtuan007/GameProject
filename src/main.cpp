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
#include "drawElement.h"

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
TTF_Font* font15;
bool isRunning = true;
string selectedTowerType = "";
float slowDown = 1;

vector<shared_ptr<Enemy>> enemies;
vector<Tower> towers;
Map gameMap;
Draw draw;
float maxFortressHP = gameMap.getFortressHP();
bool gameOver = false;
int selectedOption = 0;




bool init();
void GameIsRunning();
void kill();
bool handleEvents();
void updateFunction();
void renderFunction();
void renderFPS();
void resetGame();
string fileAssets(string file);
Uint32 lastShoot = SDL_GetTicks();

int main(int argc, char** argv) {
    if (!init()) {
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
    const Uint32 TARGET_FPS = 60;
    const Uint32 FRAME_TIME = 1000 / TARGET_FPS;
    Uint32 frameStart, frameTime;

    while (isRunning) {
        frameStart = SDL_GetTicks();
        SDL_RenderClear(renderer);

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
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            isRunning = false;
            break;
        }
        if (evt.key.keysym.sym == SDLK_F1 && evt.type == SDL_KEYDOWN) {
            system("pause");
        }
        if (evt.key.keysym.sym == SDLK_n) {
            cout << "n" << endl;
        }
        if (evt.type == SDL_MOUSEBUTTONDOWN  && !gameOver) {
            int tempX = evt.button.x;
            int tempY = evt.button.y;
            Tower tower;
            cout<< tempX << " " << tempY<<endl;
            cout<<draw.getIsVisible()<<endl;
            if(draw.clickListBtn(tempX, tempY)){
                draw.toggleIsVisible();
            }
            else if(tower.buyTower(tempX, tempY) && draw.getIsVisible()){

                if(tower.isEnoughCoin(gameMap.getCoin(), tower.getBuying())){
                    selectedTowerType = tower.getBuying();
                    if(selectedTowerType == tower.geticeType()){
                        slowDown = 0.5;
                        // draw.drawTexture("iceTower", currentX, currentY, 80, 80);
                    }else if(selectedTowerType == tower.getnormalType()){
                        // draw.drawTexture("normal", currentX, currentY, 80, 80);
                    }
                    cout<<"da mua" <<endl;
                }
                else{
                    cout << "Khong du vang" <<endl;
                    selectedTowerType = "";
                }                
            }
            if (gameMap.hasTower(tempX, tempY) == 0 && gameMap.isValidToSet(tempX, tempY)) {
                if (evt.button.button == SDL_BUTTON_LEFT) {
                    if(selectedTowerType != ""){
                        tower.setTower(tempX, tempY, slowDown, selectedTowerType);
                        towers.push_back(tower);
                        gameMap.setUsedTile(tempX, tempY);
                        tower.payment(gameMap, selectedTowerType);
                        selectedTowerType = "";
                        slowDown = 1;
                    }
                    else if(selectedTowerType == ""){
                        cout << "Please, pick a tower type" <<endl;
                    }
                }
            }
        }

        if(gameOver && evt.type == SDL_KEYDOWN){
            switch (evt.key.keysym.sym) {
                case SDLK_LEFT:
                case SDLK_UP:
                    selectedOption = 0; // Try Again
                    break;
                case SDLK_RIGHT:
                case SDLK_DOWN:
                    selectedOption = 1; // Menu
                    break;
                case SDLK_RETURN: // Phím Enter để xác nhận
                    if (selectedOption == 0) { // Try Again: Reset trạng thái
                        resetGame();
                    } 
                    // else { // menu: Thoát game
                    //     isRunning = false;
                    // }
                    break;
            }
        }
    }
    return true;
}

Uint32 lastTime = SDL_GetTicks();
Uint32 runningTime = 0;
float fortressHP = gameMap.getFortressHP();
void updateFunction() {
    if(!gameOver){
        const int MAX_ENEMIES = 5;
        if (enemies.size() < MAX_ENEMIES && rand() % 100 < 2) {
            enemies.push_back(make_shared<Enemy>());
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 dT = currentTime - lastTime;
        runningTime += dT;
        lastTime = currentTime;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            (*enemyIt)->update(dT, fortressHP);
            gameMap.setFortressHP(fortressHP);
            if ((*enemyIt)->isEnemyOutScreen() || !((*enemyIt)->isAlive())) {
                shared_ptr<Enemy> destroyedEnemy = *enemyIt;
                for (auto &tower : towers) {
                    tower.removeBulletWithEnemy(destroyedEnemy);
                }
                enemyIt = enemies.erase(enemyIt);
            } else {
                for (auto& tower : towers) {
                    int towerPosX, towerPosY, towerW, towerH;
                    tower.getTowerRect(towerPosX, towerPosY, towerW, towerH);
                    SDL_Rect rectTower = {towerPosX, towerPosY, towerW, towerH};
                    const Uint32 SHOOT_COOLDOWN = 300;
                    Uint32 currentTime = SDL_GetTicks();
                    if ((*enemyIt)->comeNearTower(rectTower, tower.getTowerArea()) && currentTime - tower.getLastShoot() >= SHOOT_COOLDOWN) {
                        tower.shootEnemy(*enemyIt, currentTime);
                    }
                }
                ++enemyIt;
            }
        }
        for (auto& tower : towers) {
            tower.updateBullet(dT, gameMap);
        }
    }

    if(fortressHP <= 0) {
        gameOver = true;
    }
}

void renderFunction() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    gameMap.renderMap(renderer);  
    for (auto& enemy : enemies) {
        (*enemy).render(runningTime, draw);
    }

    
    for (size_t i = 0; i < towers.size(); i++) {
        towers[i].render(renderer, draw);
        towers[i].renderBullet(renderer);
    }
    if(!draw.getIsVisible()){
       draw.drawTexture("showlist", 0, draw.SCREEN_HEIGHT - draw.BTN_HEIGHT, draw.BTN_WIDTH, draw.BTN_HEIGHT);
    }
    else{
        draw.drawTexture("hidelist", 0, draw.SCREEN_HEIGHT - draw.BTN_HEIGHT, draw.BTN_WIDTH, draw.BTN_HEIGHT);
    }
    draw.renderListBtn(gameMap.getCoin(), font);
    renderFPS();
    SDL_Rect dst = {1000, 10, 0, 0};
    draw.renderText("Fortress: " + to_string(static_cast<int>(gameMap.getFortressHP())) + " / " + to_string(static_cast<int>((maxFortressHP))), dst, font);

    if(gameOver){
        draw.drawTexture("game_over", 1280 / 2 - 50, 720 / 2 - 100, 0, 0);
        SDL_Rect tryAgain = {1280 / 2 - 40, 720 / 2, 0, 0};
        draw.renderText("TRY AGAIN", tryAgain, font);

        SDL_Rect menu = {1280 / 2 - 35, 720 / 2 + 50, 0, 0};
        draw.renderText("MENU", menu, font);

        int arrowY;
        if(selectedOption == 0){
            arrowY = 720 / 2; // Try Again
        }
        else if(selectedOption == 1){
            arrowY = 720 / 2 + 50; //Menu
        }
        SDL_Rect arrow = {1280 / 2 - 55, arrowY, 0, 0};
        draw.renderText(">", arrow, font);

        SDL_Rect enterToConfirm = {1280 / 2 - 30, 720 /2 + 80, 0, 0};
        draw.renderText("Enter to confirm", enterToConfirm, font15);
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
    font = TTF_OpenFont(fileAssets("minecraft.ttf").c_str(), 30);
    if (!font) {
        cerr << "Khong the load font chu: " << TTF_GetError() << endl;
        return false;
    }
    font15 = TTF_OpenFont(fileAssets("minecraft.ttf").c_str(), 15);
    if (!font15) {
        cerr << "Khong the load font chu: " << TTF_GetError() << endl;
        return false;
    }
    
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        cerr << "Khong the khoi tao anh: " << IMG_GetError() << endl;
        return false;
    }

    gameMap.loadMap(fileAssets("map.txt"), renderer); 
    draw = Draw(renderer);
    draw.loadTexture("iceTower", fileAssets("iceTower.png"));
    draw.loadTexture("normal", fileAssets("normal.png"));
    draw.loadTexture("showlist", fileAssets("showlist.png"));
    draw.loadTexture("hidelist", fileAssets("hidelist.png"));
    draw.loadTexture("list", fileAssets("list.png"));

    draw.loadTexture("bomb_character_o_walk_1", fileAssets("bomb_character_o_walk_1.png"));
    draw.loadTexture("bomb_character_o_walk_2", fileAssets("bomb_character_o_walk_2.png"));
    draw.loadTexture("bomb_character_o_walk_3", fileAssets("bomb_character_o_walk_3.png"));
    draw.loadTexture("bomb_character_o_walk_4", fileAssets("bomb_character_o_walk_4.png"));
    draw.loadTexture("bomb_character_o_walk_5", fileAssets("bomb_character_o_walk_5.png"));
    draw.loadTexture("bomb_character_o_walk_6", fileAssets("bomb_character_o_walk_6.png"));

    draw.loadTexture("bomb_character_o_walk_flip_1", fileAssets("bomb_character_o_walk_flip_1.png"));
    draw.loadTexture("bomb_character_o_walk_flip_2", fileAssets("bomb_character_o_walk_flip_2.png"));
    draw.loadTexture("bomb_character_o_walk_flip_3", fileAssets("bomb_character_o_walk_flip_3.png"));
    draw.loadTexture("bomb_character_o_walk_flip_4", fileAssets("bomb_character_o_walk_flip_4.png"));
    draw.loadTexture("bomb_character_o_walk_flip_5", fileAssets("bomb_character_o_walk_flip_5.png"));
    draw.loadTexture("bomb_character_o_walk_flip_6", fileAssets("bomb_character_o_walk_flip_6.png"));
    
    draw.loadTexture("bomb_character_o_explode_1", fileAssets("bomb_character_o_explode_1.png"));
    draw.loadTexture("bomb_character_o_explode_2", fileAssets("bomb_character_o_explode_2.png"));
    draw.loadTexture("bomb_character_o_explode_3", fileAssets("bomb_character_o_explode_3.png"));
    
    draw.loadTexture("game_over", fileAssets("game_over.png"));

    return true;
}

void kill() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_CloseFont(font15);
    font = NULL;
    font15 = NULL;
    renderer = NULL;
    window = NULL;
    TTF_Quit();
    SDL_Quit();
}



string fileAssets(string file) {
    return "D:/laptrinh/LTNC/Code/GameProject/assets/" + file;
}

void renderFPS() {
    static Uint32 lastFrame = SDL_GetTicks();
    static int frameCount = 0;
    static float fps = 0.0f;

    frameCount++;
    Uint32 currentFrame = SDL_GetTicks();
    if (currentFrame - lastFrame >= 1000) {
        fps = frameCount / ((currentFrame - lastFrame) / 1000.0f);
        frameCount = 0;
        lastFrame = currentFrame;
    }

    SDL_Rect dest = {10, 10, 0, 0};
    draw.renderText("FPS: " + to_string(static_cast<int>(fps)), dest, font);
}

void resetGame(){
    enemies.clear();
    for(auto& tower : towers){
        tower.resetBullet();
    }
    towers.clear();
    gameOver = false;
    lastShoot = SDL_GetTicks();
    lastTime = SDL_GetTicks();
    runningTime = 0;
    gameMap.setFortressHP(100);
    fortressHP = gameMap.getFortressHP();
    gameMap.resetMap();
}