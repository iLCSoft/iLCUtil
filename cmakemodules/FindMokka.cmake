########################################################################
# cmake module for finding Mokka
#
# returns:
#   Mokka_FOUND         : set to TRUE or FALSE
#   Mokka_INCLUDE_DIRS  : paths to Mokka includes
#   Mokka_LIBRARY_DIRS  : paths to Mokka libraries
#   Mokka_LIBRARIES     : list of Mokka libraries (either set to
#       libCGAPack - if found  - OR - Mokka libraries in $MOKKA/tmp )
#   Mokka_CGA_FOUND     : set to TRUE or FALSE (checks for libCGAPack)
#
# @author Jan Engels, DESY
########################################################################

SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

SET( Mokka_CGA_FOUND FALSE )
MARK_AS_ADVANCED( Mokka_CGA_FOUND )

IF( NOT Mokka_FIND_QUIETLY )
    MESSAGE( STATUS "Check for Mokka: ${Mokka_DIR}" )
ENDIF()


SET( Mokka_INCLUDE_DIRS Mokka_INCLUDE_DIRS-NOTFOUND )
SET( Mokka_LIBRARIES Mokka_LIBRARIES-NOTFOUND )
MARK_AS_ADVANCED ( Mokka_INCLUDE_DIRS Mokka_LIBRARIES )

# include dirs
IF( EXISTS "${Mokka_DIR}" )
    
    EXECUTE_PROCESS(
        COMMAND find ${Mokka_DIR}/source -name include -type d # -printf "%p;" # not recognized on mac osx
        OUTPUT_VARIABLE Mokka_INCLUDE_DIRS
        RESULT_VARIABLE _exit_code
    )
        
    IF( NOT _exit_code EQUAL 0 )
        MESSAGE( STATUS "Check for Mokka includes -- failed (cmd: find Mokka_DIR/source -name include -type d" )
        SET( Mokka_INCLUDE_DIRS Mokka_INCLUDE_DIRS-NOTFOUND )
    ELSE()
        #SEPARATE_ARGUMENTS( Mokka_INCLUDE_DIRS UNIX_COMMAND "${Mokka_INCLUDE_DIRS}" )
        SEPARATE_ARGUMENTS( Mokka_INCLUDE_DIRS )
    ENDIF()

    IF( NOT DEFINED ENV{G4SYSTEM} )
        SET( ENV{G4SYSTEM} "Linux-g++" )
    ENDIF()

    # libraries
    FIND_LIBRARY( Mokka_LIBRARIES
        NAMES CGAPack
        PATHS "${Mokka_DIR}"  # $ENV{G4LIB}/$ENV{G4SYSTEM}
        PATH_SUFFIXES lib lib/$ENV{G4SYSTEM}
    )

    IF( Mokka_LIBRARIES )
        SET( Mokka_CGA_FOUND TRUE )
        GET_FILENAME_COMPONENT( Mokka_LIBRARY_DIRS ${Mokka_LIBRARIES} PATH )
    ELSE()
        FILE( GLOB_RECURSE Mokka_LIBRARIES "${Mokka_DIR}/tmp/*.so" )
        IF( NOT Mokka_LIBRARIES )
            FILE( GLOB_RECURSE Mokka_LIBRARIES "${Mokka_DIR}/tmp/*.a" )
        ENDIF()
        IF( Mokka_LIBRARIES )
            SET( Mokka_LIBRARY_DIRS "${Mokka_DIR}/tmp" ) # FIXME loop MOKKA_LIBS using GET_FILENAME_COMPONENT
        ENDIF()
    ENDIF()

    IF( NOT Mokka_FIND_QUIETLY )
        IF( Mokka_CGA_FOUND )
            MESSAGE( STATUS "Check for Mokka libCGAPack -- ok" )
        ELSE()
            MESSAGE( STATUS "Check for Mokka libCGAPack -- failed -- did you compile Mokka with \"make MOKKA_PACK_LIBS=1\"?" )
        ENDIF()
    ENDIF()

ENDIF()


# debug
#MESSAGE( STATUS "Mokka auto-detected include dirs: ${Mokka_INCLUDE_DIRS}" )
#MESSAGE( STATUS "Mokka auto-detected libraries: ${Mokka_LIBRARIES}" )


# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set Mokka_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Mokka DEFAULT_MSG Mokka_INCLUDE_DIRS Mokka_LIBRARIES )


SET( Mokka_FOUND ${MOKKA_FOUND} )
SET( MOKKA_CGA_FOUND ${Mokka_CGA_FOUND} )

