// drawNotification.h
#pragma once
#include "drawElement.h"
#include <vector>
#include <string>

using namespace std;

class Notification : public Draw {
private:
    struct Message {
        Uint32 startTime;
        string text;
        int x;
        int y;
        int w;
        int h;
        // int indexY;
        // bool isActive = true;
    };
    string key = "notification";
    // int tempIndexY = 0;
    Uint32 duration = 3000; // Thời gian hiển thị mặc định: 3 giây
    vector<Message> notifs;

public:
    Notification();
    Notification(SDL_Renderer* renderer); 
    void setNotif(string text, int x, int y, int w, int h, Uint32 startTime = 0); 
    void updateAndShow(TTF_Font* typeFont); // Gộp update và show thành một hàm
};