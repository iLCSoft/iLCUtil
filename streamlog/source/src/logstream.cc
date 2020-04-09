#include "streamlog/logstream.h"
#include "streamlog/prefix.h"

namespace streamlog{

 
  logstream::logstream() : _prefix( new streamlog::prefix ){} 

  logstream::~logstream() {

    if( _prefix ){
      delete _prefix ;
      _prefix = NULL ; 
    }
  }
  
  void logstream::init( std::ostream& os , const std::string name ) {
    
    static bool first=true ;
    
    if( first && os ) {

      _os = &os ;

      _prefix->_name = name ;

      first = false ;
    }
    
    else if( !os) {
      std::cerr << "ERROR: logstream::init() invalid ostream given " << std::endl ;      
    }
  }

  unsigned logstream::setLevel( const std::string& levelName ) {

    unsigned l = _level ;
    LevelMap::iterator it = _map.find( levelName ) ;
    if( it != _map.end() ) {
      _level = it->second ;
    }
    return l ;
  } 

  printthread logstream::operator()() { 
    
    if( _active && _os ) {
      
      _active = false ;
      
      return printthread{(*_prefix)(),_os} ;
    }
    else
     return printthread{} ;
  }

  /** thread global instance of logstream */
  thread_local logstream out ;

  /// global mutex for actual io
  std::mutex printthread::_mutexPrint{};
}
