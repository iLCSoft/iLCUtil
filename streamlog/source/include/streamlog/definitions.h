// -*- mode: c++;
#ifndef streamlog_definitions_h
#define streamlog_definitions_h

#include <string>
#include <ostream>

/**
 *  The variable STREAMLOG_LOGGING_TS can be tweaked before including this header:
 *  @code
 *  #define STREAMLOG_LOGGING_TS
 *  #include <streamlog/definitions.h>
 *  // do some stuff with it
 *  @endcode
 *  or while using the build system with -STREAMLOG_LOGGING_TS .
 *  This enable or disable thread safety of logging messages
 */
namespace streamlog {

  /**
   *  @brief  code_code class
   *  Defines color codes (SGR parameters, see https://en.wikipedia.org/wiki/ANSI_escape_code)
   */
  class color_code {
  public:
    // general constants
    static const unsigned int reset       = 0 ;
    static const unsigned int bold        = 1 ;
    static const unsigned int nobold      = 22 ;
    static const unsigned int bg_shift    = 10 ;
    // foreground color codes
    static const unsigned int fg_default  = 39 ;
    static const unsigned int fg_black    = 30 ;
    static const unsigned int fg_red      = 31 ;
    static const unsigned int fg_green    = 32 ;
    static const unsigned int fg_yellow   = 33 ;
    static const unsigned int fg_blue     = 34 ;
    static const unsigned int fg_magenta  = 35 ;
    static const unsigned int fg_cyan     = 36 ;
    static const unsigned int fg_white    = 37 ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  color_helper class
   *  Helper class to setup colors in console
   */
  class color_helper {
  public:
    /**
     *  @brief  Constructor. Setup a color
     *
     *  @param  fgc the foreground color
     *  @param  bold whether to apply bold
     */
    color_helper( unsigned int fgc, bool bold ) :
      _str( createStr( fgc, bold ) ) {
        /* nop */
    }

    /**
     *  @brief  Constructor. Setup the reset string
     */
    color_helper() :
      _str( "\033[0m" ) {
        /* nop */
    }

    /**
     *  @brief  Get the ANSI color string
     */
    const std::string &str() const {
      return _str ;
    }

  private:
    /**
     *  @brief  Create the color string
     *
     *  @param  fgc the foreground color
     *  @param  bold whether to apply bold
     */
    std::string createStr( unsigned int fgc, bool bold ) const {
      return "\033["
        + std::to_string( fgc ) + ";"
        + std::to_string( bold ? color_code::bold : color_code::nobold ) + "m";
    }

  private:
    /// The full color string to be aadded to an ostream to color it
    const std::string        _str ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  color class
   *  Defines builtin terminal colors
   */
  class color {
  public:
    // reset color
    static const color_helper reset ;
    // normal colors
    static const color_helper normal_black ;
    static const color_helper normal_red ;
    static const color_helper normal_green ;
    static const color_helper normal_yellow ;
    static const color_helper normal_blue ;
    static const color_helper normal_magenta ;
    static const color_helper normal_cyan ;
    static const color_helper normal_white ;
    // bold colors
    static const color_helper bold_black ;
    static const color_helper bold_red ;
    static const color_helper bold_green ;
    static const color_helper bold_yellow ;
    static const color_helper bold_blue ;
    static const color_helper bold_magenta ;
    static const color_helper bold_cyan ;
    static const color_helper bold_white ;
  };

  //--------------------------------------------------------------------------

  std::ostream &operator<<( std::ostream &out, const color_helper &c ) {
    out << c.str() ;
    return out ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  nullmutex class
   *  Standard Mutex interface without actual lock
   */
  class nullmutex {
  public:
    nullmutex() = default ;
    ~nullmutex() = default ;
    nullmutex(nullmutex &&) = delete ;
    nullmutex(const nullmutex&) = delete ;

  public:
    void lock () {}
    void unlock() {}
    void try_lock() {}
  };

#ifdef STREAMLOG_LOGGING_TS // logging thread safety
#include <mutex>
  using mutex = std::mutex ;
#else
  using mutex = nullmutex ;
#endif

}

#endif
