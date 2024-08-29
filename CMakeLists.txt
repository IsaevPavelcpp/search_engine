cmake_minimum_required(VERSION 3.28)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")


add_subdirectory(nlohmann_json)
enable_testing()
add_subdirectory(googletest)
add_executable(search_engine "source file/main.cpp"
        "source file/ConverterJSON.cpp"
        header/ConverterJSON.h
        header/ConverterJSON.h
        "source file/InvertedIndex.cpp"
        "header/InvertedIndex.h"
        "source file/SearchServer.cpp"
        header/SearchServer.h)

target_link_libraries(search_engine PRIVATE gtest_main gmock_main nlohmann_json::nlohmann_json)

