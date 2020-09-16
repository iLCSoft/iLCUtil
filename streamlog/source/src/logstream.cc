#include "streamlog/logstream.h"
#include "streamlog/prefix.h"

namespace streamlog{
  
  /**
   *  @brief  logconfig class.
   *  Private class managing the global logger configuration.
   *  Each time a thread is created, a thread local instance of a 
   *  logstream is created and takes it's configuration from this 
   *  global config. A mutex is used to ensure the thread safety. 
   */
  class logconfig {
    using LevelMap = logstream::LevelMap ;
  public:
    logconfig() = default ;
    ~logconfig() = default ;
    logconfig &operator=( const logconfig & ) = delete ;
    logconfig( const logconfig & ) = delete ;
    
    /** Initialize the logconfig object
     */
    void init( std::ostream *os, const std::string &name, const std::string &levelName, const LevelMap &levels ) {
      std::lock_guard <std::mutex> lock( _mutex ) ;
      _stream = os ;
      _name = name ;
      _levelName = levelName ;
      _levelMap = levels ;
    }
    
    /** Apply the logconfig data to the logstream object 
     */
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
    /// Synchronisation mutex
    std::mutex     _mutex {} ;
    /// The global shared ostream
    std::ostream  *_stream {&std::cout} ;
    /// The global logger name
    std::string    _name {"Main"} ;
    /// The global log level name
    std::string    _levelName {MESSAGE::name()} ;
    /// The global map of possible log levels
    LevelMap       _levelMap {} ;
  };
  
  logstream::logstream() {
    update_prefix() ;
  }
  
  logstream::logstream( logconfig *lcfg ) {
    update_prefix() ;
    lcfg->apply( this ) ;
  }
  
  void logstream::init( std::ostream& os , const std::string &nam ) {
    set_stream( &os ) ;
    set_name( nam ) ;
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
      return printthread {(_prefix)(),_os} ;
    }
    else {
      return printthread {} ;
    }
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
