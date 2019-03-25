////////////////////////
// example streamlog_mt_file_example.cc
////////////////////////

#include <streamlog/streamlog.h>
#include <streamlog/logsink.h>

#include <future>

using namespace streamlog ;

int main(int argc, char** argv) {
  
  std::string fbasename = "streamlog_mt_file_example" ;
  if ( argc > 1 ) {
    fbasename = argv [1] ;
  }

  // thread file will write messages in different files for each single thread using the same logger
  auto logger = logstream::createLogger<mt>( "test_logger" , logstream::threadFile(fbasename, ".log") ) ;
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
