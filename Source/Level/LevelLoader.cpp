//
// Created by null on 9/29/2024.
//

#include "LevelLoader.h"

Level * LevelLoader::LoadFromFile(std::string file) {
    std::ifstream fileStream;
    fileStream.open(file);
    if (!fileStream.is_open()) {
        spdlog::critical("Could not open file {}", file);
        return nullptr;
    }

    fileStream.seekg (0, std::ifstream::end);
    int length = fileStream.tellg();
    fileStream.seekg (0, std::ifstream::beg);

    char* buffer = new char[length];
    fileStream.read(buffer, length);
    fileStream.close();

    Level* level = Level::Deserialize(buffer, length);
    return level;
}

int LevelLoader::SaveToFile(std::string file, const Level *level) {
    std::ofstream fileStream;
    fileStream.open(file);
    if (!fileStream.is_open()) {
        spdlog::critical("Could not open file {}", file);
        return -1;
    }

    size_t size = 0;
    char* data = level->Serialize(&size);

    fileStream.write(data, size);
    fileStream.close();
    spdlog::info("Saved {} walls to {}", level->walls.size(), file);

    return 0;
}
