#pragma once
#include<fstream>
#include<iostream>
#include<string>
#include<cstring>
#include<ctime>
#include<cstdlib>
#include<iomanip>

#include<SDL.h>
#include<SDL_image.h>

using namespace std;
const int MAPW = 16;
const int MAPH = 9;
const int tileSize = 80;

class Map{
private:
    SDL_Rect mapRect;
    string mapInfo[MAPW][MAPH];
public:
    bool getMap(string inputName, string outputName);
    void renderMap(SDL_Renderer *renderer);
    // void standardization()
};