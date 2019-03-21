#include "streamlog/logstream.h"
#include "streamlog/loglevels.h"

namespace streamlog {

  logstream::default_logger_type &logstream::global() {
    static default_logger_type out ;
    return out ;
  }

}
