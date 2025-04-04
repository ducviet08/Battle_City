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
#include "Game.h"
#include "PlayerTank.h"

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
int maxScore;
vector<class EnemyTank> enemies;
vector<class Explosion> explosions;
int score = 0;
int timee = 0; // Avoid name conflict with std::time
int level = 1;
bool gameStarted = false;
 bool game2player =false;
bool inMenu = true;
SDL_Texture* startButtonTexture = nullptr;
SDL_Texture* player2ButtonTexture = nullptr;
SDL_Texture* continueButtonTexture = nullptr;
SDL_Texture* pauseTexture = nullptr;
bool showInstructions = false;
bool gameOver = false;
bool gameWon = false;
bool gamePlayer1=false;
SDL_Texture* menuText=nullptr;
SDL_Texture* backgroundText=nullptr;
SDL_Texture* boardText=nullptr;
SDL_Texture* win1Texture = nullptr;
SDL_Texture* win2Texture = nullptr;
SDL_Texture* winTexture = nullptr;
SDL_Texture* loseTexture = nullptr;
SDL_Texture* playAgainText = nullptr;
SDL_Texture *playerTexture = nullptr;
SDL_Texture *player2Texture = nullptr;
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
SDL_Texture* health1Texture=nullptr;
SDL_Texture* health2Texture=nullptr;
SDL_Texture* health3Texture=nullptr;

vector<int> wall1x = {12, 0, 4, 4, 8, 8, 8, 8, 8, 12, 12, 16, 20, 20, 20, 20, 24, 24, 24, 24, 28, 32, 32, 32, 32, 36, 36, 36, 36, 36, 40, 40, 44, 52, 52, 52, 52, 52, 52, 52, 52, 56, 56, 58, 60, 60, 62, 64, 64, 66, 68, 68, 72, 72, 72, 72, 72, 72, 72, 72};
vector<int> wall1y = {16, 12, 8, 16, 12, 20, 28, 32, 40, 28, 40, 40, 40, 36, 32, 28, 12, 16, 20, 24, 12, 12, 24, 36, 40, 12, 24, 36, 16, 20, 44, 48, 44, 4, 8, 12, 16, 20, 28, 32, 36, 38, 4, 8, 12, 38, 16, 12, 38, 8, 4, 38, 4, 8, 12, 16, 20, 28, 32, 36};
vector<int> wall2x = {0, 4, 8, 12, 16, 16, 28, 28, 32, 44};
vector<int> wall2y = {48, 12, 16, 36, 20, 32, 20, 32, 16, 4};
vector<int> wall3x = {0, 0, 0, 4, 4, 8, 8, 12, 12, 16, 16, 20, 24, 28, 28, 32, 32, 36, 36, 36, 40, 40, 40, 44, 44, 44, 44, 44, 48, 48, 56, 68, 58, 62, 66};
vector<int> wall3y = {44, 40, 36, 36, 48, 36, 48, 44, 48, 24, 28, 20, 32, 24, 28, 4, 8, 4, 40, 44, 40, 4, 16, 8, 12, 16, 48, 52, 48, 52, 24, 24, 34, 34, 34};

PlayerTank player=PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
PlayerTank player2=PlayerTank(0,0);

int main(int argc, char *argv[]) {
    Game game;
    player = PlayerTank((MAP_WIDTH - 1) / 2 * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
    if (running) {
        game.run();
    }
    return 0;
}
