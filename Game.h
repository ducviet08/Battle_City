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
    SDL_Texture* trapTexture = nullptr;       // Texture cho bẫy (khi hiển thị)
    SDL_Rect trapRect;                      // Vị trí và kích thước của bẫy ẩn
    bool trapActive = false;                // Bẫy có đang được đặt (ẩn) không?
    double trapSpawnTimer = 0.0;            // Bộ đếm thời gian để spawn bẫy tiếp theo
    const double TRAP_SPAWN_INTERVAL = 10.0;  // Thời gian giữa các lần spawn (10 giây)
    const int TRAP_SIZE = 35;               // Kích thước bẫy (pixel) - nhỏ hơn tank một chút

    // --- Biến trạng thái Stun cho người chơi ---
    bool player1Stunned = false;            // Player 1 có đang bị choáng không?
    double player1StunTimer = 0.0;          // Thời gian choáng còn lại của Player 1
    bool player2Stunned = false;            // Player 2 có đang bị choáng không?
    double player2StunTimer = 0.0;          // Thời gian choáng còn lại của Player 2
    const double STUN_DURATION = 3.0;
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
    void updateTrapsAndStun(double dt);
    void spawnTrap();
    void applyStun(PlayerTank& targetPlayer, bool& stunFlag, double& stunTimer);
};

#endif
