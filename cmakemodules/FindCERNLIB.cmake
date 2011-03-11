#############################################################
# cmake module for finding CERNLIB
#
# returns:
#   CERNLIB_FOUND        : set to TRUE or FALSE
#   CERNLIB_INCLUDE_DIRS : paths to cernlib includes
#   CERNLIB_LIBRARY_DIRS : paths to cernlib libraries
#   CERNLIB_LIBRARIES    : list of cernlib libraries
#
# @author Jan Engels, DESY
#############################################################


# ---------- includes ---------------------------------------------------------
SET( CERNLIB_INCLUDE_DIRS CERNLIB_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( CERNLIB_INCLUDE_DIRS )

FIND_PATH( CERNLIB_INCLUDE_DIRS NAMES cfortran/paw.h PATHS ${CERNLIB_DIR}/include NO_DEFAULT_PATH )
IF( NOT CERNLIB_DIR )
    FIND_PATH( CERNLIB_INCLUDE_DIRS NAMES cfortran/paw.h )
ENDIF()

IF( CERNLIB_INCLUDE_DIRS )
    GET_FILENAME_COMPONENT( CERNLIB_ROOT ${CERNLIB_INCLUDE_DIRS} PATH )
ENDIF()


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( CERNLIB mathlib kernlib )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set CERNLIB_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CERNLIB DEFAULT_MSG CERNLIB_INCLUDE_DIRS CERNLIB_LIBRARIES )

