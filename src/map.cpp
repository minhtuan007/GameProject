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
            if (tileTextures[i][j] != nullptr) {
                SDL_DestroyTexture(tileTextures[i][j]);
                tileTextures[i][j] = nullptr;
            }
        }
    }
    for (int i = 0; i < MAPH * 2; i++) {
        for (int j = 0; j < MAPW * 2; j++) {
            if (graphic[i][j] != nullptr) {
                SDL_DestroyTexture(graphic[i][j]);
                graphic[i][j] = nullptr;
            }
        }
    }
}

bool Map::loadMap(SDL_Renderer* renderer) {
    ifstream file(mapFile);
    if (!file) {
        cerr << "Khong the mo file: " << mapFile << endl;
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
    return false;
}

void Map::resetMap(SDL_Renderer* renderer){
    coin = initCoin;
    fortressHP = initFortressHP;
    numBase = 0;
    for(int i = 0; i < MAPH; i++){
        for(int j = 0; j < MAPW; j++){
            used[i][j] = 0;
        }
    }
    loadMap(renderer);
}

void Map::changeTileTexture(int tempX, int tempY, SDL_Renderer* renderer){
    numBase +=1;
    int tileX = tempX / tileSize;
    int tileY = tempY / tileSize;
    if (tileTextures[tileY][tileX] != NULL) {
        SDL_DestroyTexture(tileTextures[tileY][tileX]); 
        tileTextures[tileY][tileX] = NULL;
    }
    SDL_Surface* baseSur = IMG_Load((basePath + "tile_0043.png").c_str());
    if (!baseSur) {
        cerr << "Khong the load anh: " << IMG_GetError() << endl;
        return;
    }
    SDL_Texture* baseTexture = SDL_CreateTextureFromSurface(renderer, baseSur);
    SDL_FreeSurface(baseSur);
    if (!baseTexture) {
        cerr << "Khong the tao texture tu surface cho anh: " << SDL_GetError() << endl;
        return;
    }
    tileTextures[tileY][tileX] = baseTexture;
    mapInfo[tileY][tileX] = "tile_0043.png";
}

int Map::remainBase(int tempX, int tempY){
    int tileX = tempX / tileSize;
    int tileY = tempY / tileSize;
    if(mapInfo[tileY][tileX] == "tile_0012.png" || mapInfo[tileY][tileX] == "tile_0013.png" || mapInfo[tileY][tileX] == "tile_0014.png" 
        || mapInfo[tileY][tileX] == "tile_0024.png" || mapInfo[tileY][tileX] == "tile_0025.png" || mapInfo[tileY][tileX] == "tile_0026.png"
        || mapInfo[tileY][tileX] == "tile_0036.png" || mapInfo[tileY][tileX] == "tile_0037.png" || mapInfo[tileY][tileX] == "tile_0038.png"
        || mapInfo[tileY][tileX] == "tile_0039.png" || mapInfo[tileY][tileX] == "tile_0040.png" || mapInfo[tileY][tileX] == "tile_0041.png" 
        || mapInfo[tileY][tileX] == "tile_0042.png" || mapInfo[tileY][tileX] == "tile_0043.png"){

        return -1;
    }
    return level.limitBase - numBase;
}

bool Map::loadLevelInfo(string inputName){
    ifstream file(inputName);
    if (!file) {
        cerr << "Khong the mo file: " << inputName << endl;
        return false;
    }
    if(!(file >> initEnemyHP)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }

    if(!(file >> initEnemySpeed)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }

    if(!(file >> initFortressHP)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }
    fortressHP = initFortressHP;

    if(!(file >> initCoin)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }
    coin = initCoin;

    if(!(file >> prize)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }
    if(!(file >> level.limitBase)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }
    if(!(file >> level.vectorNum)){
        cerr << "khong the nap loadLevelInfo" << endl;
        return false;
    }
    
    Uint32 tempVar;
    for(int i = 0; i < level.vectorNum; ++i){
        if(!(file >> tempVar)){
            cerr << "khong the nap loadLevelInfo" << endl;
            return false;
        }
        level.startWave.push_back(tempVar);
    }

    for(int i = 0; i < level.vectorNum; ++i){
        if(!(file >> tempVar)){
            cerr << "khong the nap loadLevelInfo" << endl;
            return false;
        }
        level.waveLength.push_back(tempVar);    
    }

    for(int i = 0; i < level.vectorNum; ++i){
        if(!(file >> tempVar)){
            cerr << "khong the nap loadLevelInfo" << endl;
            return false;
        }
        level.enemyNum.push_back(tempVar);    
    }
    
    
    file.close();
    return true;
}

string Map::getHoverType(int x, int y){
    if(x >= 5 && y >= 480 && x <= 100 && y <= 570){
        return iceType;
    }else if(x >= 5 && y >= 580 && x <= 100 && y <= 675){
        return normalType;
    }
    else if(x >= 5 && y >= 385 && x <= 100 && y <= 470){
        return base;
    }
    return "";
}

void Map::towerTypeInfo(string hoverType, Draw& draw, TTF_Font* typeFont){
    string info = "";
    int price;
    if(hoverType == iceType){
        info = "Deals 5 damage and slows enemy's speed by 50%";
        price = iceTowerCoin;
    }
    else if(hoverType == normalType){
        info = "Deals 20 damage to enemy";
        price = normalTowerCoin;
    }
    else if(hoverType == base){
        info = "Used to place a tower";
        price = baseCoin;
    }else{
        info = "";
    }
    draw.showTowerInfo(hoverType, price, info, typeFont);
}