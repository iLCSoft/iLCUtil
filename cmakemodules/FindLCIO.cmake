#############################################################
# cmake module for finding LCIO
#
# FIXME this module should be DEPRECATED
#       LCIOConfig.cmake should be used instead
#
# requires:
#   ZLIB
#
# returns:
#   LCIO_FOUND        : set to TRUE or FALSE
#   LCIO_INCLUDE_DIRS : paths to lcio includes
#   LCIO_LIBRARY_DIRS : paths to lcio libraries
#   LCIO_LIBRARIES    : list of lcio libraries
#
# @author Jan Engels, DESY
#############################################################



# ---------- includes ---------------------------------------------------------
SET( LCIO_INCLUDE_DIRS LCIO_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( LCIO_INCLUDE_DIRS )

FIND_PATH( LCIO_INCLUDE_DIRS
    NAMES lcio.h
    PATHS ${LCIO_DIR}/include
    NO_DEFAULT_PATH
)
IF( NOT LCIO_DIR )
    FIND_PATH( LCIO_INCLUDE_DIRS NAMES lcio.h )
ENDIF()

IF( LCIO_INCLUDE_DIRS )
    GET_FILENAME_COMPONENT( LCIO_ROOT ${LCIO_INCLUDE_DIRS} PATH )
ENDIF()


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( LCIO lcio sio )



# ------- zlib dependency -------------
FIND_PACKAGE( ZLIB QUIET )
IF( LCIO_DIR )
    SET( LCIO_ROOT ${LCIO_DIR} ) # for MacroExportPackageDeps
ENDIF()
SET( LCIO_DEPENDS_LIBRARIES ${ZLIB_LIBRARIES} )
INCLUDE( "MacroExportPackageDeps" )
EXPORT_PACKAGE_DEPENDENCIES( LCIO ) # append ZLIB_LIBRARIES to LCIO_LIBRARIES unless FIND_PACKAGE_SKIP_DEPENDENCIES is set
# -------------------------------------



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set LCIO_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LCIO DEFAULT_MSG LCIO_INCLUDE_DIRS LCIO_LIBRARIES ${LCIO_COMPONENT_VARIABLES} ZLIB_LIBRARIES )

