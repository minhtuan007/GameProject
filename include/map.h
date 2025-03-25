#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <SDL.h>
#include <SDL_image.h>

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
    string graphicFile = "D:/laptrinh/LTNC/Code/GameProject/assets/graphic.txt";
    int coin = 50;
    int prize = 10;
    float fortressHP = 100;
public:
    Map();                              
    ~Map();                            
    bool loadMap(string inputName, SDL_Renderer* renderer); 
    void renderMap(SDL_Renderer* renderer);
    int hasTower(int tempX, int tempY);
    void setUsedTile(int tempX, int tempY);
    bool isValidToSet(int tempX, int tempY);
    int getCoin() const {return coin;};
    void setCoin(int coin) {this->coin = coin;};
    int getPrize() const {return prize;};
    float getFortressHP() {return fortressHP;};
    void setFortressHP(float newHP) {fortressHP = newHP;};
    void resetMap();
    // int getTileSize() const {return tileSize;};
};