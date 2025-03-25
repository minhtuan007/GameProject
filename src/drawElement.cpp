#include "drawElement.h"


Draw::Draw() : renderer(nullptr) {
}

Draw::Draw(SDL_Renderer* renderer) : renderer(renderer){
    if (!renderer) {
        std::cerr << "Renderer is null in Draw constructor!" << std::endl;
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

void Draw::drawTexture(const string& key, int x, int y, int w, int h) {
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

    SDL_RenderCopy(renderer, it->second.get(), nullptr, &dstRect);
}

bool Draw::clickListBtn(int x, int y){
    if(x >= 0 && x <= BTN_WIDTH && y >= 720 - BTN_HEIGHT && y <= 720){
        return true;
    }
    return false;
}



void Draw::renderListBtn(int coin, TTF_Font* typeFont){
    if(slider.isVisible){
        drawTexture("list", 0, SCREEN_HEIGHT - 308 - BTN_HEIGHT, 120, 308);
        SDL_Rect dst = {42, 395, 40, 70};
        renderText(to_string(coin), dst, typeFont);
    }

}

void Draw::renderText(string text, SDL_Rect destination, TTF_Font* typeFont) {
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSur = TTF_RenderText_Solid(typeFont, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSur);
    destination.w = textSur->w;
    destination.h = textSur->h;   
    SDL_RenderCopy(renderer, textTexture, NULL, &destination);
    SDL_FreeSurface(textSur);
    SDL_DestroyTexture(textTexture);

    textTexture = NULL;
    textSur = NULL;
}










// void Draw::drawTexturePart(const string& key, int x, int y, int srcX, int srcY, int srcW, int srcH, int dstW, int dstH) {
//     auto it = textures.find(key);
//     if (it == textures.end()) {
//         cerr << "Texture with key " << key << " not found!" << endl;
//         return;
//     }

//     SDL_Rect srcRect = {srcX, srcY, srcW, srcH};
//     SDL_Rect dstRect = {x, y, dstW, dstH};
//     if (dstW == 0 || dstH == 0) {
//         dstRect.w = srcRect.w;
//         dstRect.h = srcRect.h;
//     }

//     SDL_RenderCopy(renderer, it->second.get(), &srcRect, &dstRect);
// }

// void Draw::drawTextureEx(const string& key, int x, int y, int w, int h, double angle, SDL_RendererFlip flip) {
//     auto it = textures.find(key);
//     if (it == textures.end()) {
//         cerr << "Texture with key " << key << " not found!" << endl;
//         return;
//     }

//     SDL_Rect dstRect = {x, y, w, h};
//     if (w == 0 || h == 0) {
//         SDL_QueryTexture(it->second.get(), nullptr, nullptr, &dstRect.w, &dstRect.h);
//     }

//     SDL_RenderCopyEx(renderer, it->second.get(), nullptr, &dstRect, angle, nullptr, flip);
// }