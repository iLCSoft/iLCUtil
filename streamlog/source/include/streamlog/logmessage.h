// -*- mode: c++;
#ifndef streamlog_logmessage_h
#define streamlog_logmessage_h

// -- std headers
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

namespace streamlog {

  /**
   *  @brief  logmessage struct
   *  Hold a log message and related metadata to be logged
   */
  struct logmessage {
    using time_point = std::chrono::system_clock::time_point ;
    using thread_id = std::thread::id ;

    /// The logger name initiating the message
    std::string                 _loggerName { "UNKNOWN" } ;
    /// The log level publishing the message
    unsigned int                _level {0} ;
    /// The log level name publishing the message
    std::string                 _levelName { "VERBOSE" } ;
    /// The thread id in which the message was generated
    thread_id                   _threadId { std::this_thread::get_id() } ;
    /// The time at which the message was published
    time_point                  _time { std::chrono::system_clock::now() } ;
    /// The log message
    std::stringstream           _message {} ;
  };

}

#endif
