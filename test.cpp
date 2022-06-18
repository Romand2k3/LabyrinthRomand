#include "doctest.h"
#include "calculate.h"

TEST_CASE("test") {
    int v = OnMap(50, 50, 200, 200);

    REQUIRE(v == true);
}

