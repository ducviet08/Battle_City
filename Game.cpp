#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <string>
#include "SaveLoad.h"
#include <sstream>
using namespace std;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font* font;
extern bool running;
extern vector<class Wall> walls;

extern int enemyNumber;
extern bool gamePlayer1;
extern int maxScore;
extern vector<class EnemyTank> enemies;
extern vector<class Explosion> explosions;
extern int score;
extern double timee;
extern int level;
extern bool gameStarted;
extern bool inMenu;
extern SDL_Texture* menuText;
extern SDL_Texture* backgroundText;
extern SDL_Texture* boardText;
extern SDL_Texture* startButtonTexture;
extern SDL_Texture* player2ButtonTexture;
extern SDL_Texture* continueButtonTexture;
extern SDL_Texture* pauseTexture;
extern SDL_Texture* health1Texture;
extern SDL_Texture* health2Texture;
extern SDL_Texture* health3Texture;
//SDL_Texture* saveButtonTexture = nullptr;
extern bool gameOver;
extern bool gameWon;
extern SDL_Texture* win1Texture;
extern SDL_Texture* win2Texture;
extern SDL_Texture* winTexture;
extern SDL_Texture* loseTexture;
extern SDL_Texture* playAgainText;
extern SDL_Texture *playerTexture;
extern SDL_Texture *player2Texture;
extern SDL_Texture *enemyTexture;
extern SDL_Texture *bulletTexture;
extern SDL_Texture *wallTexture;
extern SDL_Texture *wall2Texture;
extern SDL_Texture *wall3Texture;
extern Mix_Chunk *fireSound;
extern Mix_Chunk *explosionSound;
extern Mix_Chunk *healthSound;
extern Mix_Chunk *trapSound;
extern Mix_Music *winSound;
extern Mix_Music *loseSound;
extern SDL_Texture *explosionTextures[4];
extern vector<int> wall1x;
extern vector<int> wall1y;
extern vector<int> wall2x;
extern vector<int> wall2y;
extern vector<int> wall3x;
extern vector<int> wall3y;
extern PlayerTank player;
extern PlayerTank player2;
extern SDL_Texture *shieldTexture; // Texture cho giáp
extern bool player1ShieldActive;   // Trạng thái giáp
extern double player1ShieldTimer;  // Thời gian còn lại của giáp

//Hàm khởi tạo tường
void Game::generateWalls() {
    for (int i = 0; i < wall1x.size(); i++)
        walls.push_back(Wall(wall1x[i] * 10, wall1y[i] * 10, 0));
    for (int i = 0; i < wall2x.size(); i++)
        walls.push_back(Wall(wall2x[i] * 10, wall2y[i] * 10, 1));
    for (int i = 0; i < wall3x.size(); i++)
        walls.push_back(Wall(wall3x[i] * 10, wall3y[i] * 10, 2));
}

