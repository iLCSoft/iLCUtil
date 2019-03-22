# streamlog

*streamlog* is a simple c++ logging library.

## Basic use

The library provides utility macros for easy logging:

```cpp
#include <streamlog/streamlog.h>
// log in the global logger (console)
streamlog_out(DEBUG)   << "This is a message with DEBUG level " << std::endl;
streamlog_out(MESSAGE) << "This is a message with MESSAGE level " << std::endl;
streamlog_out(WARNING) << "This is a message with WARNING level " << std::endl;
streamlog_out(ERROR)   << "This is a message with ERROR level " << std::endl;
```

The global logger instance can be configured (usually on startup):

```cpp
#include <streamlog/streamlog.h>
auto &logger = logstream::global() ;
logger.setName( "MyApplication" ) ;
logger.setLevel<MESSAGE>() ;
```

## Creating logger objects

Logger objects can be created using the class logstream:

```cpp
#include <streamlog/streamlog.h>
// standard console output
auto stdConsole = logstream::createLogger( "mylogger" ) ;
// colored console output
auto colConsole = logstream::createLogger( "mylogger", logstream::coloredConsole() ) ;
// logfile output
auto fileLogger = logstream::createLogger( "mylogger", logstream::simpleFile( "logfile.log" ) ) ;
// logfile and colored console output
auto mixLogger = logstream::createLogger( "mylogger", {
  logstream::simpleFile( "logfile2.log" ),
  logstream::coloredConsole()
}) ;
```

In some case you might want to shared the sinks (console, file, etc...) among multiple logger instances:

```cpp
#include <streamlog/streamlog.h>
// the shared sinks
auto sinks = {
  logstream::simpleFile( "logfile.log" ),
  logstream::coloredConsole()
} ;
auto trkLogger = logstream::createLogger( "Tracking" , sinks ) ;
auto pfaLogger = logstream::createLogger( "ParticleFlow", sinks ) ;
```

By default, the logger thread safety is enable/disable at compile time.
If you work in a single threaded environment, you might want to switch off the internal lock providing thread safety to gain a bit of speed.

To get a thread safe logger (streamlog::mt):

```cpp
#include <streamlog/streamlog.h>
// a thread safe console logger
auto logger = logstream::createLogger<streamlog::mt>( "mylogger" , logstream::coloredConsole() ) ;
```

To get a non thread safe logger (streamlog::st):

```cpp
#include <streamlog/streamlog.h>
// a non thread safe console logger
auto logger = logstream::createLogger<streamlog::st>( "mylogger" , logstream::coloredConsole() ) ;
```
## Logger API

Here after an example usage of how to create and use a logger:

```cpp
#include <streamlog/streamlog.h>
// a thread safe colored console logger + log file
auto sinks = {
  logstream::coloredConsole<streamlog::mt>(),
  logstream::simpleFile<streamlog::mt>( "logfile.log" )
};
auto logger = logstream::createLogger<streamlog::mt>( "Application" , sinks ) ;
// set the log level
logger->setLevel<MESSAGE>() ;
// log some random messages
// messages will appear as: [ LEVEL "Application" ] message
logger->log<DEBUG>() << "This message won't appear ..." << std::endl ;
logger->log<MESSAGE>() << "Hello world !" << std::endl ;
logger->log<WARNING>() << "WE ARE UNDER ATTACK !!!" << std::endl ;
logger->log<ERROR>() << "The program brain.exe crashed. Do you want to restart it ?" << std::endl ;
```

It is possible to change to log message prefix using the standard_formatter class:

```cpp
#include <streamlog/streamlog.h>
auto logger = logstream::createLogger<streamlog::mt>( "Application" ) ;
logger->setLevel<MESSAGE>() ;

// print: [ MESSAGE "Application" ] Hello world !
logger->log<MESSAGE>() << "Hello world !" << std::endl ;

// create a new formatter
auto formatter = streamlog::make_unique<standard_formatter>() ;
formatter->setOption( standard_formatter::print_option::time, true ) ;
logger->setFormatter( formatter ) ; // formatter is moved here

// print: [10:45:12] [ MESSAGE "Application" ] Hello world !
logger->log<MESSAGE>() << "Hello world !" << std::endl ;

formatter = streamlog::make_unique<standard_formatter>() ;
formatter->setOption( standard_formatter::print_option::time, true ) ;
formatter->setOption( standard_formatter::print_option::thread, true ) ;
formatter->setOption( standard_formatter::print_option::logger, false ) ;
logger->setFormatter( formatter ) ; // formatter is moved here

// print: [10:45:12] [thread 1] [ MESSAGE ] Hello world !
logger->log<MESSAGE>() << "Hello world !" << std::endl ;
```

## Possible logger sinks

### Standard console:

Log message in std::cout.

```cpp
#include <streamlog/streamlog.h>
auto sink = logstream::console() ;
```

### Colored console:

Log message in std::cout with fancy colors.

```cpp
#include <streamlog/streamlog.h>
auto sink = logstream::coloredConsole() ;
```

### Log file:

Log message in a file.

```cpp
#include <streamlog/streamlog.h>
auto sink = logstream::simpleFile( "filename.log", std::ios_base::out ) ;
```

### Thread log file:

Log messages in a file dedicated to the caller thread, meaning that each thread in the program has its own log file but uses the same sink and logger. The purpose of this sink is to help for debugging multi-threaded programs by looking at the log of each thread separately.

```cpp
#include <streamlog/streamlog.h>
// In a program with 2 threads (main + one more) this will
// open two log files called filename_1.log and filename_2.log
// and each thread logging in this sink will write its message
// in its own file
auto sink = logstream::threadFile( "filename", ".log", std::ios_base::out ) ;
```
