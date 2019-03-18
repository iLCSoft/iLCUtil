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
    formatter(const formatter&) = default ;
    formatter &operator=(const formatter&) = default ;
    
  public:
    /**
     *  @brief  Format the log message in the stringstream output object
     * 
     *  @param  msg the input message to format
     *  @param  out the output stringstream to receive
     */
    virtual void format( const logmessage& msg, std::stringstream &out ) = 0 ;
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
    std::string colorPrefix( const logmessage& msg ) const ;
    std::string resetColor() const ;
    void format( const logmessage& msg, std::stringstream &out ) ;
    
  private:
    /// The printing options
    std::bitset<noptions>    _options {} ;
  };

}

#endif