#include "streamlog/logstream.h"
#include "streamlog/loglevels.h"
#include "streamlog/logbuffer.h"
#include "streamlog/prefix.h"

namespace streamlog{
  
  logstream::logstream() : 
    _ns( new nullstream ) , 
    _os(0) , 
    _level(0) ,
    _active(false) , 
    _lb(0),
    _prefix( std::make_shared<streamlog::prefix>()) {
    /* nop */
  }
  
  //--------------------------------------------------------------------------

  logstream::~logstream() {
    
    if( _ns ){ 
        delete _ns ;
        _ns = NULL ;
    }
    
    if( _os ){ 
      delete _os ;
      _os = NULL ;
    }
    
    if( _lb ){
      delete _lb ;
      _lb = NULL ;
    }
  }
  
  //--------------------------------------------------------------------------
  
  void logstream::init( std::ostream& os , const std::string name ) {
    
    static bool first=true ;
    
    if( first && os ) {
      
      //      _name = name ;      
      
      // create a new log buffer and attach this to a wrapper to the given ostream  

      _lb = new logbuffer( os.rdbuf() , this ) ;
      
      _os = new std::ostream( _lb ) ;
      
      //attach also the original stream to the logger...
      //os.rdbuf( _lb ) ; // this needs some work 
      // FIXME : this needs to go to the c'tor !!!!
     
      _prefix->_name = name ;

      first = false ;
    }
    
    else if( !os) {
      std::cerr << "ERROR: logstream::init() invalid ostream given " << std::endl ;      
    }
  }
  
  //--------------------------------------------------------------------------

  unsigned logstream::setLevel( const std::string& levelName ) {

    unsigned l = _level ;
    LevelMap::iterator it = _map.find( levelName ) ;
    if( it != _map.end() ) {
      _level = it->second ;
    }
    return l ;
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

  std::ostream& logstream::operator()() { 
    
    if( _active && _os ) {
      
      _active = false ;
      
      return *_os ;
    }
    else
      return *_ns ;
    
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /** global instance of logstream */
  logstream out ;
}
