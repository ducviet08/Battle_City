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
    bool game2player;
    SDL_Texture* healthPackTexture = nullptr;
    SDL_Rect healthPackRect;
    bool healthPackActive = false;            // Cục máu có đang hiển thị không?
    double healthPackSpawnTimer = 0.0;        // Bộ đếm thời gian để spawn
    const double HEALTH_PACK_SPAWN_INTERVAL = 10.0; // Thời gian giữa các lần spawn (10 giây)
    const int HEALTH_PACK_SIZE = 30;
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
    void updateHealthPack(double dt);
    void spawnHealthPack();
};

#endif
