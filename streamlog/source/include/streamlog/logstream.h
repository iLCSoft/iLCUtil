// -*- mode: c++;
#ifndef logstream_h
#define logstream_h

#include "streamlog/prefix.h"
#include "streamlog/loglevels.h"

#include <iostream>
#include <map>
#include <mutex>
#include <sstream>

namespace streamlog{

  class prefix_base ;
  class logbuffer ;
  class logscope ;
  
  class logconfig ;

  /** Thread safe helper class that collects streamed data 
   *  and sends it to the actual ostream on deletion.
   *  Idea taken from https://stackoverflow.com/questions/14718124
   *  @author F,Gaede, DESY
   *  @date Apr 2020
   */
  class printthread: public std::stringstream {
  protected:
    std::string _pref{};
    std::ostream* _o = nullptr ;
  public:
    printthread(const printthread &) = delete ;
    printthread &operator=(const printthread &) = delete ;
    /// returns an invalid stream
    printthread() : printthread::basic_ios( 0 ) {} ;
    /// initialize w/ prefix and final ostream
    printthread(std::string prefix,std::ostream* o):_pref(prefix),_o(o){};
    /// copy c'tor copies stringbugf from local stringstream
    printthread( printthread&& rhs ){
      printthread::basic_ostream<char>( rhs.rdbuf() ) ;
    }
    /// on deletion we actually write to the output
    ~printthread(){
      if( nullptr == _o ) {
        return ;
      }
      // prepend the prefix to every newline before dumping to ostream
      std::ostringstream oss;
      for (std::string line; std::getline(*this, line, '\n');) {
	       oss << _pref << line << '\n';
      }
      std::lock_guard<std::mutex> guard(_mutexPrint);
      *_o << oss.str() ;
    }
  private:
    static std::mutex _mutexPrint;
  };


  /** Class defining a log stream that is used to print log messages depending 
   *  on current log level. Can be initialized with any std::ostream, typically either 
   *  std::cout or an std::ofstream file handler.
   *  There is one global (thread local) instance of this class defined in the 
   *  library: logstream::out  <br>
   *  Typically only this instance is needed, e.g.: <br>
   *  <pre>
   *    // in int main() :
   *    streamlog::global_init( std::cout, argv[0], streamlog::MESSAGE::name() ) ;
   * 
   *    //...
   *    
   *    if( streamlog::out.write< streamlog::DEBUG1 >() )
   *       streamlog::out() << " this message will only be printed if level >= DEBUG1::level " 
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
   *  the behaviour of the logstream, i.e. the current log level and log scope name can be changed only
   *  through an object of streamlog::logscope.
   * 
   *  @see logstream::write() 
   *  @see logscope
   *
   *  @author F. Gaede, DESY
   *  @author R. Ete, DESY (2020)
   *  @version $Id: logstream.h,v 1.3 2007-08-08 13:08:34 gaede Exp $
   */
  class logstream {

    friend class logscope ;
    friend class logconfig ;

    typedef std::map< std::string,  unsigned > LevelMap ;

  public :

    logstream() ; 
    logstream( logconfig *lcfg ) ;
    logstream(const logstream&) = delete ;
    logstream& operator=(const logstream&) = delete ;
    ~logstream() = default ;
    
    /** Initialize the global configuration.
     *  This sets the global logger settings (stream, name, current level, levels map) 
     *  that are shared by all thread local instances. On new thread creation, the global 
     *  thread local logger instance receives the global configuration in its constructor.
     */
    static void global_init( std::ostream *os, const std::string &name, 
      const std::string &levelName, const LevelMap &lm = streamlog::log_levels_map() ) ;
    
    /** Get the map of builtin log levels
     */
    static LevelMap builtin_levels() ;
    
    /** Set the stream for this logger
     */
    void set_stream( std::ostream *os ) {
      if( nullptr != os ) {
        _os = os ;
      }
    }
    
    /** Set the logger name
     */
    void set_name( const std::string &n ) {
      _name = n ;
      update_prefix() ;
    }

    /** Initialize the logstream with an std::ostream, e.g. std::cout and 
     *  the main scope name, e.g. argv[0].
     *  Only first call to this method has an effect, subsequent calls are ignored.
     * 
     */
    void init( std::ostream& os , const std::string &name ) ;

   /** True if next log message of the current level (class T ) will be written, i.e.
     *  the next call to std::ostream& operator()() will return a valid outstream.
     * 
     */
    template <class T>
    inline bool write() {
      // dont' call chek_level if T::active == false
      return ( T::active && check_level<T>() ) ;
    }

    /** True if next log message of the current level (class T ) would be written
     *  - can be used to conditionally execute code blocks that are needed before writing to
     * the outstream.
     * 
     */
    template <class T>
    inline bool would_write() {
      return ( T::active && T::level >= _level ) ;
    }

   /** Return the actual std::ostream where the message should be writen to - will return
     *  a nullstream unless prepended by a successfull call to write<MESSAGELEVEL>()
     */
    printthread operator()() ;

    /** Adds a level name to the stream for setting the log level with a string through 
     *  a scope class. Only names added with this method will have an effect - other log 
     *  levels can only be activated with logstream::logscope::setLevel<T>().
     *  Useful for framework programs where the log level can be changed via a steering 
     *  parameter.
     *  
     *  @see logstream::logscope::setLevel(const std::string levelName )
     */
    template <class T>
    void addLevelName() {
      _map[ T::name() ] = T::level ;
    }
    
    /** Get the logger name
     */
    const std::string &name() const {
      return _name ;
    }
    
    /** Get the logger name
     */
    unsigned int level() const {
      return _level ;
    }
    
  protected:
    
    template <class T>
    void set_level() {
      set_level( T::name() ) ;
    }
    
    /** Set the current level - user need to use a streamlog::logscope object 
     *  to do this.
     */
    void set_level( unsigned int level ) { 
      _level = level ;
    } 

    /** Set the current level through its name - only level previously made known to
     *  the stream through addLevelName will have an effect.
     */
    unsigned int set_level( const std::string& levelName ) ; 

    /** Get the log prefix formatter
     */
    logprefix &prefix() { 
      return _prefix ; 
    }

    /** used internally by write<T> */
    template<class T>
    bool check_level() {
      if( T::level >= _level ) {
	      _active = true ;
        _levelName = T::name() ;
        update_prefix() ;
      }
      return _active ;
    }

    /** Used internally to update the prefix before writing
     */
    void update_prefix() {
      _prefix = std_prefix( _name, _levelName ) ;
    }

  private:
    /// Wrapper for actual ostream
    std::ostream   *_os = {&std::cout} ;
    /// The logger name
    std::string     _name {"Main"} ;
    /// The log level name
    std::string     _levelName {MESSAGE::name()} ;
    /// current log level
    unsigned int    _level {MESSAGE::level} ;    
    /// boolean helper 
    bool            _active {false} ;      
    /// prefix formatter
    logprefix       _prefix {} ;
    /// string map of level names
    LevelMap        _map {};
  };

  /// The thread local global instance of a logstream object
  extern thread_local logstream out ;

}
#endif
