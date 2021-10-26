#include <gtest/gtest.h>

namespace
{
    class AmpPhaseTests : public ::testing::Test
    {
      public:
        AmpPhaseTests() = default;
    };

    TEST_F(AmpPhaseTests, Test1)
    {
        ASSERT_EQ(true, false);
    }
} // namespace