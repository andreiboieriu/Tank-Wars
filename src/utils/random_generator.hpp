#pragma once

// std
#include <random>
#include <memory>

class RandomGenerator {

public:
    RandomGenerator();

    int GenInt(int min, int max);

    float GenFloat(float min, float max);

private:
    std::unique_ptr<std::mt19937> mGenerator;
};

extern std::unique_ptr<RandomGenerator> gRandomGenerator;