#include <streamlog/logstream.h>

#include <thread>

namespace streamlog {
  
  class logconfig {
    using LevelMap = logstream::LevelMap ;
  public:
    logconfig() = default ;
    ~logconfig() = default ;
    logconfig &operator=( const logconfig & ) = delete ;
    logconfig( const logconfig & ) = delete ;
    
    void init( std::ostream *os, const std::string &name, const std::string &levelName, const LevelMap &levels ) {
      std::lock_guard <std::mutex> lock( _mutex ) ;
      _stream = os ;
      _name = name ;
      _levelName = levelName ;
      _levelMap = levels ;
    }
    
    void apply( logstream *ls ) {
      std::lock_guard <std::mutex> lock( _mutex ) ;
      ls->set_stream( _stream ) ;
      ls->set_name( _name ) ;
      ls->set_level( _levelName ) ;
      if( not _levelMap.empty() ) {
        ls->_map = _levelMap ;
      }
    }
    
  private:
    std::mutex     _mutex {} ;
    std::ostream  *_stream {&std::cout} ;
    std::string    _name {"Main"} ;
    std::string    _levelName {MESSAGE::name()} ;
    LevelMap       _levelMap {} ;
  };
  
  logstream::logstream( logconfig *cfg ) {
    update_prefix() ;
    cfg->apply( this ) ;
  }
   
  logstream::logstream() {
    update_prefix() ;
  }
  
  void logstream::init( std::ostream& os , const std::string &name ) {
    set_stream( &os ) ;
    set_name( name ) ;
  }

  unsigned logstream::set_level( const std::string& levelName ) {
    unsigned l = _level ;
    LevelMap::iterator it = _map.find( levelName ) ;
    if( it != _map.end() ) {
      _level = it->second ;
      _levelName = levelName ;
      update_prefix() ;
    }
    return l ;
  } 

  printthread logstream::operator()() { 
    
    if( _active && _os ) {
      
      _active = false ;
      
      return printthread{(_prefix)(),_os} ;
    }
    else
     return printthread{} ;
  }
  
  // static global logger config, protected by mutex
  // shared by thread local global logger instances
  static logconfig __logconfig__ ;

  /** thread global instance of logstream */
  thread_local logstream out( &__logconfig__ ) ;
    
  /// global mutex for actual io
  std::mutex printthread::_mutexPrint{};
  
  void logstream::global_init( std::ostream *os, const std::string &name, const std::string &levelName, const LevelMap &lm ) {
    __logconfig__.init( os, name, levelName, lm ) ;
    __logconfig__.apply( &out ) ;
  }
  
}
