#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "streamlog/streamlog.h"

#include "fibonacci.h"
#include "ATemplate.h"


// global pointer to logstream 
static std::ostream* logger = nullptr ;



void simple_task(const std::string name, unsigned n ){

  // init the logger for this stream
  streamlog::out.init( *logger, name ) ;


  streamlog_out( DEBUG ) << " in simple task : " << name << std::endl ;


  streamlog_out( MESSAGE ) << " will compute << " << n << " sums of the first "
			   << n << " fibonacci numbers ... " << std::endl ;

  for( unsigned i=0 ; i<n ; ++i){

    streamlog_test::fibonacci fibo(i) ;


    // change the scope name for logging
    std::stringstream s ;
    s << name << "_loop_" << i ;
    streamlog::logscope scope(streamlog::out) ;
    scope.setName( s.str() ) ;


    streamlog_out( MESSAGE5 ) << " sum of first " << i << " fibonacci numbers : " << std:: endl 
			      << "    ->   "    << fibo.sum()
			      << std::endl ;

  }
  
  streamlog_out( MESSAGE ) << " done in task : " << name << std::endl ;

}


/** Write out some log messages from four threads.
 *  Either using std::cout or - if filename given 
 *  - a global log-file.
 */
int main(int argc, char** argv) {

  std::ofstream* file = nullptr ;

    
  std::string binname( argv[0] ) ;
  binname = binname.substr( binname.find_last_of("/") + 1 , binname.size() ) ;

  if( argc > 1 ) {

    file = new std::ofstream(  argv[1]  )  ;

    logger = file ;
    
    streamlog::out.init( *file , binname ) ;

    std::cout << "  writing log to file : " << argv[1] << std::endl ;

  }
  else{

    logger = &std::cout ;
    streamlog::out.init( std::cout , binname) ;
  }
 
  
  streamlog_out( DEBUG ) << " this debug message will be printed only if " <<  std::endl  
		   << " the current log level is larger or equal to DEBUG::level and DEBUG::active==true" 
		   <<  std::endl ;
  

  std::thread t1(  simple_task, "task1", 10 );
  std::thread t2(  simple_task, "task2", 10 );
  std::thread t3(  simple_task, "task3", 10 );
  std::thread t4(  simple_task, "task4", 10 );


  t1.join() ;
  t2.join() ;
  t3.join() ;
  t4.join() ;


  
  if(file)
    delete file ;

}
