#############################################################
# cmake module for finding CondDBMySQL
#
# returns:
#   CondDBMySQL_FOUND        : set to TRUE or FALSE
#   CondDBMySQL_INCLUDE_DIRS : paths to CondDBMySQL includes
#   CondDBMySQL_LIBRARY_DIRS : paths to CondDBMySQL libraries
#   CondDBMySQL_LIBRARIES    : list of CondDBMySQL libraries
#
# @author Jan Engels, DESY
#############################################################

# -- fix for backwards compatibility
IF( NOT DEFINED CondDBMySQL_DIR AND DEFINED CondDBMySQL_HOME )
    SET( CondDBMySQL_DIR "${CondDBMySQL_HOME}" )
ENDIF( NOT DEFINED CondDBMySQL_DIR AND DEFINED CondDBMySQL_HOME )

IF( NOT CondDBMySQL_FIND_QUIETLY )
    MESSAGE( STATUS "Check for CondDBMySQL: ${CondDBMySQL_DIR}" )
ENDIF( NOT CondDBMySQL_FIND_QUIETLY )


# ---------- includes ---------------------------------------------------------
SET( CondDBMySQL_INCLUDE_DIRS CondDBMySQL_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( CondDBMySQL_INCLUDE_DIRS )

FIND_PATH( CondDBMySQL_INCLUDE_DIRS
    NAMES CondDBInterface.h
    PATHS ${CondDBMySQL_DIR}
    PATH_SUFFIXES include
    NO_DEFAULT_PATH
)


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( CondDBMySQL conddb )


# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set CondDBMySQL_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CondDBMySQL DEFAULT_MSG CondDBMySQL_INCLUDE_DIRS CondDBMySQL_LIBRARIES )


# FIXME DEPRECATED
SET( CondDBMySQL_FOUND ${CONDDBMYSQL_FOUND} )
IF( CondDBMySQL_FOUND )
    SET( CondDBMySQL_DEFINITIONS "-DUSE_CONDDB" )
    MARK_AS_ADVANCED( CondDBMySQL_DEFINITIONS )
ENDIF( CondDBMySQL_FOUND )
