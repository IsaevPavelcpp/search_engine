
#include "gtest/gtest.h"

#include "../header/InvertedIndex.h"

void TestInvertedIndexFunctionality(
        const std::vector<std::string>& docs,
        const std::vector<std::string>& requests,
        const std::vector<std::vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (auto &request: requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
};
const std::vector<std::string> requests = {"london", "the"};
const std::vector<std::vector<Entry>> expected = {
        {
                {0, 1}
        },
        {
                {0, 1}, {1, 3}
        }
};

TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {

const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
};
const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
const std::vector<std::vector<Entry>> expected = {
        {
                {0, 4}, {1, 1}, {2, 5}
        }, {
                {0, 3}, {1, 2}, {2, 5}
        }, {
                {3, 1}
        }
};

TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
const std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement"
};
const std::vector<std::string> requests = {"m", "statement"};
const std::vector<std::vector<Entry>> expected = {
        {
        },
        {
                {1, 1}
        }
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestEmptyRequest) {
const std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement spell such",
        "It’s origins and the history of creation are still hotly debated in scholarly circles"
        "The events unfolding on a tapestry took place in the years 1064 to 1066"
};
const std::vector<std::string> requests = {""};
const std::vector<std::vector<Entry>> expected = {
        {}
};
TestInvertedIndexFunctionality(docs, requests, expected);
}