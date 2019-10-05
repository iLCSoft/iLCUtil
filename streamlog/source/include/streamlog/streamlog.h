// -*- mode: c++;
#ifndef streamlog_streamlog_h
#define streamlog_streamlog_h

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
#include <streamlog/loglevels.h>

#define  streamlog_logger_level( LOGGER, MLEVEL ) ( (LOGGER).wouldWrite< streamlog::MLEVEL >() )
#define  streamlog_logger_out( LOGGER, MLEVEL ) (LOGGER).log<streamlog::MLEVEL>()
#define  streamlog_logger_message( LOGGER, MLEVEL , CODE_BLOCK , OUT_MESSAGE)\
   if( (LOGGER).wouldWrite< streamlog::MLEVEL >() ) { \
      CODE_BLOCK \
	(LOGGER).log<streamlog::MLEVEL>() << OUT_MESSAGE }
//for use in templated classes with gcc 3.2 use these macros:
#define  streamlog_logger_out_T( LOGGER, MLEVEL ) (LOGGER).template log<streamlog::MLEVEL>()
#define  streamlog_logger_message_T( LOGGER, MLEVEL , CODE_BLOCK , OUT_MESSAGE)\
   if( (LOGGER).template wouldWrite< streamlog::MLEVEL >() ) { \
      CODE_BLOCK \
	(LOGGER).log<streamlog::MLEVEL>() << OUT_MESSAGE }

// alias to global logger
#define  streamlog_level( MLEVEL ) streamlog_logger_level(streamlog::logstream::global(), MLEVEL)
#define  streamlog_out( MLEVEL ) streamlog_logger_out(streamlog::logstream::global(), MLEVEL)
#define  streamlog_message( MLEVEL , CODE_BLOCK , OUT_MESSAGE) streamlog_logger_message(streamlog::logstream::global(), MLEVEL, CODE_BLOCK, OUT_MESSAGE)
#define  streamlog_out_T( MLEVEL ) streamlog_logger_out_T(streamlog::logstream::global(), MLEVEL)
#define  streamlog_message_T( MLEVEL , CODE_BLOCK , OUT_MESSAGE) streamlog_logger_message_T(streamlog::logstream::global(), MLEVEL, CODE_BLOCK, OUT_MESSAGE)

#else

#define  streamlog_out( MLEVEL ) std::cout

#define  streamlog_message( MLEVEL , CODE_BLOCK , OUT_MESSAGE)\
   if( true ) { \
      CODE_BLOCK \
      std::cout << OUT_MESSAGE }

#endif


#endif
