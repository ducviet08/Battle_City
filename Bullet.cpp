#include "Bullet.h"
#include <cmath>

Bullet::Bullet(int startX, int startY, int dirX, int dirY) {
    x = startX;
    y = startY;
    dx = dirX;
    dy = dirY;
    active = true;
    rect = {x, y, 10, 10};
}

void Bullet::move() {
    x += dx;
    y += dy;
    rect.x = x;
    rect.y = y;
    if (x < 0 || x > 800 - 40 ||
        y < 0 || y > 600) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer *renderer, SDL_Texture *bulletTexture) {
    if (active) {
        double angle = atan2(dy, dx) * 180 / M_PI; // Tính góc xoay

        SDL_Point center;
        center.x = rect.w / 2;
        center.y = rect.h / 2;

        SDL_RenderCopyEx(renderer, bulletTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
}
