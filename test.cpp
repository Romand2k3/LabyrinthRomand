#include "doctest.h"
#include "calculate.h"

TEST_CASE("onmap") {
    REQUIRE(OnMap(50, 50, 200, 200) == true);
    REQUIRE(OnMap(250, 250, 200, 200) == false);
}

TEST_CASE("pyrahit") {
REQUIRE(PyraHit(5, 5, 10, 10) == false);
REQUIRE(PyraHit(5, 5, 5, 5) == true);
REQUIRE(PyraHit(5, 5, 6, 4) == true);
}