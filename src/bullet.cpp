#include "bullet.h"

Bullet::Bullet(int startX, int startY, float speed, shared_ptr<Enemy> target)
    : posX(startX), posY(startY), speed(speed), target(target) {
    rectBullet = {startX, startY, widthBullet, heightBullet};
    if (target) {
        targetX = target->getPosX() + target->getPosW() / 2;
        targetY = target->getPosY() + target->getPosH() / 2;
    }
}

void Bullet::update(Uint32 dT) {
    if (target) {
        // Cập nhật vị trí liên tục của target để nhắm tới
        targetX = target->getPosX() + target->getPosW() / 2;
        targetY = target->getPosY() + target->getPosH() / 2;
        float deltaX = targetX - posX;
        float deltaY = targetY - posY;
        float length = sqrt(deltaX * deltaX + deltaY * deltaY);

        if (length != 0) {
            dirX = deltaX / length;
            dirY = deltaY / length;
        } else {
            dirX = 0;
            dirY = 0;
        }

        float timeInSec = dT / 1000.0f;
        posX += dirX * speed * timeInSec;
        posY += dirY * speed * timeInSec;

        rectBullet.x = static_cast<int>(posX);
        rectBullet.y = static_cast<int>(posY);
    }else return;
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 204);
    SDL_RenderFillRect(renderer, &rectBullet);
}

bool Bullet::isBulletOutScreen() {
    return rectBullet.x < -10 || rectBullet.x > 1290 || rectBullet.y < -10 || rectBullet.y > 730;
}

bool Bullet::isEnemyFired() {
    if (!target) return false;
    SDL_Rect enemyRect = { target->getPosX(), target->getPosY(), target->getPosW(), target->getPosH()};
    return SDL_HasIntersection(&rectBullet, &enemyRect);
}

