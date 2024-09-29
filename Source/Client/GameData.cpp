#include "GameData.h"
#include <spdlog/spdlog.h>

void GameData::Save() {
    nlohmann::json data = {
        {"connect_modal", {
            {"host", connect_modal_host},
            {"port", connect_modal_port}
        }}
    };

    std::ofstream o("mpoxes.json");
    o << std::setw(4) << data << std::endl;

    spdlog::info("Saved data");
}

void GameData::Load() {
    std::ifstream o("mpoxes.json");

    connect_modal_host.reserve(256);
    connect_modal_port.reserve(7);
    if (o.is_open()) {
        nlohmann::json data;
        o >> data;

        connect_modal_host = data["connect_modal"]["host"];
        connect_modal_port = data["connect_modal"]["port"];

        spdlog::info("Loaded data from mpoxes.json");
    }
    else {
        spdlog::warn("Failed to load data from mpoxes.json, creating new");

        connect_modal_host = "127.0.0.1";
        connect_modal_port = "7777";

        Save();
    }
}

