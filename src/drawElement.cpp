#include "drawElement.h"


Draw::Draw() : renderer(nullptr) {
}

Draw::Draw(SDL_Renderer* renderer) : renderer(renderer){
    if (!renderer) {
        cerr << "Renderer is null in Draw constructor!" << endl;
    }
}

Draw::~Draw() {
    textures.clear();
}

void Draw::loadTexture(const string& key, const string& filePath) {
    if (textures.find(key) != textures.end()) {
        cerr << "Texture with key " << key << " already exists!" << endl;
        return;
    }

    SDL_Surface* imgSur = IMG_Load(filePath.c_str());
    if (!imgSur) {
        cerr << "Khong the load anh " << filePath << ": " << IMG_GetError() << endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgSur);
    SDL_FreeSurface(imgSur);

    if (!texture) {
        cerr << "Khong the tao texture tu surface cho anh " << filePath << ": " << SDL_GetError() << endl;
        return;
    }

    // Lưu texture vào map với shared_ptr
    textures[key] = shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
}

void Draw::drawTexture(const string& key, int x, int y, int w, int h, bool setLogicalSize) {
    auto it = textures.find(key);
    if (it == textures.end()) {
        cerr << "Texture with key " << key << " not found!" << endl;
        return;
    }

    SDL_Rect dstRect = {x, y, w, h};
    if (w == 0) {
        SDL_QueryTexture(it->second.get(), nullptr, nullptr, &dstRect.w, nullptr);
    }
    if (h == 0) {
        SDL_QueryTexture(it->second.get(), nullptr, nullptr, nullptr, &dstRect.h);
    }
    if(setLogicalSize){
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    SDL_RenderCopy(renderer, it->second.get(), nullptr, &dstRect);
}

bool Draw::clickListBtn(int x, int y){
    if(x >= 0 && x <= BTN_WIDTH && y >= 720 - BTN_HEIGHT && y <= 720){
        return true;
    }
    return false;
}

bool Draw::clickInfoBtn(int x, int y){
    if(x >= 1240 && x <= 1240 + 30 && y >= 40 && y <= 40 + 30){
        return true;
    }
    return false;
}
//610, 475, 80, 35
bool Draw::clickInfo_BackBtn(int x, int y){
    if(x >= 580 && x <= 580 + 140 && y >= 590 && y <= 590 + 60){
        return true;
    }
    return false;
}

void Draw::renderListBtn(int coin, TTF_Font* typeFont){
    if(slider.isVisible){
        drawTexture("list", 0, SCREEN_HEIGHT - BTN_HEIGHT - 360, 0, 0);
        SDL_Rect dst = {37, 337, 40, 50};
        renderText(to_string(coin), dst, typeFont);
    }

}

void Draw::renderText(string text, SDL_Rect destination, TTF_Font* typeFont, Uint32 wrapLength) {
    SDL_Color textColor = {255, 255, 255}; 

    if (text.empty()) {
        return;
    }

    SDL_Surface* textSur = nullptr;
    if (wrapLength > 0) {
        textSur = TTF_RenderText_Blended_Wrapped(typeFont, text.c_str(), textColor, wrapLength);
    } else {
        textSur = TTF_RenderText_Blended(typeFont, text.c_str(), textColor);
    }

    if (!textSur) {
        cout << "TTF_RenderText failed: " << TTF_GetError() << endl;;
        return;
    }

    destination.w = textSur->w;
    destination.h = textSur->h;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSur);
    if (!textTexture) {
        cout << "SDL_CreateTextureFromSurface failed: " <<  SDL_GetError() << endl;
        SDL_FreeSurface(textSur);
        return;
    }

    SDL_RenderCopy(renderer, textTexture, NULL, &destination);

    SDL_FreeSurface(textSur);
    SDL_DestroyTexture(textTexture);
    textTexture = NULL;
    textSur = NULL;
}

void Draw::showTowerInfo(string hoverType, int price, string info, TTF_Font* typeFont){
    roundedBoxRGBA(renderer, 100, 430, 240, 610, 5, 0, 0, 0, 100);
    SDL_Rect priceDst = {110, 440, 0, 0};
    renderText(hoverType + " price: " + to_string(price), priceDst, typeFont);

    SDL_Rect msDst = {110, 480, 0, 0}; 
    renderText(info, msDst, typeFont, 130);

}




