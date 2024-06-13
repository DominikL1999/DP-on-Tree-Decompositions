#include "util.h"

int test_filter(int argc, char** argv) {
    std::vector<int>ints{10,-20,5,20,100,2,8,11,-50};
    std::vector<int>filteredInts = filter<int>(ints, [](int i){return i < 50;});
    bool success = setEqual({10,-20,5,20,2,8,11,-50}, filteredInts);

    return !success;
}