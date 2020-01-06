#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <azgra/matrix.h>

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Matrix at(x,y)","[matrix<T>]" )
{
    azgra::Matrix<int> m(3,3,1);
    REQUIRE( m.at(0,0) == 1 );
}

TEST_CASE( "Factorials are computed", "[factorial]" ) 
{
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
