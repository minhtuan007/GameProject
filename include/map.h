#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <SDL.h>
#include <SDL_image.h>

#include "drawElement.h" 

using namespace std;

const int MAPW = 16;
const int MAPH = 9;
const int tileSize = 80;
const int graphicTile = 40;

class Map {
private:
    vector<vector<int>> used;
    SDL_Rect mapRect;
    SDL_Rect graphicRect;
    string mapInfo[MAPH][MAPW];         
    SDL_Texture* tileTextures[MAPH][MAPW]; 
    SDL_Texture* graphic[MAPH*2][MAPW*2]; 
    string graphicInfo[MAPH*2][MAPW*2]; 
    string graphicFile = "";
    string mapFile = "";
    string basePath = "assets/Map/Tiles/";
    int initCoin;
    float initFortressHP;
    float initEnemyHP;
    float initEnemySpeed;
    int coin;
    int prize;
    float fortressHP;
    int numBase = 0;
    int baseCoin = 50;
    int iceTowerCoin = 100;
    float iceDamage = 5;

    int normalTowerCoin = 70;
    float damage = 20;

    string iceType = "Ice";
    string normalType = "Normal";
    string base = "Base";
    struct LEVEL_INFO{
        int limitBase;
        int vectorNum;
        vector<Uint32> startWave;
        vector<Uint32> waveLength;
        vector<Uint32> enemyNum;
    };
public:
    Map();                              
    ~Map();                            
    bool loadLevelInfo(string inputName);
    LEVEL_INFO level;
    bool loadMap(SDL_Renderer* renderer); 
    void renderMap(SDL_Renderer* renderer);
    int hasTower(int tempX, int tempY);
    void setUsedTile(int tempX, int tempY);
    bool isValidToSet(int tempX, int tempY);
    int getCoin() const {return coin;};
    void setCoin(int coin) {this->coin = coin;};
    int getPrize() const {return prize;};
    float getFortressHP() {return fortressHP;};
    float getInitFortressHP() {return initFortressHP;};
    float getInitEnemyHP() {return initEnemyHP;};
    float getInitEnemySpeed() {return initEnemySpeed;};
    int getLimitBase() {return level.limitBase;};
    void setFortressHP(float newHP) {fortressHP = newHP;};
    void changeTileTexture(int tempX, int tempY, SDL_Renderer* renderer);
    void resetMap(SDL_Renderer* renderer);
    int remainBase(int tempX, int tempY);
    void setMapFile(string mapFile) {this->mapFile = mapFile;};
    void setGraphicFile(string graphicFile) {this->graphicFile = graphicFile;};
    int getBasePrice() const {return baseCoin;};
    int getIceTowerPrice() const {return iceTowerCoin;};
    int getIceDamage() const {return iceDamage;};
    int getNormalTowerPrice() const {return normalTowerCoin;};
    int getNormalDamage() const {return damage;};
    string getHoverType(int x, int y);
    void towerTypeInfo(string hoverType, Draw& draw, TTF_Font* typeFont);
    string getIceName() const {return iceType;}
    string getNormalName() const {return normalType;}
    string getBaseName() const {return base;}
};