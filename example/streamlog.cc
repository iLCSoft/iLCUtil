////////////////////////
// example for streamlog
////////////////////////

#include <exception>
#include <iostream>

#include <streamlog/streamlog.h>

using namespace std ;
using namespace streamlog ;

int main(int , char** ) {

  logstream::global_init( &std::cout, "Example", MESSAGE::name() ) ;
  
  streamlog_out(DEBUG) << "Hello world!" << std::endl ;
  streamlog_out(MESSAGE) << "Hello world!" << std::endl ;
  streamlog_out(WARNING) << "Hello world!" << std::endl ;
  streamlog_out(ERROR) << "Hello world!" << std::endl ;
  
  {
    logscope scope( streamlog::out, "ExampleScope", WARNING::name() ) ;
    streamlog_out(DEBUG) << "Hello world (in scope)!" << std::endl ;
    streamlog_out(MESSAGE) << "Hello world (in scope)!" << std::endl ;
    streamlog_out(WARNING) << "Hello world (in scope)!" << std::endl ;
    streamlog_out(ERROR) << "Hello world (in scope)!" << std::endl ;
  }
  
  streamlog_out(DEBUG) << "Hello world!" << std::endl ;
  streamlog_out(MESSAGE) << "Hello world!" << std::endl ;
  streamlog_out(WARNING) << "Hello world!" << std::endl ;
  streamlog_out(ERROR) << "Hello world!" << std::endl ;

  return 0;
}

