#include "streamlog/formatter.h"

namespace streamlog {

  standard_formatter::standard_formatter() {
    setOption( print_option::logger , true ) ;
    setOption( print_option::level , true ) ;
  }
  
  //--------------------------------------------------------------------------
  
  standard_formatter::standard_formatter( const std::bitset<noptions> &bs ) :
    _options(bs) {
    
  }

  //--------------------------------------------------------------------------

  bool standard_formatter::optionSet( print_option opt ) const {
    return _options.test( static_cast<std::size_t>(opt) ) ;
  }

  //--------------------------------------------------------------------------

  void standard_formatter::setOption( print_option opt , bool val ) {
    _options.set( static_cast<std::size_t>(opt), val ) ;
  }

  //--------------------------------------------------------------------------

  void standard_formatter::format( const logmessage& msg, std::stringstream &out ) {
    const bool printLogger = optionSet( print_option::logger ) ;
    const bool printLevel = optionSet( print_option::level ) ;
    const bool printLoggerOrLevel = ( printLogger || printLevel ) ;
    // date and time
    if ( optionSet( print_option::time ) ) {
      auto tt = std::chrono::system_clock::to_time_t( msg._time ) ;
      std::tm tm {} ;
      // deal with thread safety details here
  #if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
      localtime_s(&tm, &tt) ;
  #else
      localtime_r(&tt, &tm) ; // POSIX
  #endif
      char buffer[1024] ;
      std::strftime( buffer, 1024, "%c", &tm ) ;
      out << "[" << std::string(buffer) << "] " ;
    }
    if ( printLoggerOrLevel ) {
      out << "[" ;
      // log level
      if ( printLevel ) {
        out << " " << msg._levelName ;
      }
      // logger name
      if ( printLogger ) {
        out << "\"" << msg._loggerName << "\"" ;
      }
      out << "] " ;
    }
    // thread id
    if ( optionSet( print_option::threadid ) ) {
      out << "[thread " << msg._threadId << "] " ;
    }
    out << msg._message.str() ;
  }
  
  //--------------------------------------------------------------------------
  
  std::unique_ptr<formatter> standard_formatter::clone() const {
    return std::unique_ptr<formatter>( new standard_formatter( _options ) ) ;
  }

}