Game::Game() {
        // Removed initializations of global variables
         if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
            running = false;
            return;
        }
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cerr << "Error" << SDL_GetError() << endl;
            running = false;
        }
        window = SDL_CreateWindow("Battle_City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800 + 4 * 40, 600, SDL_WINDOW_SHOWN);
        if (!window) {
            cerr << "Window no creater" << SDL_GetError << endl;
            running = 0;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cerr << "Renderer no creater" << SDL_GetError() << endl;
            running = 0;
        }
         font = TTF_OpenFont("type.ttf", 24);
         if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            running = false;
            return;
        }
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
            running = false;
            return;
        }
        if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
            std::cerr << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
            running = false;
        return;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not open audio: " << Mix_GetError() << std::endl;
        running = false;
        return;
        }
        pauseTexture=IMG_LoadTexture(renderer,"pause.png");
        playerTexture = IMG_LoadTexture(renderer, "playerTank.png");
        if (!playerTexture) {
            cerr << "loi";
        }
        shieldTexture = IMG_LoadTexture(renderer, "protect.png");
        if (!shieldTexture) {
          cerr << "Failed to load shield texture: protect.png" << endl;
         }
         player1ShieldActive = false; // Ban đầu không có giáp
         player1ShieldTimer = 0.0;
        healthPackTexture = IMG_LoadTexture(renderer, "health.png");
        win1Texture=IMG_LoadTexture(renderer,"win1.png");
        win2Texture=IMG_LoadTexture(renderer,"win2.png");
        player2Texture= IMG_LoadTexture(renderer,"enemyTank.png");
        enemyTexture = IMG_LoadTexture(renderer, "enemyTank.png");
        if (!enemyTexture) {
            cerr << "loi";
        }
        bulletTexture = IMG_LoadTexture(renderer, "bullet.png");
        if (!bulletTexture) {
            cerr << "loi";
        }
        wallTexture = IMG_LoadTexture(renderer, "wall.png");
        if (!wallTexture) {
            cerr << "loi";
        }
        wall2Texture = IMG_LoadTexture(renderer, "wall2.jpg");
        if (!wall2Texture) {
            cerr << "loi2";
        }
        wall3Texture = IMG_LoadTexture(renderer, "wall3.png");
        if (!wall3Texture) {
            cerr << "loi3";
        }

        // Load các texture nổ
        explosionTextures[0] = IMG_LoadTexture(renderer, "explosion1.png");
        explosionTextures[1] = IMG_LoadTexture(renderer, "explosion2.png");
        explosionTextures[2] = IMG_LoadTexture(renderer, "explosion3.png");
        explosionTextures[3] = IMG_LoadTexture(renderer, "explosion4.png");
        health1Texture=IMG_LoadTexture(renderer, "health1.png");
        health2Texture=IMG_LoadTexture(renderer, "health2.png");
        health3Texture=IMG_LoadTexture(renderer, "health3.png");
        for (int i = 0;i<=3; ++i) {
            if (!explosionTextures[i]) {
                cerr << "Failed to load explosion texture " << i + 1 << endl;
                running = false;
            }
        }
        gameStarted = false;
        game2player=0;

         // Tạo texture cho nút Start
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* startSurface = TTF_RenderText_Solid(font, "1 Player", textColor);
        startButtonTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_FreeSurface(startSurface);
        SDL_Surface* continueSurface = TTF_RenderText_Solid(font, "Continue", textColor);
        continueButtonTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
        SDL_FreeSurface(continueSurface);

        // Tạo texture cho nút instructions
        SDL_Surface* player2Surface = TTF_RenderText_Solid(font, "2 Player", textColor);
        player2ButtonTexture = SDL_CreateTextureFromSurface(renderer, player2Surface);
        SDL_FreeSurface(player2Surface);

        //Create Win & Lose Texture
        trapTexture = IMG_LoadTexture(renderer, "trap.png");
        winTexture=IMG_LoadTexture(renderer,"win.png");
        loseTexture=IMG_LoadTexture(renderer,"lose.png");
        SDL_Surface* saveSurface = TTF_RenderText_Solid(font, "Save Game", textColor);
        //saveButtonTexture = SDL_CreateTextureFromSurface(renderer, saveSurface);
        //SDL_FreeSurface(saveSurface);
        fireSound = Mix_LoadWAV("shoot.wav"); // Hoặc "fire.mp3", "fire.ogg", v.v.
        if (!fireSound) {
        std::cerr << "Failed to load fire sound: " << Mix_GetError() << std::endl;
        }

        explosionSound = Mix_LoadWAV("explosion.mp3");
        trapSound=Mix_LoadWAV("trap.mp3");
        healthSound=Mix_LoadWAV("pop.mp3");
        if (!explosionSound) {
        std::cerr << "Failed to load explosion sound: " << Mix_GetError() << std::endl;
        }
        menuText=IMG_LoadTexture(renderer,"menu.png");
        backgroundText=IMG_LoadTexture(renderer,"background.jpg");
        boardText=IMG_LoadTexture(renderer,"board.jpg");
        winSound=Mix_LoadMUS("win.mp3");
        loseSound=Mix_LoadMUS("lose.mp3");
        SDL_Surface* playAgainSurface = TTF_RenderText_Solid(font,"Menu",textColor);
        playAgainText = SDL_CreateTextureFromSurface(renderer,playAgainSurface);
        SDL_FreeSurface(playAgainSurface);
        healthPackActive = false;       // Ban đầu không có cục máu
        healthPackSpawnTimer = 0.0;     // Bắt đầu đếm thời gian
        healthPackRect.w = 40; // Đặt kích thước
         healthPackRect.h = 40;
         trapActive = false;             // Ban đầu không có bẫy
         trapSpawnTimer = 0.0;           // Bắt đầu đếm thời gian
         trapRect.w = TRAP_SIZE;         // Đặt kích thước
         trapRect.h = TRAP_SIZE;
         player1Stunned = false;
         player1StunTimer = 0.0;
         player2Stunned = false;
         player2StunTimer = 0.0;
        generateWalls();
        player = PlayerTank((800 / 40 - 1) / 2 * 40, (600 / 40 - 2) * 40);
        player2= PlayerTank((800 / 40 - 1) / 2 * 40, 0);
    }
    SDL_Texture *Game::createScoreTexture()
    {
        string scoreText="Score: "+to_string(score);
        SDL_Color textColor={255,255,255,255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    SDL_Texture *Game::createTimeTexture()
    {
        string timeText="Time: "+to_string(int(60-timee));
        SDL_Color textColor={255,255,255,255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    SDL_Texture *Game::createMaxScoreTexture()
    {
        string maxScoreText="Max Score: "+to_string(maxScore);
        SDL_Color textColor={255,255,255,255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, maxScoreText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    //Cập nhật hướng, di chuyển và bắn đạn của tank địch
    void Game::updateEnemyTanks() {
        for (auto &enemy : enemies) {
            enemy.directionChangeTimer++;
            if(enemy.directionChangeTimer==130)
            {
                enemy.directionChangeTimer=0;
                int r = rand() % 4;
                enemy.dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
                enemy.dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
            }
            int dx = player.x - enemy.x;
            int dy = player.y - enemy.y;

            if ((dx == 0 || dy == 0) && (rand() % 100 < 5+level)) {
                enemy.shoot(level);
            }

            int newX = enemy.x + enemy.dirX * 5;
            int newY = enemy.y + enemy.dirY * 5;
            SDL_Rect newRect = {newX, newY, 40, 40};

            bool collision = false;
            for (const auto &wall : walls) {
                if ((wall.active && SDL_HasIntersection(&newRect, &wall.rect))) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                enemy.move(walls,level);
            } else {
                enemy.shoot(level);
                enemy.directionChangeTimer=0;
                int r = rand() % 4;
                enemy.dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
                enemy.dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
            }
        }
    }
    void Game::Events() {
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL); // Lấy trạng thái bàn phím
    SDL_Rect pauseButtonRect = {760,0,40, 40};

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = 0;

        if (gameOver) {
             if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // Kiểm tra xem chuột có nhấp vào nút Play Again không
                    SDL_Rect playAgainRect = {800 / 2 - 50, 600 / 2 +160, 100, 40};
                    if (mouseX >= playAgainRect.x && mouseX <= playAgainRect.x + playAgainRect.w &&
                        mouseY >= playAgainRect.y && mouseY <= playAgainRect.y + playAgainRect.h) {
                        // Reset game state

                        gameOver = false; // Đặt lại trạng thái gameOver
                        inMenu = true;
                        game2player=false;
                        //level = 1;
                    }
                }
                return; // Không xử lý các sự kiện khác khi trò chơi kết thúc
        }
        if (inMenu) {
             if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Kiểm tra xem chuột có nhấp vào nút Start Game không
                SDL_Rect startButtonRect = {800 / 2 - 50, 600 / 2 - 60, 100, 40};
                if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                    mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h) {
                        gameStarted = true;
                        LoadGame(*this,"save.txt");
                        walls.clear();
                        generateWalls();
                        player = PlayerTank((800 / 40 - 1) / 2 * 40, (600 / 40 - 2) * 40);
                        enemyNumber = 3;
                        score = 0;
                        timee = 0;
                        level = 1;
                        spawnEnemies();
                    inMenu = false;
                    game2player =false;
                }
                 SDL_Rect continueButtonRect = {800 / 2 - 50, 600 / 2 , 100, 40};
                 if (mouseX >= continueButtonRect.x && mouseX <= continueButtonRect.x + continueButtonRect.w &&
                mouseY >= continueButtonRect.y && mouseY <= continueButtonRect.y + continueButtonRect.h) {
                     // Attempt to load the game
                        LoadGame(*this, "save.txt");
                        inMenu = false;
                        game2player=false;
                }

                // Kiểm tra xem chuột có nhấp vào nút Instructions không
                SDL_Rect player2ButtonRect = {800 / 2 - 50, 600 / 2 + 60, 100, 40};
                if (mouseX >= player2ButtonRect.x && mouseX <= player2ButtonRect.x + player2ButtonRect.w &&
                    mouseY >= player2ButtonRect.y && mouseY <= player2ButtonRect.y + player2ButtonRect.h) {
                   SDL_Rect saveButtonRect = {800 / 2 - 50, 600 / 2 + 120, 100, 40};
                   gameStarted=true;
                   inMenu=false;
                   game2player=true;
                   player = PlayerTank((800 / 40 - 1) / 2 * 40, (600 / 40 - 2) * 40);
                   player2= PlayerTank((800 / 40 - 1) / 2 * 40, 0);
                   player.health=3;
                   player2.health=3;
                   generateWalls();
                   healthPackActive = false;
                   }
            }
        }
        else
        {
             if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (mouseX >= pauseButtonRect.x && mouseX <= pauseButtonRect.x + pauseButtonRect.w &&
                        mouseY >= pauseButtonRect.y && mouseY <= pauseButtonRect.y + pauseButtonRect.h) {
                           if(!game2player) SaveGame(*this, "save.txt");
                    inMenu = true; // Trở về menu
                }
            }
        }
    }
    // Di chuyển dựa trên trạng thái phím
    if (!inMenu && !gameOver) {
            if (!player1Stunned){
            if (keystate[SDL_SCANCODE_UP]) {//mã quét
                player.dirX = 0;
                player.dirY = -1;
                player.move(0, -4, walls);
            } else if (keystate[SDL_SCANCODE_DOWN]) {
                player.dirX = 0;
                player.dirY = 1;
                player.move(0, 4, walls);
            } else if (keystate[SDL_SCANCODE_LEFT]) {
                player.dirX = -1;
                player.dirY = 0;
                player.move(-4, 0, walls);
            } else if (keystate[SDL_SCANCODE_RIGHT]) {
                player.dirX = 1;
                player.dirY = 0;
                player.move(4, 0, walls);
            }
            if (keystate[SDL_SCANCODE_RETURN]) {
                player.shoot();
            }
            }
            if(game2player)
            if (!player2Stunned){
            {
                if (keystate[SDL_SCANCODE_W]) {
                player2.dirX = 0;
                player2.dirY = -1;
                player2.move(0, -4, walls);
            } else if (keystate[SDL_SCANCODE_S]) {
                player2.dirX = 0;
                player2.dirY = 1;
                player2.move(0, 4, walls);
            } else if (keystate[SDL_SCANCODE_A]) {
                player2.dirX = -1;
                player2.dirY = 0;
                player2.move(-4, 0, walls);
            } else if (keystate[SDL_SCANCODE_D]) {
                player2.dirX = 1;
                player2.dirY = 0;
                player2.move(4, 0, walls);
            }
            if (keystate[SDL_SCANCODE_SPACE]) {
                player2.shoot();
            }
            }
            }
        }
}
// Game.cpp

