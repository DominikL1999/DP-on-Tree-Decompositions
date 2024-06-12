#include "util.h"

// Test 1. Return true on success
bool test_string_split_1() {
    std::string str = "Hello, world, how are you doing, today?";
    std::vector<std::string> parts = stringSplit(str, ',');
    return parts.size() == 4 &&
        parts[0] == "Hello" &&
        parts[1] == " world" &&
        parts[2] == " how are you doing" &&
        parts[3] == " today?";
}

int test_string_split(int argc, char** argv) {
    return !test_string_split_1();
}