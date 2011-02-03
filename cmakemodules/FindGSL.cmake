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


# find gsl-config
SET( GSL_CONFIG_EXECUTABLE GSL_CONFIG_EXECUTABLE-NOTFOUND )
MARK_AS_ADVANCED( GSL_CONFIG_EXECUTABLE )
FIND_PROGRAM( GSL_CONFIG_EXECUTABLE gsl-config PATHS ${GSL_DIR}/bin NO_DEFAULT_PATH )
IF( NOT GSL_DIR )
    FIND_PROGRAM( GSL_CONFIG_EXECUTABLE gsl-config )
ENDIF()

IF( GSL_CONFIG_EXECUTABLE )

    # ==============================================
    # ===          GSL_VERSION                   ===
    # ==============================================
    INCLUDE( MacroCheckPackageVersion )

    EXECUTE_PROCESS( COMMAND "${GSL_CONFIG_EXECUTABLE}" --version
        OUTPUT_VARIABLE GSL_VERSION
        RESULT_VARIABLE _exit_code
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( _exit_code EQUAL 0 )
        CHECK_PACKAGE_VERSION( GSL ${GSL_VERSION} )
    ELSE()
        SET( GSL_VERSION )
    ENDIF()

ENDIF( GSL_CONFIG_EXECUTABLE )



# ---------- includes ---------------------------------------------------------
SET( GSL_INCLUDE_DIRS GSL_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( GSL_INCLUDE_DIRS )

FIND_PATH( GSL_INCLUDE_DIRS NAMES gsl/gsl_vector.h PATHS ${GSL_DIR}/include NO_DEFAULT_PATH )
IF( NOT GSL_DIR )
    FIND_PATH( GSL_INCLUDE_DIRS NAMES gsl/gsl_vector.h )
ENDIF()


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( GSL gsl gslcblas )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set GSL_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GSL DEFAULT_MSG GSL_INCLUDE_DIRS GSL_LIBRARIES PACKAGE_VERSION_COMPATIBLE ${GSL_COMPONENT_VARIABLES} )