void Game::spawnHealthPack() {
    bool validPosition = false;
    int spawnX, spawnY;
    int maxSpawnX = 800 - 40 - HEALTH_PACK_SIZE; // Giới hạn trong khu vực chơi (trừ bảng điểm)
    int maxSpawnY = 600 - HEALTH_PACK_SIZE;      // Giới hạn chiều cao

    SDL_Rect checkRect;
    checkRect.w = HEALTH_PACK_SIZE;
    checkRect.h = HEALTH_PACK_SIZE;

    int attempts = 0; // Giới hạn số lần thử để tránh vòng lặp vô hạn nếu map quá chật
    while (!validPosition && attempts < 100) {
        attempts++;
        // Tạo vị trí ngẫu nhiên (có thể cần điều chỉnh để khớp lưới nếu muốn)
        spawnX = rand() % maxSpawnX;
        spawnY = rand() % maxSpawnY;

        checkRect.x = spawnX;
        checkRect.y = spawnY;

        validPosition = true; // Giả sử vị trí hợp lệ ban đầu

        // Kiểm tra va chạm với tường
        for (const auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&checkRect, &wall.rect)) {
                validPosition = false; // Không hợp lệ nếu đè lên tường
                break;
            }
        }
    }

    if (validPosition) {
        healthPackRect.x = spawnX;
        healthPackRect.y = spawnY;
        healthPackActive = true; // Kích hoạt cục máu
        std::cout << "Health pack spawned at (" << spawnX << ", " << spawnY << ")" << std::endl; // Debug
    } else {
         std::cout << "Could not find valid spawn position for health pack after " << attempts << " attempts." << std::endl; // Debug
         // Nếu không tìm được vị trí, sẽ thử lại sau 10s nữa
         healthPackSpawnTimer = 0;
    }

}

