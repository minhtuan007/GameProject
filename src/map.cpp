#include "map.h"

Map::Map() : used(MAPH, vector<int>(MAPW, 0)){
    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            tileTextures[i][j] = nullptr;
        }
    }
    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            graphic[i][j] = nullptr;
        }
    }
    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            graphicInfo[i][j] = "";
        }
    }
}

Map::~Map() {
    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            if (tileTextures[i][j]) {
                SDL_DestroyTexture(tileTextures[i][j]);
                tileTextures[i][j] = nullptr;
            }
        }
    }
    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            if (graphic[i][j]) {
                SDL_DestroyTexture(graphic[i][j]);
                graphic[i][j] = nullptr;
            }
        }
    }
}

bool Map::loadMap(string inputName, SDL_Renderer* renderer) {
    ifstream file(inputName);
    if (!file) {
        cerr << "Khong the mo file: " << inputName << endl;
        return false;
    }

    // Đọc dữ liệu từ file và tạo tên file ảnh
    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            int c;
            file >> c;
            mapInfo[i][j] = "tile_" + string(4 - to_string(c).length(), '0') + to_string(c) + ".png";
        }
    }
    file.close();

    ifstream Fgraphic(graphicFile);
    if (!Fgraphic) {
        cerr << "Khong the mo file: " << graphicFile << endl;
        return false;
    }
    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            int c;
            Fgraphic >> c;
            if(c > -1){
            graphicInfo[i][j] = "tile_" + string(4 - to_string(c).length(), '0') + to_string(c) + ".png";
            }else {
                graphicInfo[i][j] = "";
            }
        }
    }
    Fgraphic.close();

    string basePath = "D:/laptrinh/LTNC/Code/GameProject/assets/Map/Tiles/";
// Tải tileTextures
    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            if (tileTextures[i][j]) {
                SDL_DestroyTexture(tileTextures[i][j]);
                tileTextures[i][j] = nullptr;
            }
            string mapName = basePath + mapInfo[i][j];
            SDL_Surface* imgSurface = IMG_Load(mapName.c_str());
            if (!imgSurface) {
                cerr << "Khong the tai anh " << mapName << ": " << SDL_GetError() << endl;
                continue;
            }
            tileTextures[i][j] = SDL_CreateTextureFromSurface(renderer, imgSurface);
            SDL_FreeSurface(imgSurface);
            if (!tileTextures[i][j]) {
                cerr << "Khong the tao texture cho " << mapName << ": " << SDL_GetError() << endl;
            }
        }
    }
    // Tải graphic
    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            if (graphic[i][j]) {
                SDL_DestroyTexture(graphic[i][j]);
                graphic[i][j] = nullptr;
            }
            if (graphicInfo[i][j] != "") {
                string graphicName = basePath + graphicInfo[i][j];
                SDL_Surface* graphicSurface = IMG_Load(graphicName.c_str());
                if (!graphicSurface) {
                    cerr << "Khong the tai anh " << graphicName << ": " << SDL_GetError() << endl;
                    continue;
                }
                graphic[i][j] = SDL_CreateTextureFromSurface(renderer, graphicSurface);
                SDL_FreeSurface(graphicSurface);
                if (!graphic[i][j]) {
                    cerr << "Khong the tao texture cho " << graphicName << ": " << SDL_GetError() << endl;
                }
            }
        }
    }

    return true;
}

void Map::renderMap(SDL_Renderer* renderer) {
    if (!renderer) {
        cerr << "Renderer is null!" << endl;
        return;
    }

    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            if (tileTextures[i][j]) { 
                mapRect = {j * tileSize, i * tileSize, tileSize, tileSize};
                SDL_RenderCopy(renderer, tileTextures[i][j], NULL, &mapRect);
            }
        }
    }

    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            if (graphic[i][j]) { 
                graphicRect = {j * graphicTile, i * graphicTile, graphicTile, graphicTile};
                SDL_RenderCopy(renderer, graphic[i][j], NULL, &graphicRect);
            }
        }
    }
}

int Map::hasTower(int tempX, int tempY){
    int tileX = tempX / tileSize;
    int tileY = tempY / tileSize;
    if(used[tileY][tileX]){
        cout << "This tile has been used" <<endl;
    }
    return used[tileY][tileX];
};

void Map::setUsedTile(int tempX, int tempY){
    int tileX = tempX / tileSize;
    int tileY = tempY / tileSize;
    used[tileY][tileX] = 1;
}

bool Map::isValidToSet(int tempX, int tempY){
    int tileX = tempX / tileSize;
    int tileY = tempY / tileSize;
    if(mapInfo[tileY][tileX] == "tile_0043.png"){
        return true;
    }
    cout<<"Not valid to set a Tower"<<endl;
    return false;
}

void Map::resetMap(){
    coin = 50;
    prize = 10;
    fortressHP = 100;
    for(int i = 0; i < MAPH; i++){
        for(int j = 0; j < MAPW; j++){
            used[i][j] = 0;
        }
    }
}