#include "random_generator.hpp"

// std
#include <random>
#include <memory>

RandomGenerator::RandomGenerator() {
    std::random_device device;

    mGenerator = std::make_unique<std::mt19937>(device());
}

int RandomGenerator::GenInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);

    return dist(*mGenerator);
}

float RandomGenerator::GenFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);

    return dist(*mGenerator);
}