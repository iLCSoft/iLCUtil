// -*- mode: c++;
#ifndef streamlog_h
#define streamlog_h

  /**
   *  Main header file for the streamlog library.
   *  Includes necessary header files and defines macros <br>
   *      streamlog_out( MLEVEL )  <br>
   *      streamlog_message( MLEVEL , CODE_BLOCK , OUT_MESSAGE) <br>
   *
   *  @author F. Gaede, DESY
   *  @version $Id: streamlog.h,v 1.3 2007-09-12 11:59:53 gaede Exp $
   */

#ifndef DONT_USE_STREAMLOG
#define USE_STREAMLOG
#endif

#ifdef  USE_STREAMLOG

#include <streamlog/logstream.h>
#include <streamlog/logscope.h>
#include <streamlog/loglevels.h>

#define  streamlog_logger_level( LOGGER, MLEVEL ) ( (LOGGER).would_write< streamlog::MLEVEL >() )
#define  streamlog_logger_out( LOGGER, MLEVEL ) if((LOGGER).write< streamlog::MLEVEL >() ) (LOGGER)
#define  streamlog_logger_message( LOGGER, MLEVEL , CODE_BLOCK , OUT_MESSAGE)\
   if( (LOGGER).write< streamlog::MLEVEL >() ) { \
      CODE_BLOCK \
    (LOGGER) << OUT_MESSAGE }
//for use in templated classes with gcc 3.2 use these macros:
#define  streamlog_logger_out_T( LOGGER, MLEVEL ) if((LOGGER).template write< streamlog::MLEVEL >() ) (LOGGER)
#define  streamlog_logger_message_T( LOGGER, MLEVEL , CODE_BLOCK , OUT_MESSAGE)\
   if( (LOGGER).template write< streamlog::MLEVEL >() ) { \
      CODE_BLOCK \
    (LOGGER) << OUT_MESSAGE }

// alias to global logger
#define  streamlog_level( MLEVEL ) streamlog_logger_level(logstream::global(), MLEVEL)
#define  streamlog_out( MLEVEL ) streamlog_logger_out(logstream::global(), MLEVEL)
#define  streamlog_message( MLEVEL , CODE_BLOCK , OUT_MESSAGE) streamlog_logger_message(logstream::global(), MLEVEL, CODE_BLOCK, OUT_MESSAGE)
#define  streamlog_out_T( MLEVEL ) streamlog_logger_out_T(logstream::global(), MLEVEL)
#define  streamlog_message_T( MLEVEL , CODE_BLOCK , OUT_MESSAGE) streamlog_logger_message_T(logstream::global(), MLEVEL, CODE_BLOCK, OUT_MESSAGE)

#else

#define  streamlog_out( MLEVEL ) std::cout

#define  streamlog_message( MLEVEL , CODE_BLOCK , OUT_MESSAGE)\
   if( true ) { \
      CODE_BLOCK \
      std::cout << OUT_MESSAGE }

#endif


#endif
