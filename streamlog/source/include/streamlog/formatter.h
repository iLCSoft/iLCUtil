// -*- mode: c++;
#ifndef streamlog_formatter_h
#define streamlog_formatter_h

// -- std headers
#include <sstream>
#include <bitset>

//  -- streamlog headers
#include "streamlog/loglevels.h"
#include "streamlog/logmessage.h"

namespace streamlog {
  
  /**
   *  @brief  formatter class
   *  Base class for logging message formatting
   */
  template <typename string_type>
  class formatter {
  public:
    virtual ~formatter() = default ;
    
  protected:
    formatter(const formatter&) = default ;
    formatter &operator=(const formatter&) = default ;
    
  public:
    /**
     *  @brief  Format the log message in the stringstream output object
     * 
     *  @param  msg the input message to format
     *  @param  out the output stringstream to receive
     */
    virtual void format( const logmessage<string_type>& msg, std::stringstream &out ) = 0 ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  /**
   *  @brief  standard_formatter class
   *  Customizable formatting using option flags
   */
  template <typename string_type>
  class standard_formatter : public formatter<string_type> {
  public:
    enum class print_option {
      logger,     ///< Whether to print the logger name
      level,      ///< Whether to print the level name
      threadid,   ///< Whether to print the thread id
      time,       ///< Whether to print the message time
      color       ///< Whether to color the message
    };
    static const int noptions = 5 ;
    
  public:
    /**
     *  @brief  Constructor
     */
    standard_formatter() ;

    /**
     *  @brief  Whether a print option is set
     *
     *  @param  opt the option to check
     */
    bool optionSet( print_option opt ) const ;
    
    /**
     *  @brief  Set a print option
     * 
     *  @param  opt the option to set/unset
     *  @param  val whether to set or unset the option
     */
    void setOption( print_option opt , bool val = true ) ;
    
  private:
    std::string colorPrefix( const logmessage<string_type>& msg ) const ;
    std::string resetColor() const ;
    void format( const logmessage<string_type>& msg, std::stringstream &out ) ;
    
  private:
    /// The printing options
    std::bitset<noptions>    _options {} ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  template <typename string_type>
  inline standard_formatter<string_type>::standard_formatter() {
    setOption( print_option::logger , true ) ;
    setOption( print_option::level , true ) ;
  }
  
  //--------------------------------------------------------------------------

  template <typename string_type>
  inline bool standard_formatter<string_type>::optionSet( print_option opt ) const {
    return _options.test( static_cast<std::size_t>(opt) ) ;
  }
  
  //--------------------------------------------------------------------------

  template <typename string_type>
  inline void standard_formatter<string_type>::setOption( print_option opt , bool val ) {
    _options.set( static_cast<std::size_t>(opt), val ) ;
  }
  
  //--------------------------------------------------------------------------

  template <typename string_type>
  inline std::string standard_formatter<string_type>::colorPrefix( const logmessage<string_type>& msg ) const {
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

  template <typename string_type>
  inline std::string standard_formatter<string_type>::resetColor() const {
    return "\033[0m" ;
  }
  
  //--------------------------------------------------------------------------

  template <typename string_type>
  inline void standard_formatter<string_type>::format( const logmessage<string_type>& msg, std::stringstream &out ) {
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
    if ( withColor ) {
      out << resetColor() ;
    }
  }

}

#endif
