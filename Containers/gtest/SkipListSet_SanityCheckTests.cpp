#include <memory>
#include <string>
#include <gtest/gtest.h>
#include "SkipListSet.hpp"


TEST(SkipListSet_SanityCheckTests, inheritFromSet)
{
    SkipListSet<int> s1;
    Set<int>& ss1 = s1;
    int sz1 = ss1.size();
    sz1 = 0;

    SkipListSet<std::string> s2;
    Set<std::string>& ss2 = s2;
    int sz2 = ss2.size();
    sz2 = 0;
}


TEST(SkipListSet_SanityCheckTests, canCreateAndDestroy)
{
    SkipListSet<int> s1;
    SkipListSet<std::string> s2;
}


TEST(SkipListSet_SanityCheckTests, canCopyConstructToCompatibleType)
{
    SkipListSet<int> s1;
    SkipListSet<std::string> s2;
    //s1.add(1);

    SkipListSet<int> s1Copy{s1};
    SkipListSet<std::string> s2Copy{s2};
}


TEST(SkipListSet_SanityCheckTests, canMoveConstructToCompatibleType)
{
    SkipListSet<int> s1;
    SkipListSet<std::string> s2;

    SkipListSet<int> s1Copy{std::move(s1)};
    SkipListSet<std::string> s2Copy{std::move(s2)};
}


TEST(SkipListSet_SanityCheckTests, canAssignToCompatibleType)
{
    SkipListSet<int> s1;
    SkipListSet<std::string> s2;

    SkipListSet<int> s3;
    SkipListSet<std::string> s4;

    s1 = s3;
    s2 = s4;
}


TEST(SkipListSet_SanityCheckTests, canMoveAssignToCompatibleType)
{
    SkipListSet<int> s1;
    SkipListSet<std::string> s2;

    SkipListSet<int> s3;
    SkipListSet<std::string> s4;

    s1 = std::move(s3);
    s2 = std::move(s4);
}


TEST(SkipListSet_SanityCheckTests, isImplemented)
{
    SkipListSet<int> s1;
    EXPECT_TRUE(s1.isImplemented());

    SkipListSet<std::string> s2;
    EXPECT_TRUE(s2.isImplemented());
}


TEST(SkipListSet_SanityCheckTests, containsElementsAfterAdding)
{
    SkipListSet<int> s1;
    s1.add(11);
    s1.add(1);
    s1.add(5);
    s1.add(12);
    s1.add(13);
    s1.add(53);
    s1.add(17);
    s1.add(19);
    s1.add(54);
    s1.add(111);
    s1.add(132);
    s1.add(541);
    s1.add(121);
    s1.add(141);
    s1.add(551);
    EXPECT_TRUE(s1.contains(11));
    EXPECT_TRUE(s1.contains(1));
    EXPECT_TRUE(s1.contains(5));
    EXPECT_TRUE(s1.contains(12));
    EXPECT_TRUE(s1.contains(13));
    EXPECT_TRUE(s1.contains(53));
    EXPECT_TRUE(s1.contains(17));
    EXPECT_TRUE(s1.contains(19));
    EXPECT_TRUE(s1.contains(54));
    EXPECT_TRUE(s1.contains(111));
    EXPECT_TRUE(s1.contains(132));
    EXPECT_TRUE(s1.contains(541));
    EXPECT_TRUE(s1.contains(121));
    EXPECT_TRUE(s1.contains(141));
    EXPECT_TRUE(s1.contains(551));
}


TEST(SkipListSet_SanityCheckTests, doesNotContainElementsNotAdded)
{
    SkipListSet<int> s1;
    s1.add(11);
    s1.add(1);
    s1.add(5);

    EXPECT_FALSE(s1.contains(21));
    EXPECT_FALSE(s1.contains(2));
    EXPECT_FALSE(s1.contains(9));
}


TEST(SkipListSet_SanityCheckTests, sizeIsNumberOfElementsAdded)
{
    SkipListSet<int> s1;
    s1.add(11);
    s1.add(1);
    s1.add(5);

    EXPECT_EQ(3, s1.size());
}


namespace
{
    template <typename ElementType>
    class NeverGrowSkipListLevelTester : public SkipListLevelTester<ElementType>
    {
    public:
        virtual ~NeverGrowSkipListLevelTester() = default;

        virtual bool shouldOccupyNextLevel(const ElementType& element) override;
        virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;
    };


    template <typename ElementType>
    bool NeverGrowSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
    {
        return false;
    }


    template <typename ElementType>
    std::unique_ptr<SkipListLevelTester<ElementType>> NeverGrowSkipListLevelTester<ElementType>::clone()
    {
        return std::unique_ptr<SkipListLevelTester<ElementType>>{
            new NeverGrowSkipListLevelTester};
    }
}


TEST(SkipListSet_SanityCheckTests, alwaysOneLevelWhenWeNeverGrow)
{
    SkipListSet<int> s1{std::make_unique<NeverGrowSkipListLevelTester<int>>()};

    for (int i = 0; i < 100; ++i)
    {
        s1.add(i);
    }

    EXPECT_EQ(1, s1.levelCount());
}


TEST(SkipListSet_SanityCheckTests, allElementsOnLevel0WhenWeNeverGrow)
{
    SkipListSet<int> s1{std::make_unique<NeverGrowSkipListLevelTester<int>>()};

    for (int i = 0; i < 100; ++i)
    {
        s1.add(i);
    }

    ASSERT_EQ(100, s1.elementsOnLevel(0));
    ASSERT_EQ(0, s1.elementsOnLevel(1));

    for (int i = 0; i < 100; ++i)
    {
        EXPECT_TRUE(s1.isElementOnLevel(i, 0));
        EXPECT_FALSE(s1.isElementOnLevel(i, 1));
    }
}

