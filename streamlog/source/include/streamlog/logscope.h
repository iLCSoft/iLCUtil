#ifndef logscope_h
#define logscope_h

#include "streamlog/logstream.h"

namespace streamlog{

  /** Helper class to change the current scope name and log level - 
   *  if the object goes out of scope, e.g. at the end of the method
   *  where it is instantiated scope name and log level will be reset to the values 
   *  they had previously, i.e. before changed through this object.<br>
   *  Example:
   *  <pre>
   *   streamlog::logscope scope( streamlog::out ) ;
   *   scope.setName( "TrackFitter" ) ;
   *   scope.setLevel< streamlog::MESSAGE3 >() ;
   *  </pre>
   *  
   * 
   *  @author F. Gaede, DESY
   *  @version $Id: logscope.h,v 1.2 2007-07-13 11:09:04 gaede Exp $
   */
  class logscope{

  public:
    logscope() = delete ;
    logscope(const logscope&) = delete ;
    logscope& operator=(const logscope&) = delete ;

    /** Instantiate a scope object for the given logstream.
     */
    logscope(logstream& ls) : _ls(&ls) , _name("") , _level(-1) { /* nop */ }
    
    /** Reset old name and level if set through this object.
     */
    ~logscope() {
      if( _name.size() > 0 ) {
        _ls->set_name( _name ) ;
      }
      if(  _level > -1 )
 	    _ls->set_level( _level ) ; 
    }
    
    /** Change current log scope name for the lifetime of this object */
    void setName(const std::string &name) {
      _name =  _ls->name() ;
      _ls->set_name( name ) ;
    }
  

    /** Change current log level for the lifetime of this object */
    template <class T>
    void setLevel(){
      _level =  _ls->level() ;
      _ls->set_level( T::level )  ;
    }

    /** Change current log level for the lifetime of this object 
     *  through a string level name that has been registered with the logstream
     *  via logstream::addLevelName - otherwise the call will have no effect.
     */
    void setLevel(const std::string& level){
      _level =  _ls->set_level( level )  ;
    }
   
  protected: 
    logstream      *_ls = nullptr ;
    std::string     _name {} ;
    int             _level {-1} ;
  };
  
}
#endif
