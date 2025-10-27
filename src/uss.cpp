#include "uss.h"
#include <algorithm>
#include <regex>
#include <cctype>

Credentials sanitizeAndValidateCredentials(const Credentials& credentials) {
    std::vector<ValidationError> errors;
    std::string sanitizedEmail;
    std::string sanitizedPassword;

    // Try to validate email
    try {
        sanitizedEmail = sanitizeAndValidateEmail(credentials.email);
    } catch (const std::invalid_argument& e) {
        errors.push_back({"email", e.what()});
    }

    // Try to validate password
    try {
        sanitizedPassword = sanitizeAndValidatePassword(credentials.plainPassword);
    } catch (const std::invalid_argument& e) {
        errors.push_back({"plainPassword", e.what()});
    }

    // If there are any errors, throw ValidationException
    if (!errors.empty()) {
        throw ValidationException(errors);
    }

    // Return sanitized credentials
    return {sanitizedEmail, sanitizedPassword};
}

std::string sanitizeAndValidateEmail(const std::string& email) {
    // Trim whitespace
    std::string trimmed = email;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);

    // Convert to lowercase
    std::string sanitized = trimmed;
    std::transform(sanitized.begin(), sanitized.end(), sanitized.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // TODO: validation

    return sanitized;
}

std::string sanitizeAndValidatePassword(const std::string& password) {
    // TODO: sanitization
    // TODO: validation
    return password;
}

