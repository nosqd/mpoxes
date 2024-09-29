//
// Created by null on 9/29/2024.
//

#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include <string>

#include "Level.h"
#include <fstream>
#include <iostream>

class LevelLoader {
public:
  static Level* LoadFromFile(std::string file);
  static int SaveToFile(std::string file, const Level* level);
};

#endif //LEVELLOADER_H
