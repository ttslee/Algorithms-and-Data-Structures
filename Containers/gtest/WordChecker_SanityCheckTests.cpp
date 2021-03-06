#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "ListSet.hpp"
#include "WordChecker.hpp"


TEST(WordChecker_SanityCheckTests, canCreateWithSetOfStrings)
{
    WordChecker w{ListSet<std::string>{}};
}


TEST(WordChecker_SanityCheckTests, wordsExistWhenInSet)
{
    ListSet<std::string> set;
    set.add("HELLO");
    set.add("THERE");
    set.add("BOO");

    WordChecker checker{set};

    EXPECT_TRUE(checker.wordExists("HELLO"));
    EXPECT_TRUE(checker.wordExists("THERE"));
    EXPECT_TRUE(checker.wordExists("BOO"));
}


TEST(WordChecker_SanityCheckTests, wordsDoNotExistWhenNotInSet)
{
    ListSet<std::string> set;
    set.add("HELLO");
    set.add("THERE");
    set.add("BOO");

    WordChecker checker{set};

    EXPECT_FALSE(checker.wordExists("NOTBOO"));
    EXPECT_FALSE(checker.wordExists("HELLOTHERE"));
}


TEST(WordChecker_SanityCheckTests, canGenerateSuggestions)
{
    ListSet<std::string> set;
    set.add("ABDC");
    set.add("ZZZZZ");

    WordChecker checker{set};

    std::vector<std::string> suggestions = checker.findSuggestions("ABCD");

    ASSERT_EQ(1, suggestions.size());
    ASSERT_EQ("ABDC", suggestions[0]);
}

