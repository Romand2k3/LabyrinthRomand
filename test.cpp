#include "doctest.h"
#include "calculate.h"

TEST_CASE("onmap") {
    int v = OnMap(50, 50, 200, 200);

    REQUIRE(v == true);
}

TEST_CASE("pyrahit") {
REQUIRE(PyraHit(5, 5, 10, 10) == false);
}