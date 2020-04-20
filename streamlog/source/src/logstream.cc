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
    if( nullptr != &os ) {
      set_stream( &os ) ;
      set_name( name ) ;
    }
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
  
  logstream::LevelMap logstream::builtin_levels() {
    return {
      {DEBUG::name(), DEBUG::level},
      {DEBUG0::name(), DEBUG0::level},
      {DEBUG1::name(), DEBUG1::level},
      {DEBUG2::name(), DEBUG2::level},
      {DEBUG3::name(), DEBUG3::level},
      {DEBUG4::name(), DEBUG4::level},
      {DEBUG5::name(), DEBUG5::level},
      {DEBUG6::name(), DEBUG6::level},
      {DEBUG7::name(), DEBUG7::level},
      {DEBUG8::name(), DEBUG8::level},
      {DEBUG9::name(), DEBUG9::level},
      {MESSAGE::name(), MESSAGE::level},
      {MESSAGE0::name(), MESSAGE0::level},
      {MESSAGE1::name(), MESSAGE1::level},
      {MESSAGE2::name(), MESSAGE2::level},
      {MESSAGE3::name(), MESSAGE3::level},
      {MESSAGE4::name(), MESSAGE4::level},
      {MESSAGE5::name(), MESSAGE5::level},
      {MESSAGE6::name(), MESSAGE6::level},
      {MESSAGE7::name(), MESSAGE7::level},
      {MESSAGE8::name(), MESSAGE8::level},
      {MESSAGE9::name(), MESSAGE9::level},
      {WARNING::name(), WARNING::level},
      {WARNING0::name(), WARNING0::level},
      {WARNING1::name(), WARNING1::level},
      {WARNING2::name(), WARNING2::level},
      {WARNING3::name(), WARNING3::level},
      {WARNING4::name(), WARNING4::level},
      {WARNING5::name(), WARNING5::level},
      {WARNING6::name(), WARNING6::level},
      {WARNING7::name(), WARNING7::level},
      {WARNING8::name(), WARNING8::level},
      {WARNING9::name(), WARNING9::level},
      {ERROR::name(), ERROR::level},
      {ERROR0::name(), ERROR0::level},
      {ERROR1::name(), ERROR1::level},
      {ERROR2::name(), ERROR2::level},
      {ERROR3::name(), ERROR3::level},
      {ERROR4::name(), ERROR4::level},
      {ERROR5::name(), ERROR5::level},
      {ERROR6::name(), ERROR6::level},
      {ERROR7::name(), ERROR7::level},
      {ERROR8::name(), ERROR8::level},
      {ERROR9::name(), ERROR9::level},
      {SILENT::name(), SILENT::level}
    };
  }
}
