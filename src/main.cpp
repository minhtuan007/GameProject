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
#include<SDL_mixer.h>

#include "Enemies.h"
#include "Tower.h"
#include "map.h"
#include "bullet.h"
#include "drawElement.h"
#include "drawNotification.h"

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
TTF_Font* font12;
Mix_Music* music;
bool isRunning = true;
string selectedTowerType = "";
float slowDown = 1;

Map gameMap;
Draw draw;
vector<shared_ptr<Enemy>> enemies;
vector<Tower> towers;

Notification notification;
bool gameOver = false;
bool youWin = false;
bool canSetBase = true;
bool showInfo = false;
int selectedOption = 0;
int levelIndex = 0;
bool flagRunningTime = true;
bool isLevelPicked = false;
bool mapLoaded = false;
string level_info = "";
string mapName = "";
string enemyPath1 = "";
string enemyPath2 = "";
string graphicName = "";
string hoverType = "";
bool flagReadFile = false;
Uint32 lastTime;
Uint32 runningTime = 0;
float fortressHP;
bool flagWaveInfo = false;
Uint32 waveInfoTime = 0;

int levelWidth = 100;
int levelNum = 5;
int levelMargin = 20;
int levelPosX = 381;
int notifY = 70;
int notifX = 1100;
int notifH = 60;
int notifW = 170;


bool init();
bool GameIsRunning();
void kill();
bool handleEvents();
void updateFunction();
void renderFunction();
void renderFPS();
void resetGame();
int checkLevel(int tempX, int tempY);
string fileAssets(string file);
Uint32 lastShoot = SDL_GetTicks();

int main(int argc, char** argv) {
    if (!init()) {
        system("pause");
        return -1;
    }

    srand(time(0));
    
    if(!GameIsRunning()){
        system("pause");
        return -1;
    }
    cout << "GameOver!!!" << endl;
    kill();
    return 0;
}

bool GameIsRunning() {
    const Uint32 TARGET_FPS = 60;
    const Uint32 FRAME_TIME = 1000 / TARGET_FPS;
    Uint32 frameStart, frameTime;

    while (isRunning) {
        frameStart = SDL_GetTicks();


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        handleEvents();
        updateFunction();
        renderFunction();
        SDL_RenderPresent(renderer);

        if(!mapLoaded && flagReadFile && !isLevelPicked){
            if(!gameMap.loadLevelInfo(fileAssets(level_info))){
                cerr << "Khong the load level_info" << endl;
                return false;
            }
            fortressHP = gameMap.getInitFortressHP();
            if(!gameMap.loadMap(renderer)){
                cerr << "Khong the load map" << endl;
                return false;
            }
            mapLoaded = true;
            isLevelPicked = true;
            lastTime = SDL_GetTicks();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameTime);
        }
    }
    return true;
}

