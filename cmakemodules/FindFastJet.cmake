#########################################################
# cmake module for finding FastJet
#
#
# returns:
#   FastJet_FOUND        : set to TRUE or FALSE
#   FastJet_VERSION      : package version
#   FastJet_INCLUDE_DIRS : paths to gsl includes
#   FastJet_LIBRARY_DIRS : paths to gsl libraries
#   FastJet_LIBRARIES    : list of gsl libraries
#
# @author Jan Engels, DESY
#########################################################

# find fastjet-config
SET( FastJet_CONFIG_EXECUTABLE FastJet_CONFIG_EXECUTABLE-NOTFOUND )
MARK_AS_ADVANCED( FastJet_CONFIG_EXECUTABLE )
FIND_PROGRAM( FastJet_CONFIG_EXECUTABLE fastjet-config PATHS ${FastJet_DIR}/bin NO_DEFAULT_PATH )
IF( NOT FastJet_DIR )
    FIND_PROGRAM( FastJet_CONFIG_EXECUTABLE fastjet-config )
ENDIF()

IF( FastJet_CONFIG_EXECUTABLE )

    # ==============================================
    # ===          FastJet_PREFIX                ===
    # ==============================================

    EXECUTE_PROCESS( COMMAND "${FastJet_CONFIG_EXECUTABLE}" --prefix
        OUTPUT_VARIABLE FastJet_ROOT
        RESULT_VARIABLE _exit_code
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( NOT _exit_code EQUAL 0 )
        SET( FastJet_ROOT )
    ENDIF()

    # ==============================================
    # ===          FastJet_VERSION               ===
    # ==============================================
    INCLUDE( MacroCheckPackageVersion )

    EXECUTE_PROCESS( COMMAND "${FastJet_CONFIG_EXECUTABLE}" --version
        OUTPUT_VARIABLE FastJet_VERSION
        RESULT_VARIABLE _exit_code
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( _exit_code EQUAL 0 )
        CHECK_PACKAGE_VERSION( FastJet ${FastJet_VERSION} )
    ELSE()
        SET( FastJet_VERSION )
    ENDIF()

ENDIF( FastJet_CONFIG_EXECUTABLE )


# ---------- includes ---------------------------------------------------------
SET( FastJet_INCLUDE_DIRS FastJet_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( FastJet_INCLUDE_DIRS )

FIND_PATH( FastJet_INCLUDE_DIRS NAMES FjPseudoJet.hh PATHS ${FastJet_DIR}/include NO_DEFAULT_PATH )
IF( NOT FastJet_DIR )
    FIND_PATH( FastJet_INCLUDE_DIRS NAMES FjPseudoJet.hh )
ENDIF()



# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( FastJet fastjet )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set FASTJET_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( FastJet DEFAULT_MSG FastJet_INCLUDE_DIRS FastJet_LIBRARIES PACKAGE_VERSION_COMPATIBLE )

SET( FastJet_FOUND ${FASTJET_FOUND} )

