#include "util.h"

int test_ends_with(int argc, char** argv) {
    bool success = true;
    success &= endsWith("Hello, world!", "world!");
    success &= !endsWith("Hello,", "Hello, world!");
    success &= endsWith("Hello, world!", "Hello, world!");
    success &= endsWith("", "");
    success &= endsWith("Hello, world!", "");
    success &= !endsWith("Hello, world!", "worl");

    return !success;
}