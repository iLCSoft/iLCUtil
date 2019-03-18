#include "streamlog/logsink.h"

namespace streamlog {

  void console_sink::doLog( const logmessage &msg ) {
    std::stringstream ss ;
    _formatter->format( msg , ss ) ;
    std::cout << ss.str() ;
  }
  
  //--------------------------------------------------------------------------
  
  void console_sink::doFlush() {
    std::cout.flush() ;
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

}
