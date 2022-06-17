#include "doctest.h"
#include "functions.cpp"

TEST_CASE("test") {
    int v = OnMap(50, 50);

    REQUIRE(v == TRUE);
}