bool handleEvents() {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            resetGame(); //resetGame để tránh lỗi giải phóng tài nguyên 2 lần (double free)
            isRunning = false;
            break;
        }
        if (evt.key.keysym.sym == SDLK_F1 && evt.type == SDL_KEYDOWN) {
            system("pause");
        }
        if (evt.key.keysym.sym == SDLK_n) {
            cout << "n" << endl;
        }
        if(evt.type == SDL_MOUSEMOTION){
            int mouseX = evt.motion.x;
            int mouseY = evt.motion.y;
            hoverType = gameMap.getHoverType(mouseX, mouseY);
        }
        if (evt.type == SDL_MOUSEBUTTONDOWN){
            int tempX = evt.button.x;
            int tempY = evt.button.y;
            cout<< tempX << " " << tempY<<endl;
            if(!isLevelPicked){// Setup bản đồ cho từng lv
                int pickingMap = checkLevel(tempX, tempY);
                if(pickingMap != -1){
                    string levelFolder = "level" + to_string(pickingMap);
                    mapName =  levelFolder + "/level.txt";
                    level_info = levelFolder + "/level_info.txt";
                    enemyPath1 = levelFolder + "/enemyPath1.txt";
                    enemyPath2 = levelFolder + "/enemyPath2.txt";
                    graphicName = levelFolder + "/graphic.txt";
                    gameMap.setMapFile(fileAssets(mapName));
                    gameMap.setGraphicFile(fileAssets(graphicName));
                    flagReadFile = true;
                }
            }
            else if(isLevelPicked  && !gameOver && !youWin && !showInfo){
                if(draw.clickInfoBtn(tempX, tempY)){
                    showInfo = true;
                }
                Tower tower(gameMap);
                if(draw.clickListBtn(tempX, tempY)){// Khung item
                    draw.toggleIsVisible();
                }
                else if(tower.buyItem(tempX, tempY) && draw.getIsVisible()){
                    canSetBase = false;
                    // Mua item
                    if(tower.isEnoughCoin(gameMap.getCoin(), tower.getBuying())){
                        selectedTowerType = tower.getBuying();
                        if(selectedTowerType == tower.geticeType()){
                            slowDown = 0.5;
                            // draw.drawTexture("iceTower", currentX, currentY, 80, 80);
                        }else if(selectedTowerType == tower.getnormalType()){
                            // draw.drawTexture("normal", currentX, currentY, 80, 80);
                        }else if(selectedTowerType == tower.getBaseType()){
                            //
                        }
                        notification.setNotif("Buying " + selectedTowerType, 1030, notifY, 250, notifH, SDL_GetTicks());
                    }
                    else{
                        notification.setNotif("Not enough coins", 990, notifY + 1 * notifH, 290, notifH, SDL_GetTicks());
                        selectedTowerType = "";
                    }                
                }else{
                    canSetBase = true;
                }
                // đặt đế
                if (gameMap.hasTower(tempX, tempY) == 0 && canSetBase) {
                    if (evt.button.button == SDL_BUTTON_LEFT) {
                        if(selectedTowerType == tower.getBaseType()){
                            int remainBase = gameMap.remainBase(tempX, tempY);
                            if(remainBase > 0){
                                tower.payment(gameMap, selectedTowerType);
                                selectedTowerType = "";
                                gameMap.changeTileTexture(tempX, tempY, renderer);
                            }
                            else if(remainBase != -1){
                                notification.setNotif(to_string(gameMap.level.limitBase - remainBase) + "/" + to_string(gameMap.level.limitBase) + " Base",
                                    1080, notifY + 2 * notifH, 200, notifH, SDL_GetTicks());
                            }else if(remainBase == -1){
                                notification.setNotif("Not valid to set base here",
                                    830, notifY + 3 * notifH, 450, notifH, SDL_GetTicks());
                            }
                            
                        }
                        else if(gameMap.isValidToSet(tempX, tempY)){
                            if(selectedTowerType != ""){
                                tower.setTower(tempX, tempY, selectedTowerType, slowDown);
                                towers.push_back(tower);
                                gameMap.setUsedTile(tempX, tempY);
                                tower.payment(gameMap, selectedTowerType);
                                selectedTowerType = "";
                                slowDown = 1;
                            }
                            else if(selectedTowerType == ""){
                                notification.setNotif("Please, pick a tower",
                                    930, notifY + 3 * notifH, 350, notifH, SDL_GetTicks());
                            }
                        }else if(!(gameMap.isValidToSet(tempX, tempY)) && selectedTowerType != ""){
                            notification.setNotif("Not valid to set a tower here",
                                830, notifY + 4 * notifH, 450, notifH, SDL_GetTicks());
                        }
                    }
                }
            }
            // Show thuộc tính item
            else if(showInfo && draw.clickInfo_BackBtn(tempX, tempY)){
                showInfo = false;
                lastTime = SDL_GetTicks();
            }
        }

        if((gameOver || youWin) && evt.type == SDL_KEYDOWN && isLevelPicked){
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
                    if (selectedOption == 1) { // menu: Thoát game
                        isLevelPicked = false;
                        mapLoaded = false;
                    }
                    resetGame(); // Chọn gì cũng mặc định reset lại game
                    break;
            }
        }
    }
    return true;
}


