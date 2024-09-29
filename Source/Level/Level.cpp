//
// Created by null on 9/29/2024.
//

#include "Level.h"

Level::Level() {
    this->walls = std::vector<Wall*>();
}

Level::~Level() {
    for (auto it = walls.begin(); it != walls.end(); ++it) {
        delete (*it);
    }
}

void Level::Render(Shader &shader) const {
    for (auto wall: walls) {
        wall->Render(shader);
    }
}

Level *Level::CreateSampleLevel() {
    auto *level = new Level();

    Wall *walls[] = {
        new Wall{1, Vector2(0.f, 8.f), Vector2(64.f, 16.f), Color(255, 0, 255, 255)},
        new Wall{2, Vector2(0.f, -9.f), Vector2(64.f, 16.f), Color(255, 255, 255, 255)},
    };

    for (auto w: walls) {
        level->walls.push_back(w);
    }

    return level;
}

char* Level::Serialize(size_t* outSize) const {
    const char* magic = "mlvl";
    const size_t headerSize = 4 + sizeof(size_t);
    const size_t wallSize = sizeof(int) + sizeof(Vector2) + sizeof(Vector2) + sizeof(Color);
    const size_t wallsSize = wallSize * walls.size();
    const size_t totalSize = headerSize + wallsSize;

    char* buffer = new char[totalSize];
    char* currentPos = buffer;

    // Write magic number
    memcpy(currentPos, magic, 4);
    currentPos += 4;

    // Write wall count
    size_t wallCount = walls.size();
    memcpy(currentPos, &wallCount, sizeof(size_t));
    currentPos += sizeof(size_t);

    // Write walls data
    for (const auto& wall : walls) {
        memcpy(currentPos, &wall->id, sizeof(int));
        currentPos += sizeof(int);

        memcpy(currentPos, &wall->position, sizeof(Vector2));
        currentPos += sizeof(Vector2);

        memcpy(currentPos, &wall->size, sizeof(Vector2));
        currentPos += sizeof(Vector2);

        memcpy(currentPos, &wall->color, sizeof(Color));
        currentPos += sizeof(Color);
    }

    *outSize = totalSize;
    return buffer;
}

Level* Level::Deserialize(const char *data, size_t size) {
    const char* expectedMagic = "mlvl";
    const size_t headerSize = 4 + sizeof(size_t);
    const size_t wallSize = sizeof(int) + sizeof(Vector2) + sizeof(Vector2) + sizeof(Color);

    if (size < headerSize) {
        return nullptr;
    }

    if (memcmp(data, expectedMagic, 4) != 0) {
        return nullptr;
    }

    size_t wallCount;
    memcpy(&wallCount, data + 4, sizeof(size_t));

    size_t expectedSize = headerSize + wallCount * wallSize;
    if (size < expectedSize) {
        return nullptr;
    }

    auto* level = new Level();

    const char* currentPos = data + headerSize;
    for (size_t i = 0; i < wallCount; ++i) {
        int id;
        Vector2 position, size;
        Color color;

        memcpy(&id, currentPos, sizeof(int));
        currentPos += sizeof(int);

        memcpy(&position, currentPos, sizeof(Vector2));
        currentPos += sizeof(Vector2);

        memcpy(&size, currentPos, sizeof(Vector2));
        currentPos += sizeof(Vector2);

        memcpy(&color, currentPos, sizeof(Color));
        currentPos += sizeof(Color);

        level->walls.emplace_back(new Wall(id, position, size, color));
    }

    size = expectedSize;

    return level;
}