void Game::updateHealthPack(double dt) {
    if (!game2player) return; // Chỉ chạy ở chế độ 2 người chơi

    // --- Xử lý Spawn ---
    if (!healthPackActive) {
        healthPackSpawnTimer += dt; // Tăng bộ đếm thời gian
        if (healthPackSpawnTimer >= HEALTH_PACK_SPAWN_INTERVAL) {
            healthPackSpawnTimer = 0.0; // Reset timer
            spawnHealthPack();        // Thử spawn cục máu
        }
    }

    // --- Xử lý Thu Thập ---
    if (healthPackActive) {
        // Kiểm tra va chạm với người chơi 1
        if (SDL_HasIntersection(&healthPackRect, &player.rect)) {
                Mix_PlayChannel(-1,healthSound,0);
            player.health = std::min(3, player.health + 1); // Hồi máu, tối đa 3
            healthPackActive = false;                       // Biến mất
            std::cout << "Player 1 collected health pack! Health: " << player.health << std::endl; // Debug
            return; // Chỉ một người chơi có thể nhặt trong một frame
        }

        // Kiểm tra va chạm với người chơi 2
        if (SDL_HasIntersection(&healthPackRect, &player2.rect)) {
            Mix_PlayChannel(-1,healthSound,0);
            player2.health = std::min(3, player2.health + 1); // Hồi máu, tối đa 3
            healthPackActive = false;                        // Biến mất
            std::cout << "Player 2 collected health pack! Health: " << player2.health << std::endl; // Debug
        }
    }
}

