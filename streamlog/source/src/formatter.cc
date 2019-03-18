#include "streamlog/formatter.h"

namespace streamlog {

  inline standard_formatter::standard_formatter() {
    setOption( print_option::logger , true ) ;
    setOption( print_option::level , true ) ;
  }

  //--------------------------------------------------------------------------

  inline bool standard_formatter::optionSet( print_option opt ) const {
    return _options.test( static_cast<std::size_t>(opt) ) ;
  }

  //--------------------------------------------------------------------------

  inline void standard_formatter::setOption( print_option opt , bool val ) {
    _options.set( static_cast<std::size_t>(opt), val ) ;
  }

  //--------------------------------------------------------------------------

  inline std::string standard_formatter::colorPrefix( const logmessage<string_type>& msg ) const {
    if ( msg._logLevel > streamlog::error_base_level ) {
      return "\033[1m\033[31m" ;
    }
    if ( msg._logLevel > streamlog::warning_base_level ) {
      return "\033[1m\033[33m" ;
    }
    if ( msg._logLevel > streamlog::message_base_level ) {
      return "\033[1m\033[34m" ;
    }
    if ( msg._logLevel > streamlog::debug_base_level ) {
      return "\033[32m" ;
    }
    return "";
  }

  //--------------------------------------------------------------------------

  inline std::string standard_formatter::resetColor() const {
    return "\033[0m" ;
  }

  //--------------------------------------------------------------------------

  inline void standard_formatter::format( const logmessage<string_type>& msg, std::stringstream &out ) {
    const bool printLogger = optionSet( print_option::logger ) ;
    const bool printLevel = optionSet( print_option::level ) ;
    const bool printLoggerOrLevel = ( printLogger || printLevel ) ;
    const bool withColor = optionSet( print_option::color ) ;
    if ( withColor ) {
      out << colorStart( msg ) ;
    }
    // date and time
    if ( optionSet( print_option::time ) ) {
      auto tt = std::chrono::system_clock::to_time_t( msg ) ;
      std::tm tm {} ;
      // deal with thread safety details here
  #if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
      localtime_s(&tm, &tt) ;
  #else
      localtime_r(&tt, &tm) ; // POSIX
  #endif
      char buffer[1024] ;
      auto success = std::strftime( buffer, 1024, "%c", &tm ) ;
      out << "[" << std::string(buffer) << "] " ;
    }
    if ( printLoggerOrLevel ) {
      out << "[" ;
      // log level
      if ( printLevel ) {
        out << " " << msg._level ;
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
    if ( withColor ) {
      out << resetColor() ;
    }
  }

}
