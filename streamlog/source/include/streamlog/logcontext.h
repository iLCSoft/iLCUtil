// -*- mode: c++;
#ifndef streamlog_logcontext_h
#define streamlog_logcontext_h

// -- std headers
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

// -- streamlog headers
#include <streamlog/definitions.h>

namespace streamlog {

  /**
   *  @brief  logcontext struct
   *  Hold logger metadata - mainly used by sinks on logging operation
   */
  struct logcontext {
    /// The logger name initiating the message
    std::string                 _loggerName { "UNKNOWN" } ;
    /// The log level publishing the message
    unsigned int                _level {0} ;
    /// The log level name publishing the message
    std::string                 _levelName { "VERBOSE" } ;
    /// The thread id in which the message was generated
    thread_id                   _threadId { std::this_thread::get_id() } ;
    /// The time at which the message was published
    time_point                  _time { clock::now() } ;
  };

}

#endif
