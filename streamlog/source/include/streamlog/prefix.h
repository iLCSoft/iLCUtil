// -*- mode: c++;
#ifndef prefix_h
#define prefix_h

#include <functional>
#include <string>

namespace streamlog {
    
  using logprefix = std::function<std::string()> ;
  
  /** Generate a simple prefix function based on a logger name and log level
   */
  inline static logprefix std_prefix( std::string name, std::string level ) {
    return [name=std::move(name),level=std::move(level)](){
      return "[ " + level + " \"" + name + "\"] " ;
    };
  }
}

#endif
