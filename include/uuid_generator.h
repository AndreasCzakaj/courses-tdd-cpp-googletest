#pragma once

#include <string>

// ============================================================================
// UUID Generator Interface
// ============================================================================

class UuidGenerator {
public:
    virtual ~UuidGenerator() = default;
    virtual std::string create() = 0;
};

// ============================================================================
// Naive Random Implementation
// ============================================================================

class UuidGeneratorNaiveRandomImpl : public UuidGenerator {
public:
    std::string create() override;

protected:
    virtual std::string createOne();
};
