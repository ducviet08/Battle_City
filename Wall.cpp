#include "Wall.h"

Wall::Wall(int startX, int startY, int kaka) {
    x = startX;
    y = startY;
    active = true;
    rect = {x, y, 40, 40};
    type = kaka;
    switch (type) {
        case 0:
            health = 2;
            break;
        case 1:
            health = 1;
            break;
        case 2:
            health = 1000;
            break;
    }
}

void Wall::render(SDL_Renderer *renderer, SDL_Texture *wallTexture, SDL_Texture *wall2Texture, SDL_Texture *wall3Texture) {
    if (active) {
        SDL_Texture *currentTexture = nullptr;

        switch (type) {
            case 0:
                currentTexture = wallTexture;
                break;
            case 1:
                currentTexture = wall2Texture;
                break;
            case 2:
                currentTexture = wall3Texture;
                break;
        }

        if (currentTexture) {
            SDL_RenderCopy(renderer, currentTexture, NULL, &rect);
        }
    }
}
