########################################################
# cmake file for building ILCUTIL Package
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2 FATAL_ERROR)
########################################################


# project name
PROJECT( ILCUTIL )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 4 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )


LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakemodules" )
INCLUDE( ilcsoft_default_settings )

SET( ILCSOFT_CMAKE_MODULES_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/cmakemodules )

include( ExternalProject )


ExternalProject_Add( cmakemodules
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmakemodules"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    INSTALL_COMMAND ${CMAKE_BUILD_TOOL} install
)

ExternalProject_Add( streamlog
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/streamlog"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    INSTALL_COMMAND ${CMAKE_BUILD_TOOL} install
)

ExternalProject_Add( ilctest
    DEPENDS cmakemodules
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/ilctest"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    INSTALL_COMMAND ${CMAKE_BUILD_TOOL} install
)

ExternalProject_Add( example
    DEPENDS ilctest streamlog
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/example"
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "no install step needed"
    TEST_COMMAND ${CMAKE_BUILD_TOOL} test
)

INSTALL( DIRECTORY ./doc/ilcutil DESTINATION doc PATTERN ".svn" EXCLUDE )

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( ILCUTILConfig.cmake ILCUTILConfigVersion.cmake )