void updateFunction() {
    if(!gameOver && isLevelPicked && !youWin && !showInfo){
        Uint32 currentTime = SDL_GetTicks();
        Uint32 dT = currentTime - lastTime;
        runningTime += dT;
        lastTime = currentTime;
        //Tính toán các đợt tấn công của enemy
        if(runningTime >= (gameMap.level.startWave[levelIndex] + gameMap.level.waveLength[levelIndex]) * 1000 && flagRunningTime){
            levelIndex += 1;
            if(levelIndex < gameMap.level.vectorNum){
                flagWaveInfo = true;
                waveInfoTime = SDL_GetTicks();
            }
            flagRunningTime = false;
        }


        if(levelIndex < gameMap.level.vectorNum && runningTime >= (gameMap.level.startWave[levelIndex]) * 1000
            && runningTime <= (gameMap.level.startWave[levelIndex] + gameMap.level.waveLength[levelIndex]) * 1000){
            int MAX_ENEMIES = gameMap.level.enemyNum[levelIndex];
            //Spawn enemy
            if (int(enemies.size()) < MAX_ENEMIES && rand() % 100 < 2) {
                enemies.push_back(make_shared<Enemy>(fileAssets(enemyPath1), fileAssets(enemyPath2), gameMap));
            }
            flagRunningTime = true;
        }

        // Kiểm tra enemy đi ra khỏi màn hình hoặc bị tiêu diệt 
        //-> xoá đạn nếu nó dí enemy khi enemy bị tiêu diệt
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
                //Tính toán đạn bắn enemy khi enemy tới vùng hoạt động của tháp
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
        if(fortressHP <= 0) {
            gameOver = true;
            return;
        }
        if(levelIndex == gameMap.level.vectorNum && int(enemies.size()) == 0){
            youWin = true;
            return;
        }
    }
}

