#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

using namespace std;

class Draw {
private:
    struct Slider{
        float currentX;    // Vị trí hiện tại
        float targetX;     // Vị trí đích
        bool isVisible = true;    // Trạng thái hiển thị
        bool isAnimating;  // Đang animating
        int value;         // Giá trị slider (0-100)
        Uint32 startTime;  // Thời điểm bắt đầu animation
        Uint32 ANIMATION_DURATION = 500;
    };
protected:
    SDL_Renderer* renderer;
    map<string, shared_ptr<SDL_Texture>> textures; 
public:
    Draw();
    Draw(SDL_Renderer* renderer);
    ~Draw();
    int BTN_HEIGHT = 40;
    int BTN_WIDTH = 120;
    int SCREEN_HEIGHT = 720;
    int SCREEN_WIDTH = 1280;
    void loadTexture(const string& key, const string& filePath);
    void drawTexture(const string& key, int x, int y, int w = 0, int h = 0, bool setLogicalSize = true);
    void renderText(string text, SDL_Rect destination, TTF_Font* typeFont);

    Slider slider;
    bool clickListBtn(int x, int y);
    bool clickInfoBtn(int x, int y);
    bool clickInfo_BackBtn(int x, int y);
    // void openListBtn(Uint32 currentTime);
    void renderListBtn(int coin, TTF_Font* typeFont);
    void toggleIsVisible() {slider.isVisible = !slider.isVisible;}
    bool getIsVisible() const {return slider.isVisible;}
    // void updateSlider(Slider* slider, Uint32 currentTime);
    // // Vẽ một phần của texture (dành cho sprite sheet)
    // void drawTexturePart(const string& key, int x, int y, int srcX, int srcY, int srcW, int srcH, int dstW = 0, int dstH = 0);

    // // Vẽ texture với xoay và lật
    // void drawTextureEx(const string& key, int x, int y, int w, int h, double angle, SDL_RendererFlip flip = SDL_FLIP_NONE);
};