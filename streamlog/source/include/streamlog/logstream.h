// -*- mode: c++;
#ifndef streamlog_logstream_h
#define streamlog_logstream_h

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

  template <typename mutex_type>
  class logstreamT ;
  class logstream ;

  /**
   *  @brief  outstream class
   *  Encapsulate a log message and its context.
   *  For every line break or end of logging line (meaning
   *  on object destruction) a new log message is sinked in
   *  the associated logstream object
   */
  template <typename mutex_type>
  class outstream : public std::ostream {
  public:
    // typedefs
    typedef std::ostream         base_type ;

  private:
    /**
     *  @brief  streambuf class
     *  Internal buffer used for detecing line breaks
     */
    class streambuf : public std::streambuf {
      friend class outstream<mutex_type> ;
    public:
      typedef std::streambuf             base_type ;
      typedef base_type::int_type        int_type ;
      typedef base_type::char_type       char_type ;
      typedef base_type::traits_type     traits_type ;

    public:
      streambuf( outstream<mutex_type> *s ) ;
      streambuf( streambuf &&rhs ) ;
      streambuf &operator=( streambuf &&rhs ) ;
      streambuf( const streambuf & ) = delete ;
      streambuf &operator=( const streambuf & ) = delete ;
      int_type overflow( int_type c = traits_type::eof() ) override ;

    private:
      outstream<mutex_type>    *_outstream {nullptr} ;
      bool                     _writeOnDestruct {true} ;
    };
    friend class streambuf ;

  public:
    outstream(const outstream<mutex_type> &) = delete ;
    outstream<mutex_type> &operator=(const outstream<mutex_type> &) = delete ;

    /**
     *  @brief  Constructor for active stream with logcontext
     */
    outstream( logstreamT<mutex_type> *ls, const logcontext &ctx ) ;

    /**
     *  @brief  Constructor for inactive stream
     */
    outstream( logstreamT<mutex_type> *ls ) ;

    /**
     *  @brief  Move contructor
     */
    outstream( outstream<mutex_type> &&rhs ) ;

    /**
     *  @brief  Destructor.
     *  Write buffered message if any
     */
    ~outstream() ;

    /**
     *  @brief  Assignement operator
     */
    outstream<mutex_type> &operator=(outstream<mutex_type> &&rhs) ;

    /**
     *  @brief  Log the buffered message to the logstream sinks
     */
    void writeMessage( bool newLine ) ;

  private:
    /// The logger instance
    logstreamT<mutex_type>* _logstream {nullptr} ;
    /// The internal stream buffer mediator
    streambuf               _streambuf ;
    /// The log message buffer
    std::stringstream       _message {} ;
    /// The log entry context
    logcontext              _context {} ;
    /// Whether the log entry is active
    bool                    _active {false} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

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
   *    logstream::global().setName( argv[0] ) ;
   *
   *    //...
   *
   *    if( logstream::global().write< streamlog::DEBUG1 >() )
   *       logstream::global() << " this message will only be printed if level >= DEBUG1::level "
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
  template <typename mutex_type>
  class logstreamT {
    friend class outstream<mutex_type> ;
    friend class logstream ;   // for global logger creation
    // typedefs
    typedef std::map<std::string, unsigned int>    level_map ;
    typedef std::unique_ptr<formatter>             formatter_ptr ;
    typedef std::vector<logsink_ptr>               logsink_list ;
    typedef outstream<mutex_type>                  stream_type ;

  private:
    /**
     *  @brief  Constructor
     *  Only available for the global logger instance
     */
    logstreamT() ;

  public:
    logstreamT(const logstreamT<mutex_type>&) = delete ;
    logstreamT& operator=(const logstreamT&) = delete ;
    ~logstreamT() = default ;

    /**
     *  @brief  Return the global logger instance (always defined)
     */
    // static logstream &global() ;

    /**
     *  @brief  Constructor.
     *  The logger is initialized with a console sink (mt)
     *
     *  @param  name the logger name
     */
    logstreamT( const std::string &name ) ;

    /**
     *  @brief  Constructor
     *
     *  @param  name the logger name
     *  @param  sinks the list of sinks to use
     */
    logstreamT( const std::string &name , const logsink_list &sinks ) ;

    /**
     *  @brief  Constructor
     *
     *  @param  name the logger name
     *  @param  sink the single sink to use
     */
    logstreamT( const std::string &name , const logsink_ptr &sink ) ;

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
     *  logger.setLevel<MESSAGE>() ;
     *  // this will log something
     *  logger.log<MESSAGE2>() << "Hello world !" << std::endl ;
     *  // this won't log anything
     *  logger.log<DEBUG3>() << "Nobody's there ? :-(" << std::endl ;
     *  @endcode
     */
    template <typename T>
    stream_type log() ;

    /**
     *  @brief  True if next log message of the current level (class T ) would be written
     *  Can be used to conditionally execute code blocks that are needed before writing to
     *  the outstream.
     */
    template<class T>
    bool wouldWrite() const ;

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
     *  @brief  Get the current log level
     */
    unsigned int level() const ;

    /**
     *  @brief  Get the current log level name
     */
    const std::string &levelName() const ;

  private:
    /**
     *  @brief  Add all default levels defined in loglevels.h to this logstream
     */
    void addDefaultLevels() ;

    /**
     *  @brief  Write the message to the sinks
     *
     *  @param  ctx the message context
     *  @param  message the message string
     */
    void writeMessage( const logcontext &ctx, const std::string &message ) ;

  private:
    /// The logger name
    std::string         _name {"UNKNOWN"} ;
    /// The logger sinks
    logsink_list        _sinks {} ;
    /// The current log level
    unsigned int        _level {0} ;
    /// The current log level name
    std::string         _levelName {"VERBOSE"} ;
    /// The string map of level names
    level_map           _levelMap {} ;
    /// The synchronization mutex
    mutable mutex_type  _mutex {} ;
  };

  using logstream_st = logstreamT<nullmutex> ;
  using logstream_mt = logstreamT<std::mutex> ;

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  logstream class
   *  Defined the global logger type.
   */
  class logstream {
  public:
    // Use tweakable thread safety at compile time
    // to define the default logger type.
    // See streamlog/definitions.h
    using logger_type = logstreamT<default_logger_mutex> ;

  public:
    /**
     *  @brief  Get the global logger instance
     */
    static logger_type &global() ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type>::streambuf::streambuf( outstream<mutex_type> *s ) :
    _outstream(s) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type>::streambuf::streambuf( streambuf &&rhs ) :
    base_type(std::move(rhs)),
    _outstream(rhs._outstream),
    _writeOnDestruct(rhs._writeOnDestruct) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline typename outstream<mutex_type>::streambuf &outstream<mutex_type>::streambuf::operator=( streambuf &&rhs ) {
    base_type::operator=( rhs ) ;
    _outstream = rhs._outstream ;
    _writeOnDestruct = rhs._writeOnDestruct ;
    return *this ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline typename outstream<mutex_type>::streambuf::int_type outstream<mutex_type>::streambuf::overflow( int_type c ) {
    if ( not _outstream->_active ) {
      return traits_type::not_eof( c ) ;
    }
    char_type ch = traits_type::to_char_type(c) ;
    // reached end of line: write the message
    if (ch == '\n') {
      // insert line break to finish properly
      _outstream->_message << std::endl ;
      _outstream->writeMessage(false) ;
      _writeOnDestruct = false ;
      return traits_type::not_eof( c ) ;
    }
    _writeOnDestruct = true ;
    _outstream->_message << ch ;
    return traits_type::not_eof( c ) ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type>::outstream( logstreamT<mutex_type> *ls, const logcontext &ctx ) :
    _logstream(ls),
    _streambuf(this),
    _context(ctx),
    _active(true) {
    this->init(&_streambuf) ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type>::outstream( logstreamT<mutex_type> *ls ) :
    _logstream(ls),
    _streambuf(this),
    _active(false) {
    this->init(&_streambuf) ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type>::outstream( outstream &&rhs ) :
    base_type(std::move(rhs)),
    _logstream(rhs._logstream),
    _streambuf(std::move(rhs._streambuf)),
    _message(std::move(rhs._message)),
    _context(rhs._context),
    _active(rhs._active) {
    // move assignement is not enough, need to set the
    // internal streambuf using setter method
    base_type::set_rdbuf( &_streambuf ) ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type>::~outstream() {
    if ( not _active ) {
      return ;
    }
    if ( _streambuf._writeOnDestruct ) {
      writeMessage(true) ;
    }
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline outstream<mutex_type> &outstream<mutex_type>::operator=(outstream &&rhs) {
    base_type::operator=( std::move(rhs) ) ;
    _streambuf = std::move( rhs._streambuf ) ;
    _logstream = rhs._logstream ;
    _message = std::move(rhs._message) ;
    _context = rhs._context ;
    _active = rhs._active ;
    return *this ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void outstream<mutex_type>::writeMessage( bool newLine ) {
    if ( not _active ) {
      return ;
    }
    if ( newLine ) {
      _message << std::endl;
    }
    std::string message( std::move( _message.str() ) );
    _logstream->writeMessage( _context, message ) ;
    // reset the message after logging
    _message.str("");
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline logstreamT<mutex_type>::logstreamT() :
    _name("UNKNOWN") {
    // default sink is console
    _sinks.push_back( std::make_shared<console_sink<mutex_type>>() ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline logstreamT<mutex_type>::logstreamT( const std::string &name ) :
    _name(name) {
    // default sink is console
    _sinks.push_back( std::make_shared<console_sink<mutex_type>>() ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline logstreamT<mutex_type>::logstreamT( const std::string &name , const logsink_list &sinks ) :
    _name(name),
    _sinks(sinks) {
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline logstreamT<mutex_type>::logstreamT( const std::string &name , const logsink_ptr &sink ) :
    _name(name) {
    _sinks.push_back( sink ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void logstreamT<mutex_type>::setName( const std::string &n ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    _name = n ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline const std::string &logstreamT<mutex_type>::name() const {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    return _name ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void logstreamT<mutex_type>::setFormatter( formatter_ptr formatter ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    for ( auto& sink : _sinks ) {
      sink->setFormatter( formatter->clone() ) ;
    }
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void logstreamT<mutex_type>::addDefaultLevels() {
    addLevelName<DEBUG>() ;
    addLevelName<DEBUG0>() ;
    addLevelName<DEBUG1>() ;
    addLevelName<DEBUG2>() ;
    addLevelName<DEBUG3>() ;
    addLevelName<DEBUG4>() ;
    addLevelName<DEBUG5>() ;
    addLevelName<DEBUG6>() ;
    addLevelName<DEBUG7>() ;
    addLevelName<DEBUG8>() ;
    addLevelName<DEBUG9>() ;
    addLevelName<MESSAGE>() ;
    addLevelName<MESSAGE0>() ;
    addLevelName<MESSAGE1>() ;
    addLevelName<MESSAGE2>() ;
    addLevelName<MESSAGE3>() ;
    addLevelName<MESSAGE4>() ;
    addLevelName<MESSAGE5>() ;
    addLevelName<MESSAGE6>() ;
    addLevelName<MESSAGE7>() ;
    addLevelName<MESSAGE8>() ;
    addLevelName<MESSAGE9>() ;
    addLevelName<WARNING>() ;
    addLevelName<WARNING0>() ;
    addLevelName<WARNING1>() ;
    addLevelName<WARNING2>() ;
    addLevelName<WARNING3>() ;
    addLevelName<WARNING4>() ;
    addLevelName<WARNING5>() ;
    addLevelName<WARNING6>() ;
    addLevelName<WARNING7>() ;
    addLevelName<WARNING8>() ;
    addLevelName<WARNING9>() ;
    addLevelName<ERROR>() ;
    addLevelName<ERROR0>() ;
    addLevelName<ERROR1>() ;
    addLevelName<ERROR2>() ;
    addLevelName<ERROR3>() ;
    addLevelName<ERROR4>() ;
    addLevelName<ERROR5>() ;
    addLevelName<ERROR6>() ;
    addLevelName<ERROR7>() ;
    addLevelName<ERROR8>() ;
    addLevelName<ERROR9>() ;
    addLevelName<SILENT>() ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void logstreamT<mutex_type>::writeMessage( const logcontext &ctx, const std::string &message ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    for ( auto &sink : _sinks ) {
      sink->log( ctx, sink->prefix( ctx ) + message ) ;
    }
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline unsigned int logstreamT<mutex_type>::level() const {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    return _level ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline const std::string &logstreamT<mutex_type>::levelName() const {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    return _levelName ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  template <typename T>
  inline typename logstreamT<mutex_type>::stream_type logstreamT<mutex_type>::log() {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    const bool active = ( T::active && T::level >= _level ) ;
    if ( active ) {
      // configure a context and create an active stream
      logcontext ctx {} ;
      ctx._loggerName = _name ;
      ctx._level = T::level ;
      ctx._levelName = T::name() ;
      return stream_type( this, ctx ) ;
    }
    else {
      // inactive stream is not configured with a context
      return stream_type( this ) ;
    }
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  template <class T>
  inline bool logstreamT<mutex_type>::wouldWrite() const {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    return (  T::active   &&  T::level >= _level ) ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  template <class T>
  inline void logstreamT<mutex_type>::addLevelName() {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    _levelMap[ T::name() ] = T::level ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  template <typename T>
  inline unsigned int logstreamT<mutex_type>::setLevel() {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    unsigned int l = _level ;
    auto iter = _levelMap.find( T::name() ) ;
    if( iter != _levelMap.end() ) {
      _level = iter->second ;
      _levelName = iter->first ;
    }
    return l ;
  }

}
#endif
