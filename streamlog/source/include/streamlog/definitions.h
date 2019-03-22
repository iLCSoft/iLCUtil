// -*- mode: c++;
#ifndef streamlog_definitions_h
#define streamlog_definitions_h

#include <string>
#include <ostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

/**
 *  The variable STREAMLOG_DEFAULT_LOGGER_TS can be tweaked before including this header:
 *  @code
 *  #define STREAMLOG_DEFAULT_LOGGER_TS
 *  #include <streamlog/definitions.h>
 *  // do some stuff with it
 *  @endcode
 *  or by using the build system with -STREAMLOG_DEFAULT_LOGGER_TS .
 *  This enable or disable thread safety of the default logger
 */
namespace streamlog {

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

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

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
    // default and reset colors
    static const color_helper default_color ;
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

  inline std::ostream &operator<<( std::ostream &out, const color_helper &c ) {
    out << c.str() ;
    return out ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  Helper function for c++11/14 transition
   */
  template <typename T, typename ...Args>
  inline std::unique_ptr<T> make_unique(Args ...args) {
#ifdef __cpp_lib_make_unique
    return std::make_unique<T>( args... ) ;
#else
    return std::unique_ptr<T>( new T( args... ) ) ;
#endif
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  class logsink ;
  class formatter ;

  using logsink_ptr = std::shared_ptr<logsink> ;
  using logsink_list = std::vector<logsink_ptr> ;
  using formatter_ptr = std::unique_ptr<formatter> ;
  using clock = std::chrono::system_clock ;
  using time_point = clock::time_point ;
  using thread_id  = std::thread::id ;
  using mt = std::mutex ;
  using st = nullmutex ;
#ifdef STREAMLOG_DEFAULT_LOGGER_TS // default logger thread safety
  using default_logger_mutex = mt ;
#else
  using default_logger_mutex = st ;
#endif
#ifdef STREAMLOG_COUT_TS // cout thread safety
extern streamlog::mt cout_mutex ;
#define COUT_LOG( message ) { std::lock_guard<streamlog::mt> lock(cout_mutex) ; std::cout << message ; }
#else
#define COUT_LOG( message ) { std::cout << message ; }
#endif




}

#endif
