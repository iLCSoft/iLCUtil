////////////////////////
// ilctest hello_world
////////////////////////

#include "ilctest/ILCTest.h"
#include <exception>
#include <iostream>
#include <assert.h>

using namespace std ;

// this should be the first line in your test
static ILCTest ilctest = ILCTest( "hello_world" , std::cout );

//=============================================================================

int main(int argc, char** argv ){
    
    try{
    
        // ----- write your tests in here -------------------------------------

        ilctest.log( "hello world test" );

        ilctest( "Hello", "Hello", "hello test" ); // this test will pass: Hello == Hello

        //ilctest( "World", "world", "world test" ); // this test will fail: World != world

        // --------------------------------------------------------------------


    } catch( exception &e ){
    //} catch( ... ){
        ilctest.log( e.what() );
        ilctest.error( "exception occurred" );
    }


    return 0;
}

//=============================================================================

