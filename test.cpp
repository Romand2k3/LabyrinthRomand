#include "doctest.h"
#include "calculate.h"

TEST_CASE("onmap") {
    int v = OnMap(50, 50, 200, 200);

    REQUIRE(v == true);
}

TEST_CASE("pyrahit") {
int v = PyraHit(5, 5, 10, 10);

REQUIRE(v == false);
}