#include "map.h"

Map::Map() {
    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            tileTextures[i][j] = nullptr;
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

    string basePath = "D:/laptrinh/LTNC/Code/GameProject/assets/Map/Tiles/"; 
    for (int i = 0; i < MAPH; i++) {
        for (int j = 0; j < MAPW; j++) {
            if (tileTextures[i][j]) { // Giải phóng texture cũ nếu có
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
                continue;
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
}