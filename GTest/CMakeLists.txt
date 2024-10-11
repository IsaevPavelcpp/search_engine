cmake_minimum_required(VERSION 3.29)


set(CMAKE_CXX_STANDARD 17)

set(SrcFiles
        "../src/InvertedIndex.cpp"
        "../src/SearchServer.cpp"
)

set(TestsFiles
        TestInvertedIndex.cpp
        TestSearchServer.cpp
)

add_executable(GtestTests ${TestsFiles} ${SrcFiles})
target_link_libraries(GtestTests nlohmann_json::nlohmann_json GTest::gtest_main)
include(GoogleTest)
target_include_directories(GtestTests PUBLIC ${MY_INCLUDE_DIR})
gtest_discover_tests(GtestTests)



