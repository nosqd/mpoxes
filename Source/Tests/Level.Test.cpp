#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"
#include "../Level/Level.h"
#include "Testing.h"

TEST_CASE("Test functionality", "[Level]") {
  Level* expectedLevel = Level::CreateSampleLevel();

  SECTION("Test serializing and deserializing") {
    size_t dataSize = 0;
    auto* data = expectedLevel->Serialize(&dataSize);

    Level* level = Level::Deserialize(data, dataSize);

    compareLevels(expectedLevel, level);
  }
}