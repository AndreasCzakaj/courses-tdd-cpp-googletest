#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include "repository.h"

struct ValidationError {
    std::string field;
    std::string message;
};

class ValidationException : public std::exception {
private:
    std::vector<ValidationError> errors;
    mutable std::string message;

public:
    explicit ValidationException(const std::vector<ValidationError>& errors) : errors(errors) {}

    const std::vector<ValidationError>& getErrors() const {
        return errors;
    }

    const char* what() const noexcept override {
        if (message.empty()) {
            for (size_t i = 0; i < errors.size(); ++i) {
                if (i > 0) message += "; ";
                message += errors[i].field + ": " + errors[i].message;
            }
        }
        return message.c_str();
    }
};

class ServerException : public std::exception {
private:
    std::string message;

public:
    explicit ServerException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class LoginException : public std::exception {
private:
    std::string message;

public:
    explicit LoginException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

std::string sanitizeAndValidateEmail(const std::string& email);
std::string sanitizeAndValidatePassword(const std::string& password);

// Forward declaration
struct Credentials;

Credentials sanitizeAndValidateCredentials(const Credentials& credentials);

struct Person {
    std::string id;
    std::string email;
    std::string passwordHash;
    std::string status;
};

struct Credentials {
    std::string email;
    std::string plainPassword;
};

struct Session {
    std::string userId;
};

// Type alias for Person repository
using PersonRepository = IRepository<Person>;
