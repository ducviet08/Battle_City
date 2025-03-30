#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>
#include "Wall.h"
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Explosion.h"
#include <SDL_mixer.h>

extern PlayerTank player;

class Game {
public:
    Game();
    ~Game();
    void run();
    void Events();
    void update();
    void render();
private:
    void generateWalls();
    void spawnEnemies();
    void updateEnemyTanks();
    SDL_Texture *createScoreTexture();
    SDL_Texture *createTimeTexture();
    SDL_Texture *createMaxScoreTexture();
};

#endif
