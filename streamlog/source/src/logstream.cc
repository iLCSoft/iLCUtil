#include "streamlog/logstream.h"
#include "streamlog/loglevels.h"

namespace streamlog {

  logstream::logger_type &logstream::global() {
    static logger_type out ;
    return out ;
  }

}
