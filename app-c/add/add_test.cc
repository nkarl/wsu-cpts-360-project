#include <gtest/gtest.h>
#include "add.h"

TEST(AddTest, BasicAssertions) {
    EXPECT_EQ(add(2, 4), 6);
}
