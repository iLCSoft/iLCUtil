
#include <streamlog/streamlog.h>

using namespace streamlog ;

int main(int argc, char **argv) {
  
  logstream::global_init( &std::cout, argv[0], MESSAGE::name() ) ;
  
  // Standard printing
  streamlog_out(DEBUG) << "This is not printed" << std::endl ;
  streamlog_out(MESSAGE) << "This is a message" << std::endl ;
  streamlog_out(WARNING) << "This is a warning" << std::endl ;
  streamlog_out(ERROR) << "This is an error" << std::endl ;

  // logscope test
  {
    streamlog::logscope scope( streamlog::out ) ;
    scope.setName( "ScopedLogging" ) ;
    scope.setLevel< streamlog::MESSAGE3 >() ;
    
    // Standard printing
    streamlog_out(DEBUG) << "This is not printed" << std::endl ;
    streamlog_out(MESSAGE) << "This is not printed" << std::endl ;
    streamlog_out(WARNING) << "This is a warning" << std::endl ;
    streamlog_out(ERROR) << "This is an error" << std::endl ;
  }
  
  // Standard printing after logscope
  streamlog_out(DEBUG) << "This is not printed" << std::endl ;
  streamlog_out(MESSAGE) << "This is a message" << std::endl ;
  streamlog_out(WARNING) << "This is a warning" << std::endl ;
  streamlog_out(ERROR) << "This is an error" << std::endl ;
  
  return 0 ;
}