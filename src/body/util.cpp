#include "util.h"

#include <sstream>
#include <iostream>

using namespace std;

double EPSILON = 1e-6;

bool doubleEqual(const double& d1, const double& d2) {
    return std::abs(d1 - d2) < EPSILON;
}

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

bool endsWith(const std::string& str, const std::string& end) {
    if (end.size() > str.size())
        return false;
    return str.substr(str.size() - end.size()) == end;
}

std::string stripToFilename(const std::string& path) {
    size_t slash_pos = path.find_last_of('/');
    size_t backslash_pos = path.find_last_of('\\');
    if (slash_pos == SIZE_MAX && backslash_pos == SIZE_MAX)
        return path;
    else if (slash_pos == SIZE_MAX)
        return path.substr(backslash_pos + 1);
    else if (backslash_pos == SIZE_MAX)
        return path.substr(slash_pos + 1);
    else
        return path.substr(std::max(slash_pos, backslash_pos));
}
