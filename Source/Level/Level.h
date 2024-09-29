//
// Created by null on 9/29/2024.
//

#ifndef LEVEL_H
#define LEVEL_H
#include "Wall.h"

class Level {
public:
    std::vector<Wall*> walls;

    Level();
    ~Level();

    void Render(Shader& shader) const;
    static Level* CreateSampleLevel();
    char* Serialize(size_t *outSize) const;
    static Level* Deserialize(const char *data, size_t size);
};
#endif //LEVEL_H