void Game::spawnTrap() {
    if (trapActive) return; // Chỉ spawn nếu chưa có bẫy nào đang active

    bool validPosition = false;
    int spawnX, spawnY;
    int maxSpawnX = 800 - 40 - TRAP_SIZE; // Giới hạn trong khu vực chơi
    int maxSpawnY = 600 - TRAP_SIZE;

    SDL_Rect checkRect;
    checkRect.w = TRAP_SIZE;
    checkRect.h = TRAP_SIZE;

    int attempts = 0;
    while (!validPosition && attempts < 100) {
        attempts++;
        spawnX = rand() % maxSpawnX;
        spawnY = rand() % maxSpawnY;

        checkRect.x = spawnX;
        checkRect.y = spawnY;
        validPosition = true;

        // Kiểm tra va chạm với tường
        for (const auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&checkRect, &wall.rect)) {
                validPosition = false;
                break;
            }
        }
    }

    if (validPosition) {
        trapRect.x = spawnX;
        trapRect.y = spawnY;
        trapActive = true; // Đặt bẫy (vẫn ẩn)
        trapSpawnTimer = 0.0; // Reset timer để đếm cho lần spawn tiếp theo
        std::cout << "Trap placed (hidden) at (" << spawnX << ", " << spawnY << ")" << std::endl; // Debug
    } else {
         std::cout << "Could not find valid spawn position for trap." << std::endl; // Debug
         // Không tìm được vị trí, sẽ thử lại sau interval nữa
    }
}
// Game.cpp

void Game::applyStun(PlayerTank& targetPlayer, bool& stunFlag, double& stunTimer) {
    if (stunFlag) return; // Đã bị stun rồi thì thôi

    stunFlag = true;
    stunTimer = STUN_DURATION;
    std::cout << "Player stunned!" << std::endl; // Debug
    // Mix_PlayChannel(-1, trapTriggerSound, 0); // Thêm âm thanh nếu có
}
// Game.cpp

