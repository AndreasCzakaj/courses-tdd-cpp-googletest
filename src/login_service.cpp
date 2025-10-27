#include "login_service.h"
#include "repository.h"
#include "uss.h"
#include <algorithm>
#include <regex>
#include <cctype>


Session LoginService::login(const Credentials& credentials) {
    Credentials sanitizedCredentials = sanitizeAndValidateCredentials(credentials);
    auto maybePerson = getPerson(sanitizedCredentials.email);
    if (!maybePerson.has_value()) {
        throw LoginException("Unknown email");
    }

    const Person& person = *maybePerson;
    if (person.passwordHash != sanitizedCredentials.plainPassword) {
        throw LoginException("Wrong password");
    }

    return {person.id};
}

std::optional<Person> LoginService::getPerson(const std::string& email) {
    try {
        return personRepo->get(email);
    } catch (const RepositoryException& e) {
        throw ServerException("DB error");
    }
}
