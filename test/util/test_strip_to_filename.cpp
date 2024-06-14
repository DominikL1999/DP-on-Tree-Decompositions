#include <iostream>
#include <vector>

#include <util.h>

using std::cout;
using std::endl;

template<typename T>
void outputFailure(const T& input, const T& expected, const T& got) {
    cout << "Failure on input " << input << "." << endl << "Expected: " << expected << ". Got: " << got << endl;
}

// Same name as filename
int test_strip_to_filename(int argc, char** argv) {
    std::string path1 = "instance/instance1.json";
    std::string path2 = "instance\\instance1.json";
    std::string path3 = "instance1.json";
    std::string path4 = "instance1";

    if (stripToFilename(path1) != "instance1.json") {
        outputFailure<std::string>(path1, "instance1.json", stripToFilename(path1));
        return 1;
    }
    if (stripToFilename(path2) != "instance1.json") {
        outputFailure<std::string>(path2, "instance1.json", stripToFilename(path2));
        return 1;
    }
    if (stripToFilename(path3) != "instance1.json") {
        outputFailure<std::string>(path3, "instance1.json", stripToFilename(path3));
        return 1;
    }
    if (stripToFilename(path4) != "instance1") {
        outputFailure<std::string>(path4, "instance1", stripToFilename(path4));
        return 1;
    }
    
    return 0;
}