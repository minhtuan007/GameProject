#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int MAPW = 16;
const int MAPH = 9;
const int tileSize = 80;

class Map {
private:
    SDL_Rect mapRect;
    string mapInfo[MAPH][MAPW];         
    SDL_Texture* tileTextures[MAPH][MAPW]; 
public:
    Map();                              
    ~Map();                            
    bool loadMap(string inputName, SDL_Renderer* renderer); 
    void renderMap(SDL_Renderer* renderer);
};