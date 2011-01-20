########################################################
# cmake file for building ILCUTIL Package
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
########################################################


# project name
PROJECT( ILCUTIL )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )


SET( ILCSOFT_CMAKE_MODULES_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/cmakemodules )
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakemodules" )
INCLUDE( ilcsoft_default_settings )


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
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_MODULE_PATH=${ILCSOFT_CMAKE_MODULES_INSTALL_PREFIX}
    INSTALL_COMMAND ${CMAKE_BUILD_TOOL} install
)

ExternalProject_Add( hello_world
    DEPENDS ilctest streamlog
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/hello_world"
    CMAKE_ARGS -DCMAKE_FIND_ROOT_PATH=${CMAKE_INSTALL_PREFIX} -DCMAKE_MODULE_PATH=${ILCSOFT_CMAKE_MODULES_INSTALL_PREFIX}
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "no install step needed"
    TEST_COMMAND ${CMAKE_BUILD_TOOL} test
)


# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( ILCUTILConfig.cmake ILCUTILConfigVersion.cmake )

