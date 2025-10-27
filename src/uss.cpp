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

    // Validate minimum length
    if (sanitized.length() < 6) {
        throw std::invalid_argument("Email must be at least 6 characters");
    }

    // Check for @ symbol
    if (sanitized.find('@') == std::string::npos) {
        throw std::invalid_argument("Email must contain @");
    }

    // Check for . symbol
    if (sanitized.find('.') == std::string::npos) {
        throw std::invalid_argument("Email must contain .");
    }

    // Validate regex pattern
    std::regex pattern("[a-z0-9.@\\-]+");
    if (!std::regex_match(sanitized, pattern)) {
        throw std::invalid_argument("Email contains invalid characters");
    }

    return sanitized;
}

std::string sanitizeAndValidatePassword(const std::string& password) {
    // Trim whitespace
    std::string trimmed = password;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);

    // Validate length
    if (trimmed.length() < 12) {
        throw std::invalid_argument("Password must be at least 12 characters");
    }
    if (trimmed.length() > 24) {
        throw std::invalid_argument("Password must be at most 24 characters");
    }

    // Check for required character sets
    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    std::string specialChars = ".-_+,;:";

    for (char c : trimmed) {
        if (c >= 'a' && c <= 'z') {
            hasLower = true;
        } else if (c >= 'A' && c <= 'Z') {
            hasUpper = true;
        } else if (c >= '0' && c <= '9') {
            hasDigit = true;
        } else if (specialChars.find(c) != std::string::npos) {
            hasSpecial = true;
        } else {
            // Invalid character found
            throw std::invalid_argument("Password contains invalid characters");
        }
    }

    if (!hasLower) {
        throw std::invalid_argument("Password must contain at least one lowercase letter");
    }
    if (!hasUpper) {
        throw std::invalid_argument("Password must contain at least one uppercase letter");
    }
    if (!hasDigit) {
        throw std::invalid_argument("Password must contain at least one digit");
    }
    if (!hasSpecial) {
        throw std::invalid_argument("Password must contain at least one special character");
    }

    return trimmed;
}

