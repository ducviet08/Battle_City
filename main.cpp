#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <utility>
#include <SDL_ttf.h>
#include <string>
#include <sdl_mixer.h>
#include <fstream>
#include <sstream> // For stringstream
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

// Global variables (moved from Game class)
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font* font = nullptr;
bool running = true;
vector<class Wall> walls;

int enemyNumber = 3;
vector<class EnemyTank> enemies;
vector<class Explosion> explosions;
int score = 0;
int timee = 0; // Avoid name conflict with std::time
int level = 1;
bool gameStarted = false;
bool inMenu = true;
SDL_Texture* startButtonTexture = nullptr;
SDL_Texture* instructionButtonTexture = nullptr;
SDL_Texture* instructionTextTexture = nullptr;
SDL_Texture* continueButtonTexture = nullptr;
SDL_Texture* pauseTexture = nullptr;
bool showInstructions = false;
//SDL_Texture* saveButtonTexture = nullptr;
bool gameOver = false;
bool gameWon = false;
SDL_Texture* winTexture = nullptr;
SDL_Texture* loseTexture = nullptr;
SDL_Texture* playAgainText = nullptr;
SDL_Texture *playerTexture = nullptr;
SDL_Texture *enemyTexture = nullptr;
SDL_Texture *bulletTexture = nullptr;
SDL_Texture *wallTexture = nullptr;
SDL_Texture *wall2Texture = nullptr;
SDL_Texture *wall3Texture = nullptr;
Mix_Chunk *fireSound = nullptr;
Mix_Chunk *explosionSound = nullptr;
Mix_Music *winSound=nullptr;
Mix_Music *loseSound = nullptr;
SDL_Texture *explosionTextures[4];

vector<int> wall1x = {12, 0, 4, 4, 8, 8, 8, 8, 8, 12, 12, 16, 20, 20, 20, 20, 24, 24, 24, 24, 28, 32, 32, 32, 32, 36, 36, 36, 36, 36, 40, 40, 44, 52, 52, 52, 52, 52, 52, 52, 52, 56, 56, 58, 60, 60, 62, 64, 64, 66, 68, 68, 72, 72, 72, 72, 72, 72, 72, 72};
vector<int> wall1y = {16, 12, 8, 16, 12, 20, 28, 32, 40, 28, 40, 40, 40, 36, 32, 28, 12, 16, 20, 24, 12, 12, 24, 36, 40, 12, 24, 36, 16, 20, 44, 48, 44, 4, 8, 12, 16, 20, 28, 32, 36, 38, 4, 8, 12, 38, 16, 12, 38, 8, 4, 38, 4, 8, 12, 16, 20, 28, 32, 36};
vector<int> wall2x = {0, 4, 8, 12, 16, 16, 28, 28, 32, 44};
vector<int> wall2y = {48, 12, 16, 36, 20, 32, 20, 32, 16, 4};
vector<int> wall3x = {0, 0, 0, 4, 4, 8, 8, 12, 12, 16, 16, 20, 24, 28, 28, 32, 32, 36, 36, 36, 40, 40, 40, 44, 44, 44, 44, 44, 48, 48, 56, 68, 58, 62, 66};
vector<int> wall3y = {44, 40, 36, 36, 48, 36, 48, 44, 48, 24, 28, 20, 32, 24, 28, 4, 8, 4, 40, 44, 40, 4, 16, 8, 12, 16, 48, 52, 48, 52, 24, 24, 34, 34, 34};

class Bullet {
public:
    int x, y;
    int dx, dy;
    SDL_Rect rect;
    bool active;

    Bullet(int startX, int startY, int dirX, int dirY) {
        x = startX;
        y = startY;
        dx = dirX;
        dy = dirY;
        active = true;
        rect = {x, y, 10, 10};
    }

    void move() {
        x += dx;
        y += dy;
        rect.x = x;
        rect.y = y;
        if (x < 0 || x > SCREEN_WIDTH - TILE_SIZE ||
            y < 0 || y > SCREEN_HEIGHT) {
            active = false;
        }
    }

