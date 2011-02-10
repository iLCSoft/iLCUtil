#########################################################
# cmake module for finding Fortran library
#
#
# returns:
#   FORTRAN_COMPILER_NAME   : fortran compiler name, e.g. g77, gfortran
#   FORTRAN_LIBRARY_NAME    : fortran library name, e.g. g2c, gfortran
#   FORTRAN_LIBRARY         : fortran library, e.g. /usr/lib/libgfortran.so
#
# @author Jan Engels, DESY
#########################################################

# gfortran / gfortran-32
SET( FORTRAN_LIBRARY_NAME "gfortran" )

# g77
IF( CMAKE_Fortran_COMPILER )
    GET_FILENAME_COMPONENT( FORTRAN_COMPILER_NAME "${CMAKE_Fortran_COMPILER}" NAME_WE )
    IF( FORTRAN_COMPILER_NAME STREQUAL "g77" )
        SET( FORTRAN_LIBRARY_NAME "g2c" )
    ENDIF()
ENDIF()

IF( CMAKE_COMPILER_IS_GNUCC )
    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE )
    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_VARIABLE GCC_MACHINE OUTPUT_STRIP_TRAILING_WHITESPACE )
ENDIF()

# ------- needed for 32bit compatibility mode  ------------------
IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # 64bit platform
    IF( BUILD_32BIT_COMPATIBLE ) # 32bit compatibility mode
        LIST( APPEND FORTRAN_LIBRARY_SEARCH_PATH
            /usr/lib/gcc/${GCC_MACHINE}/${GCC_VERSION}/32
        )
    ENDIF()
ELSE() # 32bit platform or 64bit with CXXFLAGS="-m32"
    LIST( APPEND FORTRAN_LIBRARY_SEARCH_PATH
        /usr/lib/gcc/${GCC_MACHINE}/${GCC_VERSION}/32
    )
ENDIF()
# ---------------------------------------------------------------

LIST( APPEND FORTRAN_LIBRARY_SEARCH_PATH
    /usr/lib/gcc/${GCC_MACHINE}/${GCC_VERSION}
    #/usr/lib64
    #/usr/lib
)

IF( APPLE )
    LIST( INSERT FORTRAN_LIBRARY_SEARCH_PATH 0 /usr/osxws/lib )
ENDIF( APPLE )


SET( FORTRAN_LIBRARY FORTRAN_LIBRARY-NOTFOUND )
MARK_AS_ADVANCED( FORTRAN_LIBRARY )
FIND_LIBRARY( FORTRAN_LIBRARY NAMES ${FORTRAN_LIBRARY_NAME} PATHS ${FORTRAN_LIBRARY_SEARCH_PATH} NO_DEFAULT_PATH )
# if previous find fails, try to find in system paths..
FIND_LIBRARY( FORTRAN_LIBRARY NAMES ${FORTRAN_LIBRARY_NAME} )


# debug infos
IF( NOT FORTRAN_LIBRARY_FIND_QUIETLY )
    MESSAGE( STATUS "GCC_VERSION (${GCC_VERSION})" )
    MESSAGE( STATUS "GCC_MACHINE (${GCC_MACHINE})" )
    MESSAGE( STATUS "FORTRAN_COMPILER_NAME (${FORTRAN_COMPILER_NAME})" )
    MESSAGE( STATUS "FORTRAN_LIBRARY_NAME (${FORTRAN_LIBRARY_NAME})" )
    MESSAGE( STATUS "FORTRAN_LIBRARY_SEARCH_PATH: ${FORTRAN_LIBRARY_SEARCH_PATH}" )
    MESSAGE( STATUS "FORTRAN_LIBRARY: ${FORTRAN_LIBRARY}" )
ENDIF()



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set FORTRAN_LIBRARY_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( FORTRAN_LIBRARY DEFAULT_MSG FORTRAN_LIBRARY )

