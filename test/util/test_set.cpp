#include "util.h"

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

bool test_set_equal() {
    bool success = true;
    success &= test_set_equal_1();
    success &= test_set_equal_2();
    success &= test_set_equal_3();
    
    return success;
}

int test_set(int argc, char** argv) {
    std::unordered_set<int>ints1{5,1,9,3,7};
    std::unordered_set<int>ints2{7,5,9,-2};
    bool success = true;

    success &= returnAndOutputOnFailure({1,3}, (setDifferrence(ints1, ints2)));
    success &= returnAndOutputOnFailure({5,1,9,3,7,-2}, (setUnion(ints1, ints2)));
    success &= returnAndOutputOnFailure({5,7,9}, (setIntersect(ints1, ints2)));
    success &= test_set_equal();

    return !success;
}