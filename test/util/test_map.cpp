#include "util.h"

#include <vector>

bool test_map_1() {
    std::vector<std::string> strs{"20", "30", "40"};
    std::vector<int> ints = map<std::string, int>(strs, [](const std::string& str){return std::stoi(str);});
    return returnAndOutputOnFailure(std::vector<int>{20, 30, 40}, ints);
}

int test_map(int argc, char** argv) {
    bool success = test_map_1();

    return !success;
}