    void render(SDL_Renderer *renderer) {
        if (active) {
            double angle = atan2(dy, dx) * 180 / M_PI; // Tính góc xoay

            SDL_Point center;
            center.x = rect.w / 2;
            center.y = rect.h / 2;

            SDL_RenderCopyEx(renderer, bulletTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
        }
    }
};

class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    int health;
    int type;
    Wall(int startX, int startY, int kaka) {
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

    void render(SDL_Renderer *renderer) {
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
};

// Lớp hiệu ứng nổ
class Explosion {
public:
    int x, y;
    int currentFrame;
    int frameTimer;
    bool active;

    Explosion(int startX, int startY) {
        x = startX;
        y = startY;
        currentFrame = 0;
        frameTimer = 0;
        active = true;
    }

    void update() {
        frameTimer++;
        if (frameTimer >= 5) { // Thay đổi frame sau mỗi 5 ticks
            frameTimer = 0;
            currentFrame++;
            if (currentFrame >= 4) { // 4 là số lượng frame của hiệu ứng nổ
                active = false;
            }
        }
    }

    void render(SDL_Renderer *renderer) {
        if (active) {
            SDL_Rect srcRect = {0, 0, 40, 40}; // Kích thước của mỗi frame nổ
            SDL_Rect destRect = {x-15 , y-20, 40, 40};
            SDL_RenderCopy(renderer, explosionTextures[currentFrame], &srcRect, &destRect);
        }
    }
};
class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    int directionChangeTimer = 0;
    std::vector<Bullet> bullets;

    EnemyTank(int startX, int startY) {
        moveDelay = 15; // Delay for movement
        shootDelay = 5;  // Delay for shooting
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = 1;
        active = true;
    }

    void move(const std::vector<Wall> &walls,int a) {
        if (--moveDelay > 0) return;
        moveDelay = 10;

        int newX = x + this->dirX * (5+a);
        int newY = y + this->dirY * (5+a);

        // Kiểm tra nếu xe tăng chạm biên cửa sổ
        if (newX < 0 || newX > SCREEN_WIDTH - 2*TILE_SIZE || newY < 0 || newY > SCREEN_HEIGHT - TILE_SIZE) {
            // Đổi hướng ngẫu nhiên
            int r = rand() % 4;
            dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
            dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
            return; // Không di chuyển trong frame này
        }

        SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
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

    void shoot(int a) {
        if (--shootDelay > 0) return;
        shootDelay = 5;
        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                                   this->dirX*(1+a), this->dirY*(1+a)));
    }

    void updateBullets() {
        for (auto &bullet : bullets) {
            bullet.move();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](Bullet &b) { return !b.active; }), bullets.end());
    }

    void render(SDL_Renderer *renderer) {
        double angle = atan2(dirY, dirX) * 180 / M_PI;

        SDL_Point center;
        center.x = rect.w / 2;
        center.y = rect.h / 2;

        SDL_RenderCopyEx(renderer, enemyTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);

        for (auto &bullet : bullets) {
            bullet.render(renderer);
        }
    }
};

class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet> bullets;
    int cooldown=0;
    PlayerTank(int startX, int startY) {
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1; // Default direction up
    }

    void move(int dx, int dy, const vector<Wall> &walls) {
        int newX = x + dx;
        int newY = y + dy;
        this->dirX = dx;
        this->dirY = dy;
        SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
        for (int i = 0; i < walls.size(); i++) // Corrected loop counter type
            if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect))
                return;
        if (newX >= 0 && newY >= 0 && newX <= SCREEN_WIDTH - 2 * TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE) {
            x = newX;
            y = newY;
            rect.x = x;
            rect.y = y;
        }
    }

    void shoot() {
        if(cooldown<=0)
        {
        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                                   this->dirX*2, this->dirY*2));
        cooldown=700;
        Mix_PlayChannel(-1,fireSound, 0);
        }
    }

    void updateBullets() {
        for (auto &bullet : bullets) {
            bullet.move();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](Bullet &b) { return !b.active; }), bullets.end());
    }

    void render(SDL_Renderer *renderer) {
        double angle = atan2(dirY, dirX) * 180 / M_PI; // Tính góc xoay

        SDL_Point center;
        center.x = rect.w / 2;
        center.y = rect.h / 2;

        SDL_RenderCopyEx(renderer, playerTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
        for (auto &bullet : bullets)
            bullet.render(renderer);
    }
};
PlayerTank player=PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);

// Forward declaration of Game class
class Game;

bool SaveGame(const Game& game, const string& filename);
bool LoadGame(Game& game, const std::string& filename);

// Lưu game sang file save.txt (moved outside Game class)
bool SaveGame(const Game& game, const string& filename) {
    ofstream file(filename); // Open file in text mode
    if (!file.is_open()) {
        cerr << "Failed to open file for saving: " << filename << endl;
        return false;
    }

    // Save general info
    file << "Score: " << score << endl;
    file << "Time: " << timee << endl;
    file << "Level: " << level << endl;
    file << "EnemyNumber: " << enemyNumber << endl;

    // Save player position
    file << "PlayerX: " << player.x << endl;
    file << "PlayerY: " << player.y << endl;

    // Save wall states
    file << "Walls: " << walls.size() << endl;
    for (const auto& wall : walls) {
        file << wall.active << " ";
    }
    file << endl;

    // Save enemy states and positions
    file << "Enemies: " << enemies.size() << endl;
    for (const auto& enemy : enemies) {
        file << enemy.active << " " << enemy.x << " " << enemy.y << endl;
    }

    file.close();
    cout << "Game saved to: " << filename << endl;
    return true;
}

