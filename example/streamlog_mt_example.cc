////////////////////////
// example streamlog_mt_example.cc
////////////////////////

#include <streamlog/streamlog.h>
#include <streamlog/logsink.h>

#include <future>

using namespace streamlog ;

int main(int, char** argv) {

  // try the colored console (multi threaded mode)
  auto logger = logstream::createLogger<mt>( "test_logger" , logstream::coloredConsole<mt>() ) ;
  logger->setLevel<DEBUG>() ;

  std::vector<std::future<void>> futures {} ;
  
  // spawn 5 threads writing in the same logger
  for( auto i=0 ; i<5 ; i++ ) {
    futures.push_back( std::async( std::launch::async, [&]() {
      auto tid = std::this_thread::get_id() ;
      for ( auto j=0 ; j<1000 ; j++ ) {
        logger->log<MESSAGE>() << "Writing iteration " << j << " from thread no " << tid << std::endl ;
      }
    })) ;
  }
  // join tasks
  for ( auto &f : futures ) {
    f.get() ;
  }

  return 0;
}
