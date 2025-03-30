// drawNotification.cpp
#include "drawNotification.h"

Notification::Notification(){};
// : Draw(nullptr){};

Notification::Notification(SDL_Renderer* renderer) : Draw(renderer) {
    // Khởi tạo renderer từ class cha Draw
    loadTexture("notification", "D:/laptrinh/LTNC/Code/GameProject/assets/notification.png");
}

void Notification::setNotif(string text, int x, int y, int w, int h, Uint32 startTime) {
    Message notif;
    // notif.indexY = tempIndexY;
    notif.text = text;
    notif.x = x;
    notif.y = y;
    //  + notif.indexY * y;
    notif.w = w;
    notif.h = h;
    notif.startTime = (startTime == 0) ? SDL_GetTicks() : startTime; // Nếu không truyền startTime, lấy thời gian hiện tại
    notifs.push_back(notif);
    // tempIndexY++;
}

void Notification::updateAndShow(TTF_Font* typeFont) {
    Uint32 currentTime = SDL_GetTicks();
    
    // Xóa các thông báo đã hết thời gian và vẽ các thông báo còn lại
    for (auto it = notifs.begin(); it != notifs.end();) {
        Uint32 passedTime = currentTime - it->startTime;
        if (passedTime > duration) {
            it = notifs.erase(it); // Xóa thông báo hết thời gian
            // it->indexY -= 1;
        } else {
            // Vẽ nền thông báo
            drawTexture(key, it->x, it->y, it->w, it->h);
            // Vẽ text, căn chỉnh trong khung
            SDL_Rect messDst = {it->x + 25, it->y + 18, 0, 0};
            renderText(it->text, messDst, typeFont);
            ++it;
        }
    }
}