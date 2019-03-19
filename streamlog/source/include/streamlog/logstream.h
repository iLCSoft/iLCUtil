// -*- mode: c++;
#ifndef logstream_h
#define logstream_h

// -- streamlog headers
#include "streamlog/definitions.h"
#include "streamlog/logsink.h"

// -- std headers
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <map>

namespace streamlog {

  /** 
   *  @brief  logstream class.
   *  
   *  Class defining a log stream that is used to print log messages depending
   *  on current log level. Can be initialized with different sinks (console, file, ...).
   *  There is one global instance of this class defined in the library accessible through
   *  the method logstream::global() <br>
   *  Typically only this instance is needed, e.g.: <br>
   *  <pre>
   *    // in int main() :
   *    streamlog::global().setName( argv[0] ) ;
   *
   *    //...
   *
   *    if( streamlog::global().write< streamlog::DEBUG1 >() )
   *       streamlog::global() << " this message will only be printed if level >= DEBUG1::level "
   *                        << std::endl ;
   *
   *    // or the same, simply using a macro:
   *
   *    streamlog_out( DEBUG )  << " this message will only be printed if level >= DEBUG1::level "
   *                            << std::endl ;
   *
   *
   *  </pre>
   *  Note that with the above calling sequence or the macro no runtime overhead is created
   *  if streamlog::DEBUG1::active is false and else if the log level is smaller than
   *  streamlog::DEBUG1::level no formatting of the message will happen, i.e. also very little
   *  runtime cost is involved. <br>
   *
   *  @author F. Gaede, DESY
   *  @author R. Ete, DESY
   *  @version $Id: logstream.h,v 1.3 2007-08-08 13:08:34 gaede Exp $
   */
  class logstream {
    // typedefs
    typedef std::map<std::string, unsigned int>    level_map ;
    typedef std::unique_ptr<formatter>             formatter_ptr ;
    typedef std::vector<logsink_ptr>               logsink_list ;

  public:
    logstream(const logstream&) = delete ;
    logstream& operator=(const logstream&) = delete ;
    ~logstream() = default ;
    
    /**
     *  @brief  Return the global logger instance (always defined)
     */
    static logstream &global() ;

    /**
     *  @brief  Constructor.
     *  The logger is initialized with a console sink (mt)
     */
    logstream() ;

    /**
     *  @brief  Constructor.
     *  The logger is initialized with a console sink (mt)
     *
     *  @param  name the logger name
     */
    logstream( const std::string &name ) ;

    /**
     *  @brief  Constructor
     *
     *  @param  name the logger name
     *  @param  sinks the list of sinks to use
     */
    logstream( const std::string &name , const logsink_list &sinks ) ;

    /**
     *  @brief  Constructor
     *
     *  @param  name the logger name
     *  @param  sink the single sink to use
     */
    logstream( const std::string &name , const logsink_ptr &sink ) ;

    /**
     *  @brief  Set the logger name
     *
     *  @param  name the logger name
     */
    void setName( const std::string &name ) ;

    /**
     *  @brief  Get the logger name
     */
    const std::string &name() const ;
    
    /**
     *  @brief  Set the formatter instance
     *  The formatter is cloned and set for every sink. The sinks must be 
     *  set before calling this methods else the formatter won't be adopted 
     *  
     *  @param  formatter the formatter to clone and set
     */
    void setFormatter( formatter_ptr formatter ) ;
    
    /**
     *  @brief  Return the logger instance (*this) after being configured for 
     *  logging with the specified log level. Usage:
     *  @code{cpp}
     *  logstream logger( "main" ) ;
     *  logger.addDefaultLevels() ;
     *  logger.setLevel<MESSAGE>() ;
     *  // this will log something
     *  logger.log<MESSAGE2>() << "Hello world !" << std::endl ;
     *  // this won't log anything
     *  logger.log<DEBUG3>() << "Nobody's there ? :-(" << std::endl ;
     *  @endcode
     */
    template <typename T>
    logstream &log() ;
    
    /** 
     *  @brief  True if next log message of the current level (class T ) will be written, i.e.
     *  the next call to logstream& operator<<() will return a valid logstream. 
     */
    template<class T>
    bool write() ;

    /** 
     *  @brief  True if next log message of the current level (class T ) would be written
     *  Can be used to conditionally execute code blocks that are needed before writing to
     *  the outstream.
     */
    template<class T>
    bool would_write() ;

    /**
     *  @brief  Overloaded stream operator.
     *
     *  @param  t the object to log
     */
    template <typename T>
    logstream &operator<<( const T &t ) ;

    /** Adds a level name to the stream for setting the log level with a string through
     *  a scope class. Only names added with this method will have an effect - other log
     *  levels can only be activated with logstream::logscope::setLevel<T>().
     *  Useful for framework programs where the log level can be changed via a steering
     *  parameter.
     *
     *  @see logstream::logscope::setLevel(const std::string levelName )
     */
    template <class T>
    void addLevelName() ;

    /** Set the current level (level and name)
     */
    template <typename T>
    unsigned int setLevel() ;

    /** 
     *  @brief  Add all default levels defined in loglevels.h to this logstream
     */
    void addDefaultLevels() ;
    
    /**
     *  @brief  Get the current log level
     */
    unsigned int level() const ;

    /**
     *  @brief  Get the current log level name
     */
    const std::string &levelName() const ;

  protected:
    /**
     *  @brief  Used internally by write<T>
     */
    template<class T>
    bool check_level() ;

  private:
    /// The logger name
    std::string         _name {"UNKNOWN"} ;
    /// The logger sinks
    logsink_list        _sinks {} ;
    /// current log level
    unsigned int        _level {0} ;
    /// current log level name
    std::string         _levelName {"VERBOSE"} ;
    /// boolean helper
    bool                _active {false} ;
    /// string map of level names
    level_map           _levelMap {} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename T>
  inline logstream &logstream::log() {
    (void)write<T>();
    return *this ;
  }
  
  //--------------------------------------------------------------------------

  template <typename T>
  inline logstream &logstream::operator<<( const T &t ) {
    if( _active ) {
      logmessage msg ;
      msg._message << t ;
      msg._levelName = _levelName ;
      msg._level = _level ;
      msg._loggerName = _name ;
      for( auto &sink : _sinks ) {
        sink->log( msg ) ;
      }
      _active = false ;
    }
    return *this ;
  }
  
  //--------------------------------------------------------------------------
  
  template<class T>
  inline bool logstream::write() {
    // dont' call chek_level if T::active == false
    return (  T::active   &&    check_level<T>()  ) ;
  }

  //--------------------------------------------------------------------------

  template<class T>
  inline bool logstream::would_write() {
    return (  T::active   &&  T::level >= _level ) ;
  }
  
  //--------------------------------------------------------------------------

  template <class T>
  inline void logstream::addLevelName() {
    _levelMap[ T::name() ] = T::level ;
  }
  
  //--------------------------------------------------------------------------

  template <typename T>
  inline unsigned int logstream::setLevel() {
    unsigned int l = _level ;
    auto iter = _levelMap.find( T::name() ) ;
    if( iter != _levelMap.end() ) {
      _level = iter->second ;
      _levelName = iter->first ;
    }
    return l ;
  }
  
  //--------------------------------------------------------------------------

  template<class T>
  inline bool logstream::check_level() {
    if( T::level >= _level ){
      _active = true ;
      setLevel<T>() ;
    }
    return _active ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  // extern logstream out ;

}
#endif
