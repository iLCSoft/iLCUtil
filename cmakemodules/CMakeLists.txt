########################################################
# cmake file for installing ILCSOFT_CMAKE_MODULES
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
########################################################

INSTALL( DIRECTORY . DESTINATION cmakemodules
    PATTERN ".svn" EXCLUDE
    PATTERN "CMakeLists.txt" EXCLUDE # this file
)

