#include "util.h"

#include <vector>

bool test_set_equal_1() {
    std::vector<int> ints1{1,2,3,4,5,6};
    std::vector<int> ints2{2,4,6,1,3,5};

    bool success = setEqual(ints1, ints2);
    
    return success;
}

bool test_set_equal_2() {
    std::vector<int> ints1{1,2,3,4,5,6};
    std::vector<int> ints2{2,4,6,1};

    bool success = !setEqual(ints1, ints2);
    
    return success;
}

bool test_set_equal_3() {
    std::vector<int> ints1{1,2,3,4};
    std::vector<int> ints2{2,4,6,1,3,5};

    bool success = !setEqual(ints1, ints2);
    
    return success;
}

int test_set_equal(int argc, char** argv) {
    bool success = true;
    success &= test_set_equal_1();
    success &= test_set_equal_2();
    success &= test_set_equal_3();
    
    return !success;
}