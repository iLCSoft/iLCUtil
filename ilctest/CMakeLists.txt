########################################################
# cmake file for building ILCTEST Package
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( ILCTEST )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )



### DEPENDENCIES ############################################################

# FindILCSOFT_CMAKE_MODULES.cmake needs to be in CMAKE_MODULE_PATH
FIND_PACKAGE( ILCSOFT_CMAKE_MODULES 1.11 REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


INSTALL_DIRECTORY( ./include DESTINATION . FILES_MATCHING PATTERN "*.h" )


# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( ILCTESTConfig.cmake ILCTESTConfigVersion.cmake )

