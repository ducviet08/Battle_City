#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"

class Wall; // Forward declaration

class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    int health;
    SDL_Rect rect;
    bool active;
    int directionChangeTimer;
    std::vector<Bullet> bullets;

    EnemyTank(int startX, int startY, int Health);
    EnemyTank(int startX, int startY);
    void move(const std::vector<Wall> &walls,int a);
    void shoot(int a);
    void updateBullets();
    void render(SDL_Renderer *renderer, SDL_Texture *enemyTexture);
};

#endif
