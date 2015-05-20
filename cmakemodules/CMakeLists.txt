########################################################
# cmake file for installing ILCSOFT_CMAKE_MODULES
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
########################################################


PROJECT( ILCSOFT_CMAKE_MODULES )


# --------- CMakeModules version ----------------
SET( ILCSOFT_CMAKE_MODULES_VERSION_MAJOR 1 )
SET( ILCSOFT_CMAKE_MODULES_VERSION_MINOR 14 )
SET( ILCSOFT_CMAKE_MODULES_VERSION_PATCH 1 )
SET( ILCSOFT_CMAKE_MODULES_VERSION_TWEAK 0 )
SET( ILCSOFT_CMAKE_MODULES_VERSION ${ILCSOFT_CMAKE_MODULES_VERSION_MAJOR}.${ILCSOFT_CMAKE_MODULES_VERSION_MINOR}.${ILCSOFT_CMAKE_MODULES_VERSION_PATCH}.${ILCSOFT_CMAKE_MODULES_VERSION_TWEAK} )
# -----------------------------------------------


INCLUDE( ilcsoft_default_install_prefix.cmake )
INCLUDE( MacroGeneratePackageConfigFiles.cmake )

SET( ILCSOFT_CMAKE_MODULES_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/cmakemodules )


INSTALL( DIRECTORY . DESTINATION ${ILCSOFT_CMAKE_MODULES_INSTALL_PREFIX}
    PATTERN ".svn" EXCLUDE
    PATTERN "CMakeLists.txt" EXCLUDE # this file
    PATTERN "cmake" EXCLUDE # directory containing configuration files
)

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( ILCSOFT_CMAKE_MODULESConfig.cmake ILCSOFT_CMAKE_MODULESConfigVersion.cmake )

