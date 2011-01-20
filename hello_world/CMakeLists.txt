# small example to demonstrate inter-package dependencies
# in the ILCUTIL package
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )

# if this simple example were to be moved outside ILCUTIL
# the following FIND_PACKAGE command could be used to replace
# the next 2 calls to FIND_PACKAGE 
# FIND_PACKAGE( ILCUTIL REQUIRED COMPONENTS ILCTEST streamlog )

FIND_PACKAGE( ILCTEST REQUIRED )
FIND_PACKAGE( streamlog REQUIRED )


INCLUDE( ilcsoft_default_enable_ctest )
INCLUDE( MacroAddILCTest )


ADD_ILCTEST( hello_world ./hello_world.cc )

