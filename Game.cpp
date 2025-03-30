#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <string>
#include "SaveLoad.h" // Include the SaveLoad header
#include <sstream>
using namespace std;
// Global variables (moved from main.cpp)
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font* font;
extern bool running;
extern vector<class Wall> walls;

extern int enemyNumber;
extern int maxScore;
extern vector<class EnemyTank> enemies;
extern vector<class Explosion> explosions;
extern int score;
extern double timee; // Avoid name conflict with std::time
extern int level;
extern bool gameStarted;
extern bool inMenu;
extern SDL_Texture* startButtonTexture;
extern SDL_Texture* instructionButtonTexture;
extern SDL_Texture* instructionTextTexture;
extern SDL_Texture* continueButtonTexture;
extern SDL_Texture* pauseTexture;
extern bool showInstructions;
//SDL_Texture* saveButtonTexture = nullptr;
extern bool gameOver;
extern bool gameWon;
extern SDL_Texture* winTexture;
extern SDL_Texture* loseTexture;
extern SDL_Texture* playAgainText;
extern SDL_Texture *playerTexture;
extern SDL_Texture *enemyTexture;
extern SDL_Texture *bulletTexture;
extern SDL_Texture *wallTexture;
extern SDL_Texture *wall2Texture;
extern SDL_Texture *wall3Texture;
extern Mix_Chunk *fireSound;
extern Mix_Chunk *explosionSound;
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
        for (int i = 0;i<=3; ++i) {
            if (!explosionTextures[i]) {
                cerr << "Failed to load explosion texture " << i + 1 << endl;
                running = false;
            }
        }
        gameStarted = false;

         // Tạo texture cho nút Start
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start Game", textColor);
        startButtonTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_FreeSurface(startSurface);
        SDL_Surface* continueSurface = TTF_RenderText_Solid(font, "Continue", textColor);
        continueButtonTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
        SDL_FreeSurface(continueSurface);

        // Tạo texture cho nút Instructions
        SDL_Surface* instructionSurface = TTF_RenderText_Solid(font, "Instructions", textColor);
        instructionButtonTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);

        // Tạo texture cho hướng dẫn
        SDL_Surface* instructionsTextSurface = TTF_RenderText_Blended_Wrapped(font, "Su dung cuc nut huong de di chuyen. An space đe ban. Ton tai va co gang huy diet tat ca xe tang dich trong 3 man trong 2 phut!", textColor, 600);
        instructionTextTexture = SDL_CreateTextureFromSurface(renderer, instructionsTextSurface);
        SDL_FreeSurface(instructionsTextSurface);

        //Create Win & Lose Texture
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
        if (!explosionSound) {
        std::cerr << "Failed to load explosion sound: " << Mix_GetError() << std::endl;
        }
        winSound=Mix_LoadMUS("win.mp3");
        loseSound=Mix_LoadMUS("lose.mp3");
        SDL_Surface* playAgainSurface = TTF_RenderText_Solid(font,"Play Again!",textColor);
        playAgainText = SDL_CreateTextureFromSurface(renderer,playAgainSurface);
        SDL_FreeSurface(playAgainSurface);
        generateWalls();
        player = PlayerTank((800 / 40 - 1) / 2 * 40, (600 / 40 - 2) * 40);
        spawnEnemies();
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
    void Game::updateEnemyTanks() {
        for (auto &enemy : enemies) {
            //enemy.changeDirection(walls);
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
                        inMenu = false;
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
                        spawnEnemies();
                        score = 0;
                        timee = 0;
                        level = 1;
                    inMenu = false; // Bắt đầu trò chơi
                     showInstructions = false;
                }
                 SDL_Rect continueButtonRect = {800 / 2 - 50, 600 / 2 , 100, 40};
                 if (mouseX >= continueButtonRect.x && mouseX <= continueButtonRect.x + continueButtonRect.w &&
                mouseY >= continueButtonRect.y && mouseY <= continueButtonRect.y + continueButtonRect.h) {
                     // Attempt to load the game
                    if (LoadGame(*this, "save.txt")) {
                        // If the game loads successfully, set inMenu to false
                        inMenu = false;
                        showInstructions = false;
                    } else {
                        // Handle the case where loading the game fails.  For example:
                        cout << "No saved game found, starting a new game." << endl;
                    }
                }

                // Kiểm tra xem chuột có nhấp vào nút Instructions không
                SDL_Rect instructionButtonRect = {800 / 2 - 50, 600 / 2 + 60, 100, 40};
                if (mouseX >= instructionButtonRect.x && mouseX <= instructionButtonRect.x + instructionButtonRect.w &&
                    mouseY >= instructionButtonRect.y && mouseY <= instructionButtonRect.y + instructionButtonRect.h) {
                     showInstructions = true;
                }
                   SDL_Rect saveButtonRect = {800 / 2 - 50, 600 / 2 + 120, 100, 40};

            }
        }
        else
        {
             if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (mouseX >= pauseButtonRect.x && mouseX <= pauseButtonRect.x + pauseButtonRect.w &&
                        mouseY >= pauseButtonRect.y && mouseY <= pauseButtonRect.y + pauseButtonRect.h) {
                            SaveGame(*this, "save.txt");
                    inMenu = true; // Trở về menu
                     showInstructions = false;
                }
            }
        }
    }
    // Di chuyển dựa trên trạng thái phím
    if (!inMenu && !gameOver) {
            if (keystate[SDL_SCANCODE_UP]) {
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
            if (keystate[SDL_SCANCODE_SPACE]) {
                player.shoot();
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
            if (gameWon) {
                SDL_RenderCopy(renderer,winTexture,NULL,&backgroundEndGame);
                SDL_Rect scoreEndRect = {800 / 2 - 50, 600 / 2 - 20, 100, 40};
            } else {
                SDL_RenderCopy(renderer,loseTexture,NULL,&backgroundEndGame);
                SDL_Rect scoreEndRect = {800 / 2 - 50, 600 / 2 - 20, 100, 40};
            }

            SDL_Rect playAgainRect = {800 / 2 - 50, 600 / 2 + 160, 100, 40};
            SDL_RenderCopy(renderer, playAgainText, NULL, &playAgainRect);
        }
        else if (inMenu) {
            // Vẽ background menu
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect backgroundRect = {0, 0, 800+4*40, 600};
            SDL_Texture* menuText=IMG_LoadTexture(renderer,"menu.png");
            //SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_RenderCopy(renderer,menuText,NULL,&backgroundRect);
             SDL_Rect startButtonRect = {800 / 2 - 50, 600 / 2 - 60, 100, 40};
        SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);

        // Vẽ nút Continue
        SDL_Rect continueButtonRect = {800 / 2 - 50, 600 / 2 , 100, 40}; // vị trí giữa startButtonRect và instructionButtonRect
        SDL_RenderCopy(renderer, continueButtonTexture, NULL, &continueButtonRect);

        // Vẽ nút Instructions
        SDL_Rect instructionButtonRect = {800 / 2 - 50, 600 / 2 + 60, 100, 40};
        SDL_RenderCopy(renderer, instructionButtonTexture, NULL, &instructionButtonRect);
            // Hiển thị hướng dẫn nếu nút hướng dẫn được nhấp
            if (showInstructions) {
                SDL_Rect instructionTextRect = {800 / 2 - 300, 600 / 2 + 150,
                0, 80};
                SDL_RenderCopy(renderer, instructionTextTexture, NULL, &instructionTextRect);
            }
            //SDL_Rect saveButtonRect = {800 / 2 - 50, 600 / 2 + 120, 100, 40}; // Điều chỉnh vị trí
             //SDL_RenderCopy(renderer, saveButtonTexture, NULL, &saveButtonRect);
        } else {
            SDL_Rect backgroundRect = {0, 0, 800-40, 600};
            SDL_Texture* menuText=IMG_LoadTexture(renderer,"background.jpg");
            SDL_Texture* boardText=IMG_LoadTexture(renderer,"board.jpg");
            SDL_Rect boardRect = {800-40, 0,40*5, 600};
            SDL_RenderCopy(renderer,boardText,NULL,&boardRect);
            //SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_RenderCopy(renderer,menuText,NULL,&backgroundRect);
            for (int i = 0; i < walls.size(); i++)
                walls[i].render(renderer, wallTexture, wall2Texture, wall3Texture);
            player.render(renderer, playerTexture);
            for (auto &enemy : enemies) {
                enemy.render(renderer, enemyTexture);
            }
            SDL_Rect pauseRect={760,0,40,40};
            SDL_RenderCopy(renderer,pauseTexture,NULL,&pauseRect);

            // Render các vụ nổ
            for (auto &explosion : explosions) {
                explosion.render(renderer, explosionTextures);
            }
        SDL_Texture* scoreTexture = createScoreTexture();
        if (scoreTexture) {
            // Tạo rect để định vị texture
            SDL_Rect scoreRect = {780, 80, 160,120 }; // Vị trí ở góc trên bên trái

            // Vẽ texture điểm số
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

            // Giải phóng texture sau khi sử dụng
            SDL_DestroyTexture(scoreTexture);
        }
        SDL_Texture* timeTexture = createTimeTexture();
        if (timeTexture) {
            // Tạo rect để định vị texture
            SDL_Rect timeRect = {780, 280, 160,120 }; // Vị trí ở góc trên bên trái

            // Vẽ texture điểm số
            SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);

            // Giải phóng texture sau khi sử dụng
            SDL_DestroyTexture(timeTexture);
        }
        SDL_Texture* maxScoreTexture = createMaxScoreTexture();
        if (maxScoreTexture) {
            // Tạo rect để định vị texture
            SDL_Rect maxScoreRect = {780, 480, 160,120 }; // Vị trí ở góc trên bên trái

            // Vẽ texture điểm số
            SDL_RenderCopy(renderer, maxScoreTexture, NULL, &maxScoreRect);

            // Giải phóng texture sau khi sử dụng
            SDL_DestroyTexture(maxScoreTexture);
        }
        }

        SDL_RenderPresent(renderer);
    }

    void Game::update() {
        if (!inMenu && !gameOver) {
            player.updateBullets();
            updateEnemyTanks();
            player.cooldown-=15;
            for (auto &enemy : enemies) {
                enemy.move(walls,level);
                enemy.updateBullets();
                if (rand() % 100 < (2+level)) {
                    enemy.shoot(level);
                }
            }

            // Xử lý va chạm đạn của địch với tường
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

            // Xử lý va chạm đạn của người chơi với tường
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

            // Xử lý va chạm đạn của người chơi với địch
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
                        return;
                    }
                }
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

            // Cập nhật trạng thái các vụ nổ
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
            //for (auto &enemy : enemies)
            //cout<<enemy.bullets.size()<<'\n';
            if(!inMenu&&!gameOver) timee+=0.015;
        }

    }

    Game::~Game() {
        SDL_DestroyTexture(startButtonTexture);
        SDL_DestroyTexture(instructionButtonTexture);
        SDL_DestroyTexture(instructionTextTexture);
        SDL_DestroyTexture(winTexture);
        SDL_DestroyTexture(loseTexture);
        SDL_DestroyTexture(playAgainText);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
