#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

std::vector<std::string> stringSplit(const std::string& str, char delim);

//// Unit testing utilities ////

template<typename T>
bool returnAndOutputOnFailure(const T& expected, const T& got) {
    bool success = expected == got;
    if (!success)
        std::cout << "Expected: " << expected << ". Got: " << got << std::endl;
    return success;
}

//// Pretty printing ////

// Try it out: If you have a local object `p` of type std::pair<T1,T2>, call:
// `std::cout << p << << std::endl;`
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1,T2>& pair) {
    return stream << "(" << pair.first << "," << pair.second << ")";
}

// Try it out: If you have a local object `v` of type std::vector<T>, call:
// `std::cout << v << << std::endl;`
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec) {
    stream << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        stream << vec[i];
        if (i < vec.size() - 1)
            stream << ",";
    }
    stream << "]";
    return stream;
}

// Try it out: If you have a local object `m` of type std::unordered_map<T1,T2, call:
// `std::cout << m << << std::endl;`
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::unordered_map<T1,T2>& map) {
    int count = 0;
    for (auto it = map.begin(); it != map.end(); it++) {
        const std::pair<T1,T2>& pair = *it;
        stream << pair.first << " -> " << pair.second;
        if (count < map.size() - 1)
            stream << " | ";
        count++;
    }
    return stream;
}