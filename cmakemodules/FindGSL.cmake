#########################################################
# cmake module for finding GSL
#
#
# returns:
#   GSL_FOUND        : set to TRUE or FALSE
#   GSL_INCLUDE_DIRS : paths to gsl includes
#   GSL_LIBRARY_DIRS : paths to gsl libraries
#   GSL_LIBRARIES    : list of gsl libraries
#
# @author Jan Engels, DESY
#########################################################

# -- fix for backwards compatibility
IF( NOT DEFINED GSL_DIR AND DEFINED GSL_HOME )
    SET( GSL_DIR "${GSL_HOME}" )
ENDIF( NOT DEFINED GSL_DIR AND DEFINED GSL_HOME )


# ---------- includes ---------------------------------------------------------
SET( GSL_INCLUDE_DIRS GSL_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( GSL_INCLUDE_DIRS )

FIND_PATH( GSL_INCLUDE_DIRS NAMES gsl/gsl_vector.h PATHS ${GSL_DIR}/include NO_DEFAULT_PATH )
FIND_PATH( GSL_INCLUDE_DIRS NAMES gsl/gsl_vector.h )



# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( GSL gsl gslcblas )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set GSL_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GSL DEFAULT_MSG GSL_INCLUDE_DIRS GSL_LIBRARIES )

