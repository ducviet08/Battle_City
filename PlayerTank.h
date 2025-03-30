#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"

class Wall; // Forward declaration

class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    int health;
    SDL_Rect rect;
    std::vector<Bullet> bullets;
    int cooldown;

    PlayerTank(int startX, int startY);
    void move(int dx, int dy, const std::vector<Wall> &walls);
    void shoot();
    void updateBullets();
    void render(SDL_Renderer *renderer, SDL_Texture *playerTexture);
};

#endif
