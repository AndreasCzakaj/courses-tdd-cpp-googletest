#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include "repository.h"
#include "uss.h"

class LoginService {
private:
    //PersonRepository* personRepo;
    //std::optional<Person> getPerson(const std::string& email);

public:
    //explicit LoginService(PersonRepository* repository) : personRepo(repository) {}

    Session login(const Credentials& credentials);
};
