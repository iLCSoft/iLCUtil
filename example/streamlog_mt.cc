////////////////////////
// example for streamlog in multi-threading mode
////////////////////////

#include <exception>
#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

#include <streamlog/streamlog.h>

using namespace std ;
using namespace streamlog ;

void do_printout() {
  std::stringstream ss ; ss << "ExampleScope_" << std::this_thread::get_id() ;
  logscope scope( streamlog::out, ss.str(), DEBUG::name() ) ;
  streamlog_out(DEBUG) << "DEBUG from thread id " << std::this_thread::get_id() << std::endl ;
  streamlog_out(MESSAGE) << "MESSAGE from thread id " << std::this_thread::get_id() << std::endl ;
  streamlog_out(WARNING) << "WARNING from thread id " << std::this_thread::get_id() << std::endl ;
  streamlog_out(ERROR) << "ERROR from thread id " << std::this_thread::get_id() << std::endl ; 
  scope.release() ;
  
  ss.str(""); ss << "ExampleScope2_" << std::this_thread::get_id() ;
  logscope scope2( streamlog::out, ss.str(), DEBUG::name() ) ;
  streamlog_out(DEBUG) << "DEBUG from thread id " << std::this_thread::get_id() << std::endl ;
  streamlog_out(MESSAGE) << "MESSAGE from thread id " << std::this_thread::get_id() << std::endl ;
  streamlog_out(WARNING) << "WARNING from thread id " << std::this_thread::get_id() << std::endl ;
  streamlog_out(ERROR) << "ERROR from thread id " << std::this_thread::get_id() << std::endl ; 
}

int main(int , char** ) {

  logstream::global_init( &std::cout, "ExampleMT", MESSAGE::name() ) ;
  
  streamlog_out(DEBUG) << "From main thread (before threads)" << std::endl ;
  streamlog_out(MESSAGE) << "From main thread (before threads)" << std::endl ;
  streamlog_out(WARNING) << "From main thread (before threads)" << std::endl ;
  streamlog_out(ERROR) << "From main thread (before threads)" << std::endl ;
  
  std::vector<std::thread> threads {10} ;
  
  for( int i=0 ; i<10 ; i++ ) {
    threads[i] = std::thread( do_printout ) ;
  }
  
  for( int i=0 ; i<10 ; i++ ) {
    threads[i].join() ;
  }
  
  threads.clear() ;
  
  streamlog_out(DEBUG) << "From main thread (after threads)" << std::endl ;
  streamlog_out(MESSAGE) << "From main thread (after threads)" << std::endl ;
  streamlog_out(WARNING) << "From main thread (after threads)" << std::endl ;
  streamlog_out(ERROR) << "From main thread (after threads)" << std::endl ;

  return 0;
}

