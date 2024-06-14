#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

std::vector<std::string> stringSplit(const std::string& str, char delim);

bool endsWith(const std::string& str, const std::string& end);

std::string stripToFilename(const std::string& path);

template<typename T>
bool contains(const std::vector<T>& vec, const T& elem) {
    return std::find(vec.begin(), vec.end(), elem) != vec.end();
}

template<typename T>
bool setEqual(const std::vector<T>& vec1, const std::vector<T>& vec2) {
    return std::all_of(vec1.begin(), vec1.end(), [&vec2](const T& x){return contains(vec2, x);}) &&
        std::all_of(vec2.begin(), vec2.end(), [&vec1](const T& x){return contains(vec1, x);});
}

template<typename T1, typename T2>
std::vector<T2> map(const std::vector<T1>& vec1, std::function<T2(T1)>f) {
    std::vector<T2> vec2;
    vec2.resize(vec1.size());
    std::transform(vec1.begin(), vec1.end(), vec2.begin(), f);
    return vec2;
}

template<typename T>
std::vector<T> filter(const std::vector<T>& vec, std::function<bool(T)>f) {
    std::vector<T> vec2;
    for (const T& elem : vec) {
        if (f(elem))
            vec2.push_back(elem);
    }

    return vec2;
}

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