// -*- mode: c++;
#ifndef streamlog_logsink_h
#define streamlog_logsink_h

// -- std headers
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdexcept>

// -- streamlog headers
#include <streamlog/definitions.h>
#include <streamlog/formatter.h>
#include <streamlog/logmessage.h>

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
  inline void logsink<mutex_type>::log( const logmessage &msg ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    doLog( msg ) ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void logsink<mutex_type>::flush() {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    doFlush() ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void logsink<mutex_type>::setFormatter( formatter_ptr formatter ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    _formatter = formatter ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  console_sink class
   *  Standard std::cout output
   */
  template <typename mutex_type>
  class console_sink : public logsink<mutex_type> {
  public:
    console_sink() = default ;

  private:
    void doLog( const logmessage &msg ) {
      std::stringstream ss ;
      logsink<mutex_type>::_formatter->format( msg , ss ) ;
      std::cout << ss.str() ;
    }

    void doFlush() {
      std::cout.flush() ;
    }
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  colored_console_sink class
   *  Standard std::cout output with fancy colors
   */
  template <typename mutex_type>
  class colored_console_sink : public logsink<mutex_type> {
  public:
    colored_console_sink() = default ;

  private:
    void doLog( const logmessage &msg ) {
      std::stringstream ss ;
      logsink<mutex_type>::_formatter->format( msg , ss ) ;
      std::cout << color( msg._logLevel ) << ss.str() << color::reset ;
    }

    void doFlush() {
      std::cout.flush() ;
    }

    const color_helper &color( unsigned int level ) const {
      if ( logstream::error_base_level ) {
        return color::bold_red ;
      }
      if ( logstream::warning_base_level ) {
        return color::bold_yellow ;
      }
      if ( logstream::message_base_level ) {
        return color::bold_blue ;
      }
      if ( logstream::debug_base_level ) {
        return color::green ;
      }
      return color::default_color ;
    }

  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  simple_file_sink class
   *  Standard std::cout output
   */
  template <typename mutex_type>
  class simple_file_sink : public logsink<mutex_type> {
  public:
    /**
     *  @brief  Constructor
     *
     *  @param  fname the log file name
     *  @param  mode the opening mode
     */
    simple_file_sink( std::string &fname, std::ios_base::openmode mode = std::ios_base::out ) {
      _fstream.open( fname, mode ) ;
      if ( not _fstream.is_open() ) {
        throw std::runtime_error( "simple_file_sink: couldn't open log file '" + fname + "'!" ) ;
      }
    }

    /**
     *  @brief  Destructor. Close the file stream
     */
    ~simple_file_sink() {
      _fstream.close() ;
    }

  private:
    void doLog( const logmessage &msg ) {
      std::stringstream ss ;
      logsink<mutex_type>::_formatter->format( msg , ss ) ;
      _fstream << ss.str() ;
    }

    void doFlush() {
      _fstream.flush() ;
    }

  private:
    /// The log file stream
    std::ofstream           _fstream {} ;
  };




}

#endif
