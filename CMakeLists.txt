cmake_minimum_required(VERSION 3.28)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(googleTest OFF CACHE INTERNAL "")

add_executable(search_engine
        "src/main.cpp"
        "src/ConverterJSON.cpp"
        "src/InvertedIndex.cpp"
        "src/SearchServer.cpp"
        "header/ConverterJSON.h"
        "header/InvertedIndex.h"
        "header/SearchServer.h"

)
add_subdirectory(nlohmann_json)
target_link_libraries(search_engine PRIVATE  nlohmann_json::nlohmann_json)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)

add_subdirectory(GTest)

