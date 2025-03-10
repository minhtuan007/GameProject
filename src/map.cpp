#include<map.h>



bool Map::getMap(string inputName, string outputName){
    ifstream file(inputName);
    ofstream ofile(outputName);
    ifstream mapFile(outputName);

    if(!file){
        cerr << "Khong the mo file:" << endl;
        system("pause");
        return false;
    }

    for(int i = 0; i < MAPH; i++){
        for(int j = 0; j < MAPW; j++){
            int c;
            file >> c;
            ofile <<"tile_" << setw(4) << setfill('0') << c <<".png ";
        }ofile << endl;
    }

    for(int i = 0; i < MAPH; i++){
        for(int j = 0; j < MAPW; j++){
            mapFile >> mapInfo[i][j];
        }
    }

    mapFile.close();
    ofile.close();
    file.close();
    return true;
}

void Map::renderMap(SDL_Renderer *renderer){
    for(int i = 0; i < MAPH; i++){
        for(int j = 0; j < MAPW; j++){
            string mapName = "D:/laptrinh/LTNC/Code/GameProject/assets/Map/Tiles/" + mapInfo[i][j];
            SDL_Surface* imgSurface = IMG_Load(mapName.c_str());
            if ( !imgSurface ) {
                cout << "Error loading image test.png: " << SDL_GetError() << endl;
                system("pause");
            }

            SDL_Texture* imgTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);
            SDL_FreeSurface(imgSurface);
            imgSurface = NULL;
            if ( !imgTexture ) {
                cout << "Error creating texture: " << SDL_GetError() << endl;
                system("pause");
            }

            mapRect = {j * tileSize, i * tileSize, tileSize, tileSize};
            SDL_RenderCopy(renderer, imgTexture, NULL, &mapRect);
            SDL_DestroyTexture(imgTexture);
            imgTexture = NULL;
        }
    }   
}