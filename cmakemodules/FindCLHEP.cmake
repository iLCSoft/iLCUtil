#############################################################
# cmake module for finding CLHEP
#
# returns:
#   CLHEP_FOUND        : set to TRUE or FALSE
#   CLHEP_INCLUDE_DIRS : paths to clhep includes
#   CLHEP_LIBRARY_DIRS : paths to clhep libraries
#   CLHEP_LIBRARIES    : list of clhep libraries
#
# @author Jan Engels, DESY
#############################################################


# ---------- includes ---------------------------------------------------------
SET( CLHEP_INCLUDE_DIRS CLHEP_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( CLHEP_INCLUDE_DIRS )

FIND_PATH( CLHEP_INCLUDE_DIRS NAMES CLHEP/Vector/ThreeVector.h PATHS ${CLHEP_DIR}/include NO_DEFAULT_PATH )
IF( NOT CLHEP_DIR )
    FIND_PATH( CLHEP_INCLUDE_DIRS NAMES CLHEP/Vector/ThreeVector.h )
ENDIF()


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( CLHEP CLHEP )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set CLHEP_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CLHEP DEFAULT_MSG CLHEP_INCLUDE_DIRS CLHEP_LIBRARIES )

