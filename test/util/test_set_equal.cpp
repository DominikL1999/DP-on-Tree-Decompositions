#include "util.h"

#include <vector>

int test_set_equal(int argc, char** argv) {
    std::vector<int> ints1{1,2,3,4,5,6};
    std::vector<int> ints2{2,4,6,1,3,5};

    bool success = setEqual(ints1, ints2);
    
    return !success;
}