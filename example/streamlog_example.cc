////////////////////////
// example streamlog_example.cc
////////////////////////

#include <streamlog/streamlog.h>
#include <streamlog/logsink.h>

using namespace streamlog ;

int main(int, char** argv) {

  // get and configure the global logger
  auto &main = streamlog::logstream::global() ;
  main.setName( "streamlog_test" ) ;
  main.setLevel<DEBUG>() ;
  
  streamlog_out(ERROR)   << "ERROR   from main logger\nbibou" ;
  
  streamlog_out(DEBUG)   << "DEBUG   from main logger" << std::endl ;
  streamlog_out(MESSAGE) << "MESSAGE from main logger" << "pouette" << std::endl ;
  streamlog_out(WARNING) << "WARNING from main logger" << std::endl ;
  streamlog_out(ERROR)   << "ERROR   from main logger" << std::endl ;
  
  // try the colored console (multi threaded mode)
  auto color_console = std::make_shared<streamlog::colored_console_sink_mt>(); 
  streamlog::logstream logger ( "test_logger" , color_console ) ;
  logger.setLevel<MESSAGE>() ;
  
  streamlog_logger_out(logger, MESSAGE) << "Is debug active ? ";
  streamlog_logger_out(logger, MESSAGE) << (STREAMLOG_DEBUG_ACTIVE ? "yes" : "no") << std::endl ; 
  
  streamlog_logger_out(logger, DEBUG) << "DEBUG   from test logger" << std::endl ;
  streamlog_logger_out(logger, MESSAGE) << "MESSAGE from test logger" << std::endl ;
  streamlog_logger_out(logger, WARNING) << "WARNING from test logger" << std::endl ;
  streamlog_logger_out(logger, ERROR) << "ERROR   from test logger" << std::endl ;
  streamlog_logger_out(logger, DEBUG) << "DEBUG   from test logger" << std::endl ;
  streamlog_logger_out(logger, ERROR) << "ERROR   from test logger" << std::endl ;
  
  streamlog_out(ERROR)   << "ERROR   from main logger" << std::endl ;

  return 0;
}

