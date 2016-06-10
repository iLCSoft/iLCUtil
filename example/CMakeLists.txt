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


INCLUDE( ilcsoft_default_enable_ctest )
INCLUDE( MacroAddILCTest )


ADD_ILCTEST( hello_world ./hello_world.cc )

