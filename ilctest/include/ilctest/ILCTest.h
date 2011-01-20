#include <iostream>
//#include <cstring>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

class ILCTest{

public:


    // contructor
    ILCTest( const std::string& testname, std::ostream& stream=std::cout ) :
        _testname(testname), _out(stream), _failed(0), _passed(0), _last_test_status(false)
    {
        _out << std::endl << "[" << _testname << "] ";
        _out << "****************************** TEST_BEGIN ******************************" << std::endl << std::endl;
    }



    // destructor
    ~ILCTest(){

        std::stringstream sstr ;

        sstr << std::endl;
        sstr << "[" << _testname << "] number of tests PASSED : " << _passed << std::endl ;
        sstr << "[" << _testname << "] number of tests FAILED : " << _failed << std::endl ;
        sstr << std::endl;

        sstr << "[" << _testname << "] " ;
        sstr << "****************************** " ;
        sstr << ( _failed == 0 ? "TEST_PASSED" : "TEST_FAILED" ) ;
        sstr << " ******************************" ;
        sstr << std::endl << std::endl ;

        _out << sstr.str() ;
    }



    // FIXME rather use streamlog library?
    void log( const std::string& msg ){
        _out << "[" << _testname << "] " << msg << std::endl;
    }



    // FIXME should this rather be called equals ? add another method not_equals ?
    template <class V1, class V2 >
        //void equals(const V1& v1, const V2& v2 , const std::string name ) {
        void operator()(const V1& v1, const V2& v2 , const std::string name ) {

            std::stringstream sstr ;

            if ( ! (v1 == v2)  ) {
                sstr << "  " << name<< " : [" << v1 << "] != [" << v2 << "]" ;
                error( sstr.str() ) ;
            } 
            else{
                sstr << " [" << v1 << "] == [" << v2 << "]" ;
                pass( sstr.str() ) ;
            }
        }

    //    void operator()( bool cond, const std::string msg ){
    //      if ( ! cond ) error( msg ) ;
    //    } 

    

    void pass( const std::string& msg ){

        _passed++;
        _last_test_status = true ;

        _out << "[" << _testname << "] last test status : " << last_test_status() << " : " << msg << std::endl;
    }



    void error( const std::string& msg ){

        _failed++;
        _last_test_status = false ;

        std::stringstream errmsg;
        errmsg << std::endl;
        errmsg << "[" << _testname << "] ##################### TEST_FAILED ######################" << std::endl;
        errmsg << "[" << _testname << "] ### ERROR: " << msg << std::endl;
        errmsg << "[" << _testname << "] ########################################################" << std::endl;
        errmsg << std::endl;

        _out << errmsg.str();

        // also send error to stderr
        //std::cerr << errmsg.str();
    }



    void fatal_error( const std::string& msg ){
        error( msg );
        _out << "FATAL ERROR OCCURRED, program will exit now !!" << std::endl ;
        exit(1);
    }



    // returns the status from the last test
    const char* last_test_status(){
        return ( _last_test_status ? "PASSED" : "FAILED" ) ;
    }



private:

    std::string _testname ;
    std::ostream& _out ;

    unsigned int _failed ;      // number of failed tests
    unsigned int _passed ;      // number of passed tests
    bool _last_test_status ;    // true if last test succeeded, false otherwise
};
