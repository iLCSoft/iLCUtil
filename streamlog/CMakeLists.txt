########################################################
# cmake file for building Marlin example Package
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( streamlog )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 4 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

INCLUDE( ilcsoft_default_settings )

FIND_PACKAGE( Threads REQUIRED )

### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

IF( INSTALL_DOC )
    ADD_SUBDIRECTORY( ./doc )
ELSE()
    MESSAGE( STATUS "INSTALL_DOC set to OFF" )
ENDIF()


### LIBRARY #################################################################

# include directories
INCLUDE_DIRECTORIES( BEFORE ./source/include )
INSTALL( DIRECTORY ./source/include DESTINATION . PATTERN ".svn" EXCLUDE )

# add library
AUX_SOURCE_DIRECTORY( ./source/src library_sources )
ADD_SHARED_LIBRARY( ${PROJECT_NAME} ${library_sources} )
INSTALL( TARGETS ${PROJECT_NAME} DESTINATION lib )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC Threads::Threads )

ADD_SUBDIRECTORY( ./examples )

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( streamlogConfig.cmake streamlogConfigVersion.cmake )

