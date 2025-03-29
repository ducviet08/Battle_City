#include "Explosion.h"

Explosion::Explosion(int startX, int startY) {
    x = startX;
    y = startY;
    currentFrame = 0;
    frameTimer = 0;
    active = true;
}

void Explosion::update() {
    frameTimer++;
    if (frameTimer >= 5) { // Thay đổi frame sau mỗi 5 ticks
        frameTimer = 0;
        currentFrame++;
        if (currentFrame >= 4) { // 4 là số lượng frame của hiệu ứng nổ
            active = false;
        }
    }
}

void Explosion::render(SDL_Renderer *renderer, SDL_Texture *explosionTextures[]) {
    if (active) {
        SDL_Rect srcRect = {0, 0, 40, 40}; // Kích thước của mỗi frame nổ
        SDL_Rect destRect = {x-15 , y-20, 40, 40};
        SDL_RenderCopy(renderer, explosionTextures[currentFrame], &srcRect, &destRect);
    }
}
