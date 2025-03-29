#ifndef WALL_H
#define WALL_H

#include <SDL.h>

class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    int health;
    int type;

    Wall(int startX, int startY, int kaka);
    void render(SDL_Renderer *renderer, SDL_Texture *wallTexture, SDL_Texture *wall2Texture, SDL_Texture *wall3Texture);
};

#endif
