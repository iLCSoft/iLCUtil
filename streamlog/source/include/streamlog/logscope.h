#pragma once

#include <streamlog/logstream.h>

namespace streamlog {

  /** Helper class to change the current scope name and log level - 
   *  if the object goes out of scope, e.g. at the end of the method
   *  where it is instantiated scope name and log level will be reset to the values 
   *  they had previously, i.e. before changed through this object.<br>
   *  Example:
   *  <pre>
   *   streamlog::logscope scope( streamlog::out, "TrackFitter", MESSAGE3::name() ) ;
   *  </pre>
   *  
   * 
   *  @author F. Gaede, DESY
   *  @author R. Ete, DESY
   *  @version $Id: logscope.h,v 1.2 2007-07-13 11:09:04 gaede Exp $
   */
  class logscope{
  public:
    logscope() = delete ;
    logscope(const logscope&) = delete ;
    logscope& operator=(const logscope&) = delete ;

    /** Instantiate a scope object for the given logstream.
     */
    logscope( logstream& ls, const std::string &name, const std::string &ln ) : 
      _logstream(ls), 
      _oldName(ls.name()),
      _oldLevel(ls.levelName()) {
      _logstream.set_name( name ) ;
      _logstream.set_level( ln ) ;
    }
    
    /** Reset old name and level if set through this object.
     */
    ~logscope() {
      release() ;
    }
    
    /** Release the log scope, switch back to the old log scope
     */
    void release() {
      if( not _released ) {
        _logstream.set_name( _oldName ) ;
        _logstream.set_level( _oldLevel ) ;
        _released = true ;
      }
    }
    
  protected:
    /// The logstream on which to change the log scope
    logstream               &_logstream ;
    /// The old logstream name (cache)
    const std::string        _oldName ;
    /// The old logstream log level (cache)
    const std::string        _oldLevel ;
    /// Whether the log scope has been released
    bool                     _released {false} ;
  };
  
}