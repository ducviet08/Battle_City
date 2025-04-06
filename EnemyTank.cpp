#include "EnemyTank.h"
#include <algorithm>
#include "Wall.h"
#include<cmath>
extern SDL_Texture *bulletTexture;
EnemyTank::EnemyTank(int startX, int startY,int Health) {
    health=Health;
    moveDelay = 15; // Delay for movement
    shootDelay = 5;  // Delay for shooting
    x = startX;
    y = startY;
    rect = {x, y, 32, 32};
    dirX = 0;
    dirY = 1;
    active = true;
    directionChangeTimer = 0;
}
EnemyTank::EnemyTank(int startX, int startY) {
    moveDelay = 15; // Delay for movement
    shootDelay = 5;  // Delay for shooting
    x = startX;
    y = startY;
    rect = {x, y, 32, 32};
    dirX = 0;
    dirY = 1;
    active = true;
    directionChangeTimer = 0;
}

void EnemyTank::move(const std::vector<Wall> &walls,int a) {
    if (--moveDelay > 0) return;
    moveDelay = 10;

    int newX = x + this->dirX * (5+a);
    int newY = y + this->dirY * (5+a);

    // Kiểm tra nếu xe tăng chạm biên cửa sổ
    if (newX < 0 || newX > 800 - 2*40 || newY < 0 || newY > 600 - 40) {
        // Đổi hướng ngẫu nhiên
        int r = rand() % 4;
        dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
        dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
        return; // Không di chuyển trong frame này
    }

    SDL_Rect newRect = {newX, newY, 40, 40};
    for (const auto &wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return;
        }
    }

    x = newX;
    y = newY;
    rect.x = x;
    rect.y = y;

}

void EnemyTank::shoot(int a) {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + 40 / 2 - 9, y + 40 / 2 - 5,
                               this->dirX*(1+a), this->dirY*(1+a)));
}

void EnemyTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void EnemyTank::render(SDL_Renderer *renderer, SDL_Texture *enemyTexture) {
    double angle = atan2(dirY, dirX) * 180 / M_PI;

    SDL_Point center;
    center.x = rect.w / 2;
    center.y = rect.h / 2;

    SDL_RenderCopyEx(renderer, enemyTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);

    for (auto &bullet : bullets) {
        bullet.render(renderer, bulletTexture);
    }
}