void renderFunction() {

    if(isLevelPicked){
        gameMap.renderMap(renderer);  
        for (auto& enemy : enemies) {
            (*enemy).render(runningTime, draw);
        }

        
        for (size_t i = 0; i < towers.size(); i++) {
            towers[i].render(renderer, draw);
            towers[i].renderBullet(renderer);
        }

        if(flagWaveInfo){
            if(SDL_GetTicks() - waveInfoTime < 3000){//2 giây
                string waveInfo = "Wave " + to_string(levelIndex) + " - Start Time: " + to_string(gameMap.level.startWave[levelIndex]) +
                " - Duration: " + to_string(gameMap.level.waveLength[levelIndex]) + " seconds";
                SDL_Rect waveInfoDst = {330, 310, 0, 0};
                draw.renderText(waveInfo, waveInfoDst, font);
            }else{
                flagWaveInfo = false;
            }
        }
        notification.updateAndShow(font);
    

        if(!draw.getIsVisible()){
            draw.drawTexture("showlist", 0, draw.SCREEN_HEIGHT - draw.BTN_HEIGHT, draw.BTN_WIDTH, draw.BTN_HEIGHT);
        }
        else{
            if(hoverType != ""){
                gameMap.towerTypeInfo(hoverType, draw, font12);
            }
            draw.drawTexture("hidelist", 0, draw.SCREEN_HEIGHT - draw.BTN_HEIGHT, draw.BTN_WIDTH, draw.BTN_HEIGHT);
        }
        draw.renderListBtn(gameMap.getCoin(), font);
        SDL_Rect dst = {1000, 10, 0, 0};
        draw.renderText("Fortress: " + to_string(static_cast<int>(gameMap.getFortressHP())) + " / " + to_string(static_cast<int>((gameMap.getInitFortressHP()))), dst, font);
        
        draw.drawTexture("info", 1240, 40, 30, 30);


        if(gameOver || youWin){
            if(gameOver){
                draw.drawTexture("game_over", 1280 / 2 - 50, 720 / 2 - 100, 0, 0);
            }else if(youWin){
                draw.drawTexture("you_win", 1280 / 2 - 388 / 2, 720 / 2 - 200, 0, 0);
            }

            SDL_Rect tryAgain = {1280 / 2 - 70, 720 / 2, 0, 0};
            draw.renderText("TRY AGAIN", tryAgain, font);

            SDL_Rect menu = {1280 / 2 - 65, 720 / 2 + 50, 0, 0};
            draw.renderText("MENU", menu, font);

            int arrowY;
            if(selectedOption == 0){
                arrowY = 720 / 2; // Try Again
            }
            else if(selectedOption == 1){
                arrowY = 720 / 2 + 50; //Menu
            }
            SDL_Rect arrow = {1280 / 2 - 85, arrowY, 0, 0};
            draw.renderText(">", arrow, font);

            SDL_Rect enterToConfirm = {1280 / 2 - 60, 720 /2 + 80, 0, 0};
            draw.renderText("Enter to confirm", enterToConfirm, font12);
        }
        else if(showInfo){
            draw.drawTexture("notif_board", 1280 / 2 - 660, 720 / 2 - 376, 660 * 2, 376 * 2);
    
        stringstream ss;
        ss << fixed << setprecision(2) << gameMap.getInitEnemyHP();
        string result = ss.str();
        SDL_Rect EnemyHpDst = {375, 190, 0, 0};
        draw.renderText("Enemy HP: " + result, EnemyHpDst, font);
        SDL_Rect LimitBaseDst = {375, 190 + 1 * 30, 0, 0};
        draw.renderText("Limit Base: " + to_string(gameMap.getLimitBase()), LimitBaseDst, font);
        SDL_Rect prizeDst = {375, 190 + 2 * 30, 0, 0};
        draw.renderText("Prize/Enemy: " + to_string(gameMap.getPrize()) + "$", prizeDst, font);
        SDL_Rect basePriceDst = {375, 190 + 3 * 30, 0, 0};
        draw.renderText("Base Price: " + to_string(gameMap.getBasePrice()) + "$", basePriceDst, font);
        SDL_Rect icePriceDst = {375, 190 + 4 * 30, 0, 0};
        draw.renderText("Ice Tower Price: " + to_string(gameMap.getIceTowerPrice()) + "$", icePriceDst, font);
        SDL_Rect iceDamageDst = {375, 190 + 5 * 30, 0, 0};
        draw.renderText("Ice Tower Damage: " + to_string(gameMap.getIceDamage()), iceDamageDst, font);
        SDL_Rect iceSlowDamageDst = {375, 190 + 6 * 30, 0, 0};
        draw.renderText("Ice SlowDown: 50%", iceSlowDamageDst, font);
        SDL_Rect woodPriceDst = {375, 190 + 7 * 30, 0, 0};
        draw.renderText("Wood Tower Price: " + to_string(gameMap.getNormalTowerPrice()) + "$", woodPriceDst, font);
        SDL_Rect woodDamageDst = {375, 190 + 8 * 30, 0, 0};
        draw.renderText("Wood Tower Damage: " + to_string(gameMap.getNormalDamage()), woodDamageDst, font);


        draw.drawTexture("back", 580, 590, 140, 60);
        }

        SDL_Rect destTime = {10, 50, 0, 0};
        draw.renderText("Time: " + to_string(static_cast<int>(runningTime/1000)), destTime, font);
    }
    else{
        draw.drawTexture("background", 0, 0, draw.SCREEN_WIDTH, draw.SCREEN_HEIGHT, true);
        int leftLevel = (draw.SCREEN_WIDTH - levelNum * (levelWidth + levelMargin * 2)) / 2;

        SDL_Rect dstLv1 = {levelPosX, 443, 100, 100};
        draw.drawTexture("level", leftLevel + 0 * (levelWidth + levelMargin * 2) + levelMargin, draw.SCREEN_HEIGHT / 2,
                         levelWidth, levelWidth, true);
        draw.renderText("1", dstLv1, font12);

        SDL_Rect dstLv2 = {levelPosX + 1 * (levelWidth + levelMargin * 2), 443, 100, 100};
        draw.drawTexture("level", leftLevel + 1 * (levelWidth + levelMargin * 2) + levelMargin, draw.SCREEN_HEIGHT / 2,
                         levelWidth, levelWidth, true);
        draw.renderText("2", dstLv2, font12);

        SDL_Rect dstLv3 = {levelPosX + 2 * (levelWidth + levelMargin * 2), 443, 100, 100};
        draw.drawTexture("level", leftLevel + 2 * (levelWidth + levelMargin * 2) + levelMargin, draw.SCREEN_HEIGHT / 2,
                         levelWidth, levelWidth, true);
        draw.renderText("3", dstLv3, font12);

        SDL_Rect dstLv4 = {levelPosX + 3 * (levelWidth + levelMargin * 2), 443, 100, 100};
        draw.drawTexture("level", leftLevel + 3 * (levelWidth + levelMargin * 2) + levelMargin, draw.SCREEN_HEIGHT / 2,
                         levelWidth, levelWidth, true);
        draw.renderText("4", dstLv4, font12);

        SDL_Rect dstLv5 = {levelPosX + 4 * (levelWidth + levelMargin * 2), 443, 100, 100};
        draw.drawTexture("level", leftLevel + 4 * (levelWidth + levelMargin * 2) + levelMargin, draw.SCREEN_HEIGHT / 2,
                         levelWidth, levelWidth, true);
        draw.renderText("5", dstLv5, font12);

        
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
    font = TTF_OpenFont(fileAssets("minecraft.ttf").c_str(), 30);
    if (!font) {
        cerr << "Khong the load font chu: " << TTF_GetError() << endl;
        return false;
    }
    font12 = TTF_OpenFont(fileAssets("minecraft.ttf").c_str(), 12);
    if (!font12) {
        cerr << "Khong the load font chu: " << TTF_GetError() << endl;
        return false;
    }
    
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        cerr << "Khong the khoi tao anh: " << IMG_GetError() << endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize: " << Mix_GetError() << endl;
        return false;
    }

    music = Mix_LoadMUS(fileAssets("gameMusic.mp3").c_str());
    if(!music){
        cerr << "Can not load music: " << Mix_GetError() << endl;
        return false;   
    }

    if(Mix_PlayMusic(music, -1) != 0){ //return -1 if it failed
        cerr << "Can not play background music: " << Mix_GetError() << endl;
        return false;
    }


    draw = Draw(renderer);
    draw.loadTexture("Ice", fileAssets("iceTower.png"));
    draw.loadTexture("Normal", fileAssets("normal.png"));
    draw.loadTexture("showlist", fileAssets("showlist.png"));
    draw.loadTexture("hidelist", fileAssets("hidelist.png"));
    draw.loadTexture("list", fileAssets("list.png"));
    draw.loadTexture("info", fileAssets("info.png"));
    draw.loadTexture("notif_board", fileAssets("notif_board.png"));
    draw.loadTexture("back", fileAssets("back.png"));
    // draw.loadTexture("notification", fileAssets("notification.png"));

    draw.loadTexture("bomb_character_o_walk_1", fileAssets("character/bomb_character_o_walk_1.png"));
    draw.loadTexture("bomb_character_o_walk_2", fileAssets("character/bomb_character_o_walk_2.png"));
    draw.loadTexture("bomb_character_o_walk_3", fileAssets("character/bomb_character_o_walk_3.png"));
    draw.loadTexture("bomb_character_o_walk_4", fileAssets("character/bomb_character_o_walk_4.png"));
    draw.loadTexture("bomb_character_o_walk_5", fileAssets("character/bomb_character_o_walk_5.png"));
    draw.loadTexture("bomb_character_o_walk_6", fileAssets("character/bomb_character_o_walk_6.png"));

    draw.loadTexture("bomb_character_o_walk_flip_1", fileAssets("character/bomb_character_o_walk_flip_1.png"));
    draw.loadTexture("bomb_character_o_walk_flip_2", fileAssets("character/bomb_character_o_walk_flip_2.png"));
    draw.loadTexture("bomb_character_o_walk_flip_3", fileAssets("character/bomb_character_o_walk_flip_3.png"));
    draw.loadTexture("bomb_character_o_walk_flip_4", fileAssets("character/bomb_character_o_walk_flip_4.png"));
    draw.loadTexture("bomb_character_o_walk_flip_5", fileAssets("character/bomb_character_o_walk_flip_5.png"));
    draw.loadTexture("bomb_character_o_walk_flip_6", fileAssets("character/bomb_character_o_walk_flip_6.png"));
    
    draw.loadTexture("bomb_character_o_explode_1", fileAssets("character/bomb_character_o_explode_1.png"));
    draw.loadTexture("bomb_character_o_explode_2", fileAssets("character/bomb_character_o_explode_2.png"));
    draw.loadTexture("bomb_character_o_explode_3", fileAssets("character/bomb_character_o_explode_3.png"));
    
    draw.loadTexture("game_over", fileAssets("game_over.png"));
    draw.loadTexture("you_win", fileAssets("you_win.png"));
    
    draw.loadTexture("background", fileAssets("background.png"));
    draw.loadTexture("level", fileAssets("level.png"));
    
    notification = Notification(renderer);
    return true;
}

