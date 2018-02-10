#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

// http://tech.kakao.com/2017/09/27/kakao-blind-recruitment-round-1/

int main( int argc, char* argv[] )
{
    int result = Catch::Session().run( argc, argv );
    return ( result < 0xff ? result : 0xff );
}