// Load game từ file save.txt (moved outside Game class)
bool LoadGame(Game& game, const std::string& filename) {
    ifstream file(filename); // Open file in text mode
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return false;
    }

    string line;
    stringstream ss;

    // Load general info
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> score; ss.clear();
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> timee; ss.clear();
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> level; ss.clear();
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> enemyNumber; ss.clear();

    // Load player position
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> player.x; ss.clear();
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> player.y; ss.clear();

    // Load wall states
    size_t numWalls;
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> numWalls; ss.clear();

    if (numWalls == walls.size()) {
        getline(file, line);
        ss.str(line);
        for (size_t i = 0; i < walls.size(); ++i) {
            ss >> walls[i].active;
        }
        ss.clear();
    } else {
        std::cerr << "Wall count mismatch!" << std::endl;
    }

    // Load enemy states and positions
    size_t numEnemies;
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> numEnemies; ss.clear();

    enemies.clear();
    for (size_t i = 0; i < numEnemies; ++i) {
        bool active;
        int x, y;
        getline(file, line);
        ss.str(line);
        ss >> active >> x >> y;
        ss.clear();

        if (active) {
            EnemyTank enemy(x, y);
            enemy.active = true;
            enemies.push_back(enemy);
        }
    }

    file.close();
    std::cout << "Game loaded from: " << filename << std::endl;
    return true;
}



class Game {
public:
    // Removed declarations of global variables from here

    void generateWalls() {
        for (int i = 0; i < wall1x.size(); i++)
            walls.push_back(Wall(wall1x[i] * 10, wall1y[i] * 10, 0));
        for (int i = 0; i < wall2x.size(); i++)
            walls.push_back(Wall(wall2x[i] * 10, wall2y[i] * 10, 1));
        for (int i = 0; i < wall3x.size(); i++)
            walls.push_back(Wall(wall3x[i] * 10, wall3y[i] * 10, 2));
    }

