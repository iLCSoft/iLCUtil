// -*- mode: c++;
#ifndef streamlog_definitions_h
#define streamlog_definitions_h

/**
 *  The variable STREAMLOG_LOGGING_TS can be tweaked before including this header:
 *  @code
 *  #define STREAMLOG_LOGGING_TS
 *  #include <streamlog/definitions.h>
 *  // do some stuff with it
 *  @endcode
 *  or while using the build system with -STREAMLOG_LOGGING_TS .
 *  This enable or disable thread safety of logging messages
 */
namespace streamlog {
  
  /**
   *  @brief  nullmutex class
   *  Standard Mutex interface without actual lock
   */
  class nullmutex {
  public:
    nullmutex() = default ;
    ~nullmutex() = default ;
    nullmutex(nullmutex &&) = delete ;
    nullmutex(const nullmutex&) = delete ;
    
  public:
    void lock () {}
    void unlock() {}
    void try_lock() {}
  };
  
#ifdef STREAMLOG_LOGGING_TS // logging thread safety
#include <mutex>
  using mutex = std::mutex ;
#else 
  using mutex = nullmutex ;
#endif

}

#endif
