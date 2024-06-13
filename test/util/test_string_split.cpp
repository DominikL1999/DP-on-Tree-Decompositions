#include "util.h"

#include <iostream>

bool test_string_split_1() {
    std::string str = "Hello, world, how are you doing, today?";
    std::vector<std::string> parts = stringSplit(str, ',');
    return returnAndOutputOnFailure(std::vector<std::string>{"Hello"," world"," how are you doing", " today?"}, parts);
}

bool test_string_split_2() {
    std::string str = "a,,20";
    std::vector<std::string> parts = stringSplit(str, ',');
    return returnAndOutputOnFailure(std::vector<std::string>{"a","","20"}, parts);
}

bool test_string_split_3() {
    std::string str = "N1,N2,";
    std::vector<std::string> parts = stringSplit(str, ',');
    return returnAndOutputOnFailure(std::vector<std::string>{"N1","N2"}, parts);
}

int test_string_split(int argc, char** argv) {
    return !(
        test_string_split_1() &&
        test_string_split_2() &&
        test_string_split_3);
}
