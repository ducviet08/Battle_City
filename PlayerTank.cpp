#include "PlayerTank.h"
#include <algorithm>
#include "Wall.h"
#include<cmath>
#include<SDL_mixer.h>
extern SDL_Texture *bulletTexture;
PlayerTank::PlayerTank(int startX, int startY) {
    bool protect=false;
    x = startX;
    y = startY;
    rect = {x, y, 40, 40};
    dirX = 0;
    dirY = -1; // Default direction up
    cooldown = 0;
}

void PlayerTank::move(int dx, int dy, const std::vector<Wall> &walls) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;
    SDL_Rect newRect = {newX, newY, 40, 40};
    for (int i = 0; i < walls.size(); i++) // Corrected loop counter type
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect))
            return;
    if (newX >= 0 && newY >= 0 && newX <= 800 - 2 * 40 && newY <= 600 - 40) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void PlayerTank::shoot() {
    extern Mix_Chunk *fireSound;
    if(cooldown<=0)
    {
    bullets.push_back(Bullet(x + 40 / 2 - 5, y + 40 / 2 - 5,
                               this->dirX*2, this->dirY*2));
    cooldown=700;
    Mix_PlayChannel(-1,fireSound, 0);
    }
}

void PlayerTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void PlayerTank::render(SDL_Renderer *renderer, SDL_Texture *playerTexture) {
    double angle = atan2(dirY, dirX) * 180 / M_PI; // Tính góc xoay

    SDL_Point center;
    center.x = rect.w / 2;
    center.y = rect.h / 2;

    SDL_RenderCopyEx(renderer, playerTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    for (auto &bullet : bullets)
        bullet.render(renderer, bulletTexture);
}
