#include "streamlog/logstream.h"
#include "streamlog/loglevels.h"

namespace streamlog {
  
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