    Game() {
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
        window = SDL_CreateWindow("Battle_City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH + 4 * TILE_SIZE, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
        fireSound = Mix_LoadWAV("shoot.mp3"); // Hoặc "fire.mp3", "fire.ogg", v.v.
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
        player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
        spawnEnemies();
    }
    SDL_Texture *createScoreTexture()
    {
        string scoreText="Score: "+to_string(score);
        SDL_Color textColor={255,255,255,255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    SDL_Texture *createTimeTexture()
    {
        string timeText="Time: "+to_string(timee/1000);
        SDL_Color textColor={255,255,255,255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return textTexture;
    }
    void updateEnemyTanks() {
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
            SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};

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
                enemy.directionChangeTimer=0;
                int r = rand() % 4;
                enemy.dirX = (r == 0) ? -1 : (r == 1) ? 1 : 0;
                enemy.dirY = (r == 2) ? -1 : (r == 3) ? 1 : 0;
            }
        }
    }
    void Events() {
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
                    SDL_Rect playAgainRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 40, 100, 40};
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
                SDL_Rect startButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 60, 100, 40};
                if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                    mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h) {
                        gameStarted = true;
                        walls.clear();
                        generateWalls();
                        player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
                        enemyNumber = 3;
                        spawnEnemies();
                        score = 0;
                        timee = 0;
                        level = 1;
                    inMenu = false; // Bắt đầu trò chơi
                     showInstructions = false;
                }
                 SDL_Rect continueButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 , 100, 40};
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
                SDL_Rect instructionButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, 100, 40};
                if (mouseX >= instructionButtonRect.x && mouseX <= instructionButtonRect.x + instructionButtonRect.w &&
                    mouseY >= instructionButtonRect.y && mouseY <= instructionButtonRect.y + instructionButtonRect.h) {
                     showInstructions = true;
                }
                   SDL_Rect saveButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 120, 100, 40};

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

    void spawnEnemies() {
        enemies.clear();
        for (int i = 0; i < enemyNumber; ++i) {
            SDL_Rect checkEnemy;
            int ex, ey;
            bool validPosition = false;
            while (!validPosition) {
                ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
                ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
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
            enemies.push_back(EnemyTank(ex, ey));
        }
    }

    void render() {
         SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
        SDL_RenderClear(renderer);

        if (gameOver) {
            //Vẽ background sau game
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_Rect backgroundEndGame={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
            if (gameWon) {
                SDL_RenderCopy(renderer,winTexture,NULL,&backgroundEndGame);
                SDL_Rect scoreEndRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 20, 100, 40};
            } else {
                SDL_RenderCopy(renderer,loseTexture,NULL,&backgroundEndGame);
                SDL_Rect scoreEndRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 20, 100, 40};
            }

            SDL_Rect playAgainRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 40, 100, 40};
            SDL_RenderCopy(renderer, playAgainText, NULL, &playAgainRect);
        }
        else if (inMenu) {
            // Vẽ background menu
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Texture* menuText=IMG_LoadTexture(renderer,"menu.png");
            //SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_RenderCopy(renderer,menuText,NULL,&backgroundRect);
             SDL_Rect startButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 60, 100, 40};
        SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);

        // Vẽ nút Continue
        SDL_Rect continueButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 , 100, 40}; // vị trí giữa startButtonRect và instructionButtonRect
        SDL_RenderCopy(renderer, continueButtonTexture, NULL, &continueButtonRect);

        // Vẽ nút Instructions
        SDL_Rect instructionButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, 100, 40};
        SDL_RenderCopy(renderer, instructionButtonTexture, NULL, &instructionButtonRect);
            // Hiển thị hướng dẫn nếu nút hướng dẫn được nhấp
            if (showInstructions) {
                SDL_Rect instructionTextRect = {SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 + 150, 600, 80};
                SDL_RenderCopy(renderer, instructionTextTexture, NULL, &instructionTextRect);
            }
            //SDL_Rect saveButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 120, 100, 40}; // Điều chỉnh vị trí
             //SDL_RenderCopy(renderer, saveButtonTexture, NULL, &saveButtonRect);
        } else {
            SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH-TILE_SIZE, SCREEN_HEIGHT};
            SDL_Texture* menuText=IMG_LoadTexture(renderer,"background.jpg");
            //SDL_RenderFillRect(renderer, &backgroundRect);
            SDL_RenderCopy(renderer,menuText,NULL,&backgroundRect);
            for (int i = 0; i < walls.size(); i++)
                walls[i].render(renderer);
            player.render(renderer);
            for (auto &enemy : enemies) {
                enemy.render(renderer);
            }
            SDL_Rect pauseRect={760,0,40,40};
            SDL_RenderCopy(renderer,pauseTexture,NULL,&pauseRect);

            // Render các vụ nổ
            for (auto &explosion : explosions) {
                explosion.render(renderer);
            }
                SDL_Texture* scoreTexture = createScoreTexture();
        if (scoreTexture) {
            // Tạo rect để định vị texture
            SDL_Rect scoreRect = {780, 100, 160,120 }; // Vị trí ở góc trên bên trái

            // Vẽ texture điểm số
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

            // Giải phóng texture sau khi sử dụng
            SDL_DestroyTexture(scoreTexture);
        }
        SDL_Texture* timeTexture = createTimeTexture();
        if (timeTexture) {
            // Tạo rect để định vị texture
            SDL_Rect timeRect = {780, 300, 160,120 }; // Vị trí ở góc trên bên trái

            // Vẽ texture điểm số
            SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);

            // Giải phóng texture sau khi sử dụng
            SDL_DestroyTexture(timeTexture);
        }
        }

        SDL_RenderPresent(renderer);
    }

    void update() {
        if (!inMenu && !gameOver) {
            player.updateBullets();
            updateEnemyTanks();
            //if(score==18) running=false;
            //if(time==120000) running=false;
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
                        // Tạo hiệu ứng nổ tại vị trí va chạm
                        explosions.push_back(Explosion(enemy.x, enemy.y));
                        Mix_PlayChannel(-1, explosionSound, 0);

                        enemy.active = false;
                        bullet.active = false;
                        score++;
                    }
                }
            }

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                         [](EnemyTank &e) { return !e.active; }), enemies.end());

            if (enemies.empty()) {
                enemyNumber++;
                spawnEnemies();
                level++;
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
                        player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
                        enemyNumber = 3;
                        spawnEnemies();
                        score = 0;
                        timee = 0;
                        level = 1;
                        SaveGame(*this,"save.txt");
                        return;
                    }
                }
            }
             if(score==18){
                        Mix_PlayMusic( winSound,0);
                        gameOver = true;
                        gameWon = true;
                        return;
                    }
              if(timee>=120000){
                        gameOver = true;
                        gameWon = false;
                        Mix_PlayMusic( loseSound, 0);
                        walls.clear();
                        generateWalls();
                        player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
                        enemyNumber = 3;
                        spawnEnemies();
                        score = 0;
                        timee = 0;
                        level = 1;
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

    void run() {
        while (running) {
            Events();
            update();
            render();
            SDL_Delay(15);
            if(!inMenu&&!gameOver) timee+=15;
        }

    }

    ~Game() {
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
};

int main(int argc, char *argv[]) {
    Game game;
    player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    if (running) {
        game.run();
    }
    return 0;
}
