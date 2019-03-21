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
  class formatter {
  public:
    virtual ~formatter() = default ;

  protected:
    formatter() = default ;
    formatter(const formatter&) = default ;
    formatter &operator=(const formatter&) = default ;

  public:
    /**
     *  @brief  Generate a prefix string for the log message
     *
     *  @param  msg the input message to format
     */
    virtual std::string prefix( const logcontext& ctx ) = 0 ;
    
    /**
     *  @brief  Clone the formatter as a unique pointer
     */
    virtual std::unique_ptr<formatter> clone() const = 0 ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  standard_formatter class
   *  Customizable formatting using option flags
   */
  class standard_formatter : public formatter {
  public:
    enum class print_option {
      logger,     ///< Whether to print the logger name
      level,      ///< Whether to print the level name
      threadid,   ///< Whether to print the thread id
      time        ///< Whether to print the message time
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
    standard_formatter( const std::bitset<noptions> &bs ) ;
    std::string prefix( const logcontext& ctx ) ;
    std::unique_ptr<formatter> clone() const ;

  private:
    /// The printing options
    std::bitset<noptions>    _options {} ;
  };

}

#endif
