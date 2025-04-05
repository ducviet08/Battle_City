#include "SaveLoad.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Wall.h"
using namespace std;
// Lưu game sang file save.txt (moved outside Game class)
bool SaveGame(const Game& game, const std::string& filename) {
    extern int maxScore;
    extern int score;
    extern double timee;
    extern int level;
    extern int enemyNumber;
    extern vector<class Wall> walls;
    extern vector<class EnemyTank> enemies;
    extern PlayerTank player;

    std::ofstream file(filename); // Open file in text mode
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return false;
    }

    // Save general info
    file << "MaxScore: "<< maxScore<< std::endl;
    file << "Score: " << score << std::endl;
    file << "Time: " << timee << std::endl;
    file << "Level: " << level << std::endl;
    file << "EnemyNumber: " << enemyNumber << std::endl;

    // Save player position
    file << "PlayerX: " << player.x << std::endl;
    file << "PlayerY: " << player.y << std::endl;

    // Save wall states
    file << "Walls: " << walls.size() << std::endl;
    for (const auto& wall : walls) {
        file << wall.active << " ";
    }
    file << std::endl;

    // Save enemy states and positions
    file << "Enemies: " << enemies.size() << std::endl;
    for (const auto& enemy : enemies) {
        file << enemy.active << " " << enemy.x << " " << enemy.y <<" "<<enemy.health <<std::endl;
    }

    file.close();
    std::cout << "Game saved to: " << filename << std::endl;
    return true;
}

// Load game từ file save.txt (moved outside Game class)
bool LoadGame(Game& game, const std::string& filename) {
     extern int maxScore;
    extern int score;
    extern double timee;
    extern int level;
    extern int enemyNumber;
    extern vector<class Wall> walls;
    extern vector<class EnemyTank> enemies;
    extern PlayerTank player;
    std::ifstream file(filename); // Open file in text mode
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::stringstream ss;

    // Load general info
    getline(file, line); ss.str(line.substr(line.find(":") + 2)); ss >> maxScore; ss.clear();
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
        int x, y,z;
        getline(file, line);
        ss.str(line);
        ss >> active >> x >> y>>z;
        ss.clear();

        if (active) {
            EnemyTank enemy(x, y,z);
            enemy.active = true;
            enemies.push_back(enemy);
        }
    }

    file.close();
    std::cout << "Game loaded from: " << filename << std::endl;
    return true;
}
