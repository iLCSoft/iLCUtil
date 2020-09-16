
#include <streamlog/streamlog.h>

#include <thread>
#include <vector>

using namespace streamlog ;

void print() {
  // Standard printing
  streamlog_out(DEBUG) << "This is a debug" << std::endl ;
  streamlog_out(MESSAGE) << "This is a message" << std::endl ;
  streamlog_out(WARNING) << "This is a warning" << std::endl ;
  streamlog_out(ERROR) << "This is an error" << std::endl ;
}

void print_thread( int n=0 ) {
  for(int i=0 ; i<n ; ++i) {
    streamlog_out(DEBUG)   << "Thread: " << std::this_thread::get_id() << ", i:" << i << ")" << std::endl ;
    streamlog_out(MESSAGE) << "Thread: " << std::this_thread::get_id() << ", i:" << i << ")" << std::endl ;
    streamlog_out(WARNING) << "Thread: " << std::this_thread::get_id() << ", i:" << i << ")" << std::endl ;
    streamlog_out(ERROR)   << "Thread: " << std::this_thread::get_id() << ", i:" << i << ")" << std::endl ;
  }
}

int main(int argc, char **argv) {
  
  logstream::global_init( &std::cout, "global", MESSAGE::name() ) ;
  const unsigned int nthr = argc > 1 ? atoi( argv[1] ) : std::thread::hardware_concurrency() ;
  
  print() ;

  std::vector<std::thread> threads {nthr} ;
  
  for( auto &thread : threads ) {
    thread = std::thread( [](){
      // Standard printing test
      print_thread( 10 ) ;
      // Change logger scope inside the thread
      // This affects the thread local global logger streamlog::out
      {
        streamlog::logscope scope( streamlog::out ) ;
        std::stringstream ss ; ss << "scope_" << std::this_thread::get_id() ;
        scope.setName( ss.str() ) ;
        scope.setLevel< streamlog::MESSAGE3 >() ;
        print_thread( 10 ) ;
      }
      // standard printing test again after logscope
      print_thread( 10 ) ;
    } ) ;
  }
  
  for ( auto &thread : threads ) {
    thread.join() ;
  }
  
  return 0 ;
}