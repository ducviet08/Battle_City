#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <string>

class Game; // Forward declaration

bool SaveGame(const Game& game, const std::string& filename);
bool LoadGame(Game& game, const std::string& filename);

#endif
