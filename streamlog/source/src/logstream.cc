#include "streamlog/logstream.h"
#include "streamlog/loglevels.h"

namespace streamlog {
  
  logstream::logstreambuf::logstreambuf( logstream &stream ) : 
    _stream(stream) {
    /* nop */
  }
  
  //-------------------------------------------------------------------------- 

  logstream::logstreambuf::int_type logstream::logstreambuf::overflow( int_type ch ) {
    if ( c == traits_type::eof() ) {
      return traits_type::eof() ;
    }
    bool success = true;

    // add prefix to log message
    if ( _newLine == true ) {
      for ( auto &sink : _stream._sinks ) {
        sink->log( sink->prefix( _context ) ) ;
      }
      _newLine = false ;
    }

    if ( c == '\n' ) {
      _newLine = true ;
    }

    if ( success )
    success &= ( _sbuf->sputc(c) != EOF ) ;

    if( success ) 
    return 0 ;

    return EOF ;

    // if ( ch == '\n' ) {
    //   writeMessage(false) ;
    //   _msg._message.str("") ;
    // }
    return base_type::overflow( ch ) ;
  }
  
  //--------------------------------------------------------------------------
  
  // std::streamsize logstream::logstreambuf::xsputn( const char_type* s, std::streamsize count ) {
  //   if ( count == 1 && *s == '\n' ) {
  //     if ( _active ) {
  //       // line break: flush the message 
  //       writeMessage(true) ;
  //       resetMessage() ;
  //     }
  //     return 1 ;
  //   }
  //   if ( _active ) {
  //     // append message untill flush is called or new line is added
  //     _msg._message.write( s, count ) ;
  //     // std::cout << "Caching message : " << s << std::endl ;
  //     _emptyMessage = false ;
  //   }
  //   return count ;
  // }

  //--------------------------------------------------------------------------
  
  // int logstream::logstreambuf::sync() {
  //   return 0 ;
  // }

  //--------------------------------------------------------------------------
  
  // void logstream::logstreambuf::resetMessage() {
  //   _msg._message.str("") ;
  //   _msg._levelName = _levelName ;
  //   _msg._level = _level ;
  //   _msg._loggerName = _stream._name ;
  //   _emptyMessage = true ;
  // }
  // 
  // //--------------------------------------------------------------------------
  // 
  // void logstream::logstreambuf::writeMessage( bool newLine ) {
  //   if ( _emptyMessage ) {
  //     return ;
  //   }
  //   if ( newLine ) {
  //     _msg._message << std::endl ;      
  //   }
  //   for ( auto &sink : _stream._sinks ) {
  //     sink->log( _msg ) ;
  //     sink->flush() ;
  //   }
  // }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  logstream &logstream::global() {
    static logstream out ;
    return out ;
  }
  
  //--------------------------------------------------------------------------

  logstream::logstream() :
    _name("UNKNOWN"),
    _logbuffer( *this ),
    _logstream( &_logbuffer ) {
    // default sink is console
    _sinks.push_back( std::make_shared<console_sink_mt>() ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream( const std::string &name ) :
    _name(name),
    _logbuffer( *this ),
    _logstream( &_logbuffer ) {
    // default sink is console
    _sinks.push_back( std::make_shared<console_sink_mt>() ) ;
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream( const std::string &name , const logsink_list &sinks ) :
    _name(name),
    _sinks(sinks),
    _logbuffer( *this ),
    _logstream( &_logbuffer ) {
    addDefaultLevels() ;
  }

  //--------------------------------------------------------------------------

  logstream::logstream( const std::string &name , const logsink_ptr &sink ) :
    _name(name),
    _logbuffer( *this ),
    _logstream( &_logbuffer ) {
    _sinks.push_back( sink ) ;
    addDefaultLevels() ;
  }
  
  //--------------------------------------------------------------------------
  
  void logstream::flush() {
    _logstream.flush() ;
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