void Game::updateTrapsAndStun(double dt) {

    // --- Xử lý Spawn Bẫy ---
    if (!trapActive) {
        trapSpawnTimer += dt;
        if (trapSpawnTimer >= TRAP_SPAWN_INTERVAL) {
             // Đảm bảo timer không tăng quá nhiều nếu có lag
             trapSpawnTimer = fmod(trapSpawnTimer, TRAP_SPAWN_INTERVAL);
            spawnTrap(); // Thử đặt bẫy mới
        }
    }

    // --- Xử lý Kích Hoạt Bẫy ---
    if (trapActive) {
        // Kiểm tra va chạm với người chơi 1 (nếu chưa bị stun)
        if (!player1Stunned && SDL_HasIntersection(&trapRect, &player.rect)) {
                Mix_PlayChannel(-1,trapSound,0);
            applyStun(player, player1Stunned, player1StunTimer);
            trapActive = false; // Bẫy đã bị kích hoạt và biến mất
            trapSpawnTimer = 0.0; // Bắt đầu đếm lại ngay cho bẫy tiếp theo
             std::cout << "Player 1 triggered trap!" << std::endl; // Debug
        }

        // Kiểm tra va chạm với người chơi 2 (nếu chưa bị stun và bẫy VẪN còn active)
        // Điều này có nghĩa là nếu cả 2 cùng chạm vào, cả 2 cùng bị stun
        if (trapActive && !player2Stunned && SDL_HasIntersection(&trapRect, &player2.rect)) {
                Mix_PlayChannel(-1,trapSound,0);
             applyStun(player2, player2Stunned, player2StunTimer);
             trapActive = false; // Bẫy đã bị kích hoạt và biến mất
             trapSpawnTimer = 0.0; // Bắt đầu đếm lại ngay cho bẫy tiếp theo
             std::cout << "Player 2 triggered trap!" << std::endl; // Debug
        }
    }

    // --- Cập nhật Thời gian Stun ---
    if (player1Stunned) {
        player1StunTimer -= dt;
        if (player1StunTimer <= 0.0) {
            player1Stunned = false;
            player1StunTimer = 0.0;
             std::cout << "Player 1 stun ended." << std::endl; // Debug
        }
    }
    if (player2Stunned) {
        player2StunTimer -= dt;
        if (player2StunTimer <= 0.0) {
            player2Stunned = false;
            player2StunTimer = 0.0;
            std::cout << "Player 2 stun ended." << std::endl; // Debug
        }
    }
}
    void Game::spawnEnemies() {
        enemies.clear();
        for (int i = 0; i < enemyNumber; ++i) {
            SDL_Rect checkEnemy;
            int ex, ey;
            bool validPosition = false;
            while (!validPosition) {
                ex = (rand() % (800 / 40 - 2) + 1) * 40;
                ey = (rand() % (600 / 40 - 2) + 1) * 40;
                validPosition = true;
                checkEnemy.x=ex;
                checkEnemy.y=ey;
                checkEnemy.h=checkEnemy.w=40;
                for (const auto &wall : walls) {
                    if  ((wall.active && SDL_HasIntersection(&checkEnemy, &wall.rect))||((abs(player.x-ex)<200)&&(abs(player.y-ey<200)))) {
                    validPosition = false;
                        break;
                    }
                }
            }
            enemies.push_back(EnemyTank(ex, ey,level));
        }
    }

    void Game::render() {
         SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
        SDL_RenderClear(renderer);

        if (gameOver) {
            //Vẽ background sau game
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_Rect backgroundEndGame={0,0,800,600};
            if(game2player)
            {
                if (gamePlayer1) {
                SDL_RenderCopy(renderer,win1Texture,NULL,&backgroundEndGame);
            } else {
                SDL_RenderCopy(renderer,win2Texture,NULL,&backgroundEndGame);
            }

            }
            else {
            if (gameWon) {
                SDL_RenderCopy(renderer,winTexture,NULL,&backgroundEndGame);
            } else {
                SDL_RenderCopy(renderer,loseTexture,NULL,&backgroundEndGame);
            }
            }

            SDL_Rect playAgainRect = {800 / 2 - 50, 600 / 2 + 160, 100, 40};
            SDL_RenderCopy(renderer, playAgainText, NULL, &playAgainRect);
        }
        else if (inMenu) {
            // Vẽ background menu
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect backgroundRect = {0, 0, 800+4*40, 600};

            //SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_RenderCopy(renderer,menuText,NULL,&backgroundRect);
             SDL_Rect startButtonRect = {800 / 2 - 50, 600 / 2 - 60, 100, 40};
        SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);

        // Vẽ nút Continue
        SDL_Rect continueButtonRect = {800 / 2 - 50, 600 / 2 , 100, 40};
        SDL_RenderCopy(renderer, continueButtonTexture, NULL, &continueButtonRect);

        // Vẽ nút Instructions
        SDL_Rect player2ButtonRect = {800 / 2 - 50, 600 / 2 + 60, 100, 40};
        SDL_RenderCopy(renderer, player2ButtonTexture, NULL, &player2ButtonRect);
        } else {
            SDL_Rect backgroundRect = {0, 0, 800-40, 600};
            SDL_Rect boardRect = {800-40, 0,40*5, 600};
            SDL_RenderCopy(renderer,boardText,NULL,&boardRect);
            //SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_RenderCopy(renderer,backgroundText,NULL,&backgroundRect);
            for (int i = 0; i < walls.size(); i++)
                walls[i].render(renderer, wallTexture, wall2Texture, wall3Texture);
            player.render(renderer, playerTexture);
            if(!game2player){
            for (auto &enemy : enemies)
                enemy.render(renderer, enemyTexture);
                SDL_Texture* scoreTexture = createScoreTexture();
        if (scoreTexture) {
            SDL_Rect scoreRect = {780, 80, 160,120 };
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            SDL_DestroyTexture(scoreTexture);
        }
        SDL_Texture* timeTexture = createTimeTexture();
        if (timeTexture) {
            SDL_Rect timeRect = {780, 280, 160,120 }; // Vị trí ở góc trên bên trái
            SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
            SDL_DestroyTexture(timeTexture);
        }
        SDL_Texture* maxScoreTexture = createMaxScoreTexture();
        if (maxScoreTexture) {
            SDL_Rect maxScoreRect = {780, 480, 160,120 }; // Vị trí ở góc trên bên trái
            SDL_RenderCopy(renderer, maxScoreTexture, NULL, &maxScoreRect);
            SDL_DestroyTexture(maxScoreTexture);
        }
            if (player1ShieldActive && shieldTexture) {
            SDL_Rect shieldRect = {player.x-5,player.y-5,50,50};
            SDL_RenderCopy(renderer, shieldTexture, NULL, &shieldRect);
        }
            }
            else
            {
                player2.render(renderer,player2Texture);
                SDL_Rect imagePlayer1={800,20,60,60};
                SDL_RenderCopy(renderer,playerTexture,nullptr,&imagePlayer1);
                SDL_Rect imagePlayer2={800,220,60,60};
                SDL_RenderCopy(renderer,player2Texture,nullptr,&imagePlayer2);
                SDL_Rect healthPlayer1Rect={780,100,120,40};
                SDL_Rect healthPlayer2Rect={780,300,120,40};
                if (healthPackActive && healthPackTexture) {
                SDL_RenderCopy(renderer, healthPackTexture, NULL, &healthPackRect);
            }
                if(player.health==3)
                {
                    SDL_RenderCopy(renderer,health1Texture,nullptr,&healthPlayer1Rect);
                }
                else if(player.health==2)
                {
                    SDL_RenderCopy(renderer,health2Texture,nullptr,&healthPlayer1Rect);
                }
                    else if(player.health==1)
                        SDL_RenderCopy(renderer,health3Texture,nullptr,&healthPlayer1Rect);
                if(player2.health==3)
                {
                    SDL_RenderCopy(renderer,health1Texture,nullptr,&healthPlayer2Rect);
                }
                else if(player2.health==2)
                {
                    SDL_RenderCopy(renderer,health2Texture,nullptr,&healthPlayer2Rect);
                }
                    else if(player2.health==1)
                        SDL_RenderCopy(renderer,health3Texture,nullptr,&healthPlayer2Rect);

        if (player2Stunned) {
                 SDL_Rect stunRect = {player2.x,player2.y,40,20};
                 SDL_RenderCopy(renderer, trapTexture, NULL, &stunRect);
             }
            }
            if (player1Stunned) {
             SDL_Rect stunRect = {player.x,player.y,40,20}; // Vẽ tại vị trí player
             // Có thể điều chỉnh vị trí/kích thước hiệu ứng
             SDL_RenderCopy(renderer, trapTexture, NULL, &stunRect);
            }
            SDL_Rect pauseRect={760,0,40,40};
            SDL_RenderCopy(renderer,pauseTexture,NULL,&pauseRect);

            // Render các vụ nổ
            for (auto &explosion : explosions) {
                explosion.render(renderer, explosionTextures);
            }

        }

        SDL_RenderPresent(renderer);
    }

    void Game::update() {
        if (!inMenu && !gameOver) {
                if(!game2player){
                    updateEnemyTanks();
                    for (auto &enemy : enemies) {
                enemy.move(walls,level);
                enemy.updateBullets();
                if (rand() % 100 < (2+level)) {
                    enemy.shoot(level);
                }
            }
            for (auto &enemy : enemies) {
                for (auto &bullet : enemy.bullets) {
                    for (auto &wall : walls) {
                        if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                            // Tạo hiệu ứng nổ tại vị trí va chạm
                            explosions.push_back(Explosion(bullet.x, bullet.y));

                            wall.health--;
                            if(wall.health == 0)
                                wall.active = false;
                            bullet.active = false;
                            break;
                        }
                    }
                }
            }
            for (auto &bullet : player.bullets) {
                for (auto &enemy : enemies) {
                    if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                            enemy.health--;
                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(enemy.x, enemy.y));
                        Mix_PlayChannel(-1, explosionSound, 0);
                        bullet.active = false;
                        if(enemy.health==0)
                        {
                        enemy.active = false;
                        score++;
                        if (rand() % 100 < 20) { // 20% cơ hội
                                if (!player1ShieldActive) { // Chỉ kích hoạt nếu chưa có
                                    player1ShieldActive = true;
                                    player1ShieldTimer = SHIELD_DURATION;
                                    std::cout << "Shield Activated!" << std::endl; // Debug
                                    Mix_PlayChannel(-1, healthSound, 0);
                                }
                            }
                        }
                    }
                }
            }
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                         [](EnemyTank &e) { return !e.active; }), enemies.end());

            if (enemies.empty()) {
                enemyNumber++;
                level++;
                spawnEnemies();
            }

            // Xử lý va chạm đạn của địch với người chơi
            for (auto &enemy : enemies) {
                for (auto &bullet : enemy.bullets) {
                    if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                            if (!player1ShieldActive) {

                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(player.x, player.y));
                        Mix_PlayMusic(loseSound,0);
                        gameOver = true;
                        gameWon = false;
                        walls.clear();
                        generateWalls();
                        player = PlayerTank((800 / 40 - 1) / 2 * 40, (600 / 40 - 2) * 40);
                        enemyNumber = 3;
                        score = 0;
                        timee = 0;
                        level = 1;
                        spawnEnemies();
                        SaveGame(*this,"save.txt");
                        return;}
                        else {
                            explosions.push_back(Explosion(player.x, player.y));
                        Mix_PlayMusic(loseSound,0);
                         bullet.active = false;
                        }
                    }
                }
            }
             for (auto &enemy : enemies)
                if(player1ShieldActive&&SDL_HasIntersection(&enemy.rect, &player.rect))
             {
                 score++;
                 enemy.active=false;
                 Mix_PlayChannel(-1, explosionSound, 0);
                 explosions.push_back(Explosion(player.x, player.y));
             }
              if(timee>=60){
                        gameOver = true;
                        gameWon = true;
                        Mix_PlayMusic( winSound, 0);
                        walls.clear();
                        generateWalls();
                        player = PlayerTank((800 / 40 - 1) / 2 * 40, (600 / 40 - 2) * 40);
                        enemyNumber = 3;
                        spawnEnemies();
                        timee = 0;
                        level = 1;
                        maxScore=max(score,maxScore);
                        score = 0;
                        SaveGame(*this,"save.txt");
                        return;
                    }
                }
                else
                {

                    player2.updateBullets();
                    player2.cooldown-=15;
                    for (auto &bullet : player2.bullets) {
                for (auto &wall : walls) {
                    if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(bullet.x, bullet.y));
                        Mix_PlayChannel(-1, explosionSound, 0);

                        wall.health--;
                        if (wall.health == 0)
                            wall.active = false;
                        bullet.active = false;
                    }
                }
            }
            for (auto &bullet : player2.bullets) {
                    if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(bullet.x, bullet.y));
                        Mix_PlayChannel(-1, explosionSound, 0);
                        player.health--;
                        if(player.health==0)
                        {
                            gameOver=true;
                            gamePlayer1=false;
                            Mix_PlayMusic( winSound, 0);
                            healthPackActive=0;

                        }
                        bullet.active = false;
                    }
            }
            for (auto &bullet : player.bullets) {
                    if (SDL_HasIntersection(&bullet.rect, &player2.rect)) {
                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(bullet.x, bullet.y));
                        Mix_PlayChannel(-1, explosionSound, 0);
                        player2.health--;
                        if(player2.health==0)
                        {
                            gameOver=true;
                            gamePlayer1=true;
                            Mix_PlayMusic( winSound, 0);
                            healthPackActive=0;
                        }
                        bullet.active = false;
                    }
            }
            updateHealthPack(0.015);
                }
            player.updateBullets();

            player.cooldown-=15;

            for (auto &bullet : player.bullets) {
                for (auto &wall : walls) {
                    if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(bullet.x, bullet.y));
                        Mix_PlayChannel(-1, explosionSound, 0);

                        wall.health--;
                        if (wall.health == 0)
                            wall.active = false;
                        bullet.active = false;
                    }
                }
            }
            if (player1ShieldActive) {
            player1ShieldTimer -= 0.015; // Giả sử dt ~ 0.015
            if (player1ShieldTimer <= 0.0) {
                player1ShieldActive = false;
                player1ShieldTimer = 0.0;
                std::cout << "Shield Expired." << std::endl; // Debug
            }
        }
            updateTrapsAndStun(0.015);
            for (auto &explosion : explosions) {
                explosion.update();
            }

            // Loại bỏ các vụ nổ đã hoàn thành
            explosions.erase(std::remove_if(explosions.begin(), explosions.end(),
                                             [](Explosion &e) { return !e.active; }), explosions.end());

        }
    }

    void Game::run() {
        while (running) {
            Events();
            update();
            render();

            SDL_Delay(15);
            if(!inMenu&&!gameOver) timee+=0.015;

        }

    }

    Game::~Game() {
        SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(continueButtonTexture);
    SDL_DestroyTexture(player2ButtonTexture);
    SDL_DestroyTexture(player2Texture);
    SDL_DestroyTexture(winTexture);
    SDL_DestroyTexture(loseTexture);
    SDL_DestroyTexture(playAgainText);
    SDL_DestroyTexture(health1Texture);
    SDL_DestroyTexture(health2Texture);
    SDL_DestroyTexture(health3Texture);
    SDL_DestroyTexture(win1Texture);
    SDL_DestroyTexture(win2Texture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(wall2Texture);
    SDL_DestroyTexture(wall3Texture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(trapTexture);
    SDL_DestroyTexture(healthPackTexture);
    SDL_DestroyTexture(shieldTexture);
    // Giải phóng các texture nổ
    for (int i = 0; i < 4; ++i) {
        SDL_DestroyTexture(explosionTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(fireSound);
    Mix_FreeChunk(explosionSound);
    Mix_FreeMusic(winSound);
    Mix_FreeMusic(loseSound);
    TTF_CloseFont(font);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    }
