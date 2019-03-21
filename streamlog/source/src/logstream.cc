#include "streamlog/logstream.h"
#include "streamlog/loglevels.h"

namespace streamlog {

  outstream::streambuf::streambuf( outstream *s ) :
    _outstream(s) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  outstream::streambuf::streambuf( streambuf &&rhs ) :
    base_type(std::move(rhs)),
    _outstream(rhs._outstream),
    _writeOnDestruct(rhs._writeOnDestruct) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  outstream::streambuf &outstream::streambuf::operator=( streambuf &&rhs ) {
    base_type::operator=( rhs ) ;
    _outstream = rhs._outstream ;
    _writeOnDestruct = rhs._writeOnDestruct ;
    return *this ;
  }

  //--------------------------------------------------------------------------

  outstream::streambuf::int_type outstream::streambuf::overflow( int_type c ) {
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

  outstream::outstream( logstream *ls, const logcontext &ctx ) :
    _logstream(ls),
    _streambuf(this),
    _context(ctx),
    _active(true) {
    this->init(&_streambuf) ;
  }

  //--------------------------------------------------------------------------

  outstream::outstream( logstream *ls ) :
    _logstream(ls),
    _streambuf(this),
    _active(false) {
    this->init(&_streambuf) ;
  }

  //--------------------------------------------------------------------------

  outstream::outstream( outstream &&rhs ) :
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

  outstream::~outstream() {
    if ( not _active ) {
      return ;
    }
    if ( _streambuf._writeOnDestruct ) {
      writeMessage(true) ;
    }
  }

  //--------------------------------------------------------------------------

  outstream &outstream::operator=(outstream &&rhs) {
    base_type::operator=( std::move(rhs) ) ;
    _streambuf = std::move( rhs._streambuf ) ;
    _logstream = rhs._logstream ;
    _message = std::move(rhs._message) ;
    _context = rhs._context ;
    _active = rhs._active ;
    return *this ;
  }

  //--------------------------------------------------------------------------

  void outstream::writeMessage( bool newLine ) {
    if ( not _active ) {
      return ;
    }
    if ( newLine ) {
      _message << std::endl;
    }
    std::string message( std::move( _message.str() ) );
    for ( auto &sink : _logstream->_sinks ) {
      sink->log( _context, sink->prefix( _context ) + message ) ;
    }
    // reset the message after logging
    _message.str("");
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  logstream &logstream::global() {
    static logstream out ;
    return out ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream() :
    _name("UNKNOWN") {
    // default sink is console
    _sinks.push_back( std::make_shared<console_sink_mt>() ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream( const std::string &name ) :
    _name(name) {
    // default sink is console
    _sinks.push_back( std::make_shared<console_sink_mt>() ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream( const std::string &name , const logsink_list &sinks ) :
    _name(name),
    _sinks(sinks) {
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream( const std::string &name , const logsink_ptr &sink ) :
    _name(name) {
    _sinks.push_back( sink ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  void logstream::setName( const std::string &n ) {
    _name = n ;
  }

  //--------------------------------------------------------------------------

  const std::string &logstream::name() const {
    return _name ;
  }

  //--------------------------------------------------------------------------

  void logstream::setFormatter( formatter_ptr formatter ) {
    for ( auto& sink : _sinks ) {
      sink->setFormatter( formatter->clone() ) ;
    }
  }

  //--------------------------------------------------------------------------

  void logstream::addDefaultLevels() {
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

  unsigned int logstream::level() const {
    return _level ;
  }

  //--------------------------------------------------------------------------

  const std::string &logstream::levelName() const {
    return _levelName ;
  }

}
