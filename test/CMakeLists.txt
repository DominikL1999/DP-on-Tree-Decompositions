CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

# List all subdirectories containing tests here
set(test_directories
    util)

foreach (dir ${test_directories})
    add_subdirectory(${dir})
endforeach()
