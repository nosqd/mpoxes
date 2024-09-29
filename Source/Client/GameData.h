#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

namespace GameData {
    inline std::string connect_modal_host;
    inline std::string connect_modal_port;

    void Save();
    void Load();
};

#endif //GAMEDATA_H
