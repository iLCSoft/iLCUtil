// -*- mode: c++;
#ifndef streamlog_logsink_h
#define streamlog_logsink_h

// -- std headers
#include <string>
#include <thread>
#include <chrono>

// -- streamlog headers
#include <streamlog/formatter.h>

namespace streamlog {
  
  template <typename mutex_type>
  class logsink {
  public:
    typedef std::shared_ptr<formatter> formatter_ptr ;
    
    logsink() = default ;
    
  public:
    /**
     *  @brief  log the message 
     * 
     *  @param  msg the message to log
     */
    void log( const logmessage &msg ) ;
    
    /**
     *  @brief  Flush out the message
     */
    void flush() ;
    
    /**
     *  @brief  Set the log message formatter to use
     * 
     *  @param  formatter the formatter to use
     */
    void setFormatter( formatter_ptr formatter ) ;
    
  protected:
    /**
     *  @brief  Performa the actual logging operation
     *  
     *  @param  msg the message to log
     */
    virtual void doLog( const logmessage &msg ) = 0 ;
    
    /**
     *  @brief   Performs the 
     */
    virtual void doFlush() = 0 ;
    
  protected:
    /// The log message formatter
    formatter_ptr          _formatter {nullptr} ;
    
  private:
    /// The logging mutex
    mutex_type             _mutex {} ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  template <typename mutex_type>
  inline void log( const logmessage &msg ) {
    std::lock_guard<mutex_type>( _mutex ) ;
    doLog( msg ) ;
  }
  
  //--------------------------------------------------------------------------
  
  template <typename mutex_type>
  inline void flush() {
    std::lock_guard<mutex_type>( _mutex ) ;
    doFlush() ;
  }
  
  //--------------------------------------------------------------------------
  
  template <typename mutex_type>
  inline void setFormatter( formatter_ptr formatter ) {
    std::lock_guard<mutex_type>( _mutex ) ;
    _formatter = formatter ;
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  console_sink class
   *  Standard std::cout output
   */
  template <typename mutex_type>
  class console_sink : public logsink<mutex_sink> {
  public:
    console_sink() = default ;
    
  private:
    void doLog( const logmessage &msg ) ;
    void doFlush() ;
  };
  
  /**
   *  @brief  simple_file_sink class
   *  Standard std::cout output
   */
  template <typename mutex_type>
  class simple_file_sink : public logsink<mutex_sink> {
  public:
    simple_file_sink() ;
    
  private:
    void doLog( const logmessage &msg ) ;
    void doFlush() ;
  };



  
}

#endif