void kill() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_CloseFont(font12);
    Mix_FreeMusic(music);
    music = NULL;
    font = NULL;
    font12 = NULL;
    renderer = NULL;
    window = NULL;
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}



string fileAssets(string file) {
    return "D:/laptrinh/CPP/LTNC/Code/GameProject/assets/" + file;
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

    SDL_Rect destFPS = {10, 10, 0, 0};
    draw.renderText("FPS: " + to_string(static_cast<int>(fps)), destFPS, font);
}

void resetGame(){
    enemies.clear();
    for(auto& tower : towers){
        tower.resetBullet();
    }
    towers.clear();
    gameOver = false;
    youWin = false;
    showInfo = false;
    lastShoot = SDL_GetTicks();
    lastTime = SDL_GetTicks();
    runningTime = 0;
    fortressHP = gameMap.getInitFortressHP();
    gameMap.setFortressHP(fortressHP);
    gameMap.resetMap(renderer);
    selectedTowerType = "";
    hoverType = "";
    canSetBase = true;
    levelIndex = 0;
    flagRunningTime = true;
    flagReadFile = false;
    flagWaveInfo = false;
}

int checkLevel(int tempX, int tempY){
    int lvHeight = draw.SCREEN_HEIGHT / 2;
    int leftLevel = (draw.SCREEN_WIDTH - levelNum * (levelWidth + levelMargin * 2)) / 2;
    int lv1 = leftLevel + 0 * (levelWidth + levelMargin * 2) + levelMargin;
    int lv2 = leftLevel + 1 * (levelWidth + levelMargin * 2) + levelMargin;
    int lv3 = leftLevel + 2 * (levelWidth + levelMargin * 2) + levelMargin;
    int lv4 = leftLevel + 3 * (levelWidth + levelMargin * 2) + levelMargin;
    int lv5 = leftLevel + 4 * (levelWidth + levelMargin * 2) + levelMargin;
    int ans = -1;
    if(tempX >= lv1 && tempY >= lvHeight && tempX <= lv1 + levelWidth && tempY <= lvHeight + levelWidth){
        ans = 1;
    }
    else if(tempX >= lv2 && tempY >= lvHeight && tempX <= lv2 + levelWidth && tempY <= lvHeight + levelWidth){
        ans = 2;
    }
    else if(tempX >= lv3 && tempY >= lvHeight && tempX <= lv3 + levelWidth && tempY <= lvHeight + levelWidth){
        ans = 3;
    }
    else if(tempX >= lv4 && tempY >= lvHeight && tempX <= lv4 + levelWidth && tempY <= lvHeight + levelWidth){
        ans = 4;
    }
    else if(tempX >= lv5 && tempY >= lvHeight && tempX <= lv5 + levelWidth && tempY <= lvHeight + levelWidth){
        ans = 5;
    }
    return ans;
}