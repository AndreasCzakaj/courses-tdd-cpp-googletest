#include "uuid_generator.h"
#include <random>
#include <sstream>
#include <iomanip>

// Use a thread-local random engine for thread safety
static thread_local std::random_device rd;
static thread_local std::mt19937 generator(rd());
static thread_local std::uniform_int_distribution<int> distribution(0, 15);

std::string UuidGeneratorNaiveRandomImpl::create() {
    std::stringstream ss;
    for (int i = 0; i < 32; i++) {
        ss << createOne();
    }
    return ss.str();
}

std::string UuidGeneratorNaiveRandomImpl::createOne() {
    std::stringstream ss;
    ss << std::hex << distribution(generator);
    return ss.str();
}
