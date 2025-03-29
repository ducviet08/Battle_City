#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <SDL.h>

class Explosion {
public:
    int x, y;
    int currentFrame;
    int frameTimer;
    bool active;

    Explosion(int startX, int startY);
    void update();
    void render(SDL_Renderer *renderer, SDL_Texture *explosionTextures[]);
};

#endif
