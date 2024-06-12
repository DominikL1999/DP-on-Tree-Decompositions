#include "util.h"

#include <sstream>
#include <iostream>

using namespace std;
std::vector<std::string> stringSplit(const std::string &str, char delim)
{
    std::vector<std::string> parts;
    string token;
    stringstream ss{str};

    while (getline(ss, token, delim)) {
        parts.push_back(token);
    }

    return parts;
}