#pragma once

#include <functional>
#include <string>

namespace streamlog {
    
  using logprefix = std::function<std::string()> ;
  
  inline static logprefix std_prefix( std::string name, std::string level ) {
    return [name=std::move(name),level=std::move(level)](){
      return "[ " + level + " \"" + name + "\"] " ;
    };
  }
}
