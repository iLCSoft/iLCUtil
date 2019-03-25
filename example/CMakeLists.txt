# small example to demonstrate inter-package dependencies
# within the ILCUTIL package
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.2 FATAL_ERROR )

# this CMakeLists.txt just demonstrates how it is possible
# to use other packages of ILCUTIL in ILCUTIL itself. if
# this example were to be moved outside ILCUTIL the following
# FIND_PACKAGE commando should be used to replace the next 2
# calls to FIND_PACKAGE further below
# FIND_PACKAGE( ILCUTIL REQUIRED COMPONENTS ILCTEST streamlog )

INCLUDE( ilcsoft_default_settings )

FIND_PACKAGE( ILCTEST REQUIRED )
FIND_PACKAGE( streamlog REQUIRED )
FIND_PACKAGE( Threads REQUIRED )


INCLUDE( ilcsoft_default_enable_ctest )
INCLUDE( MacroAddILCTest )


ADD_ILCTEST( hello_world ./hello_world.cc )

ADD_DEFINITIONS( -DSTREAMLOG_LEVEL=0 )
INCLUDE_DIRECTORIES( BEFORE ${streamlog_INCLUDE_DIRS} )

ADD_EXECUTABLE( streamlog_example streamlog_example.cc  )
TARGET_LINK_LIBRARIES( streamlog_example ${streamlog_LIBRARIES} )

ADD_EXECUTABLE( streamlog_mt_example streamlog_mt_example.cc  )
TARGET_LINK_LIBRARIES( streamlog_mt_example ${streamlog_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

ADD_EXECUTABLE( streamlog_mt_file_example streamlog_mt_file_example.cc  )
TARGET_LINK_LIBRARIES( streamlog_mt_file_example ${streamlog_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )


