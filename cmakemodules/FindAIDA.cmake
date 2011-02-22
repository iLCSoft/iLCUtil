################################################################
# cmake module for finding AIDA
#
# requires AIDA_DIR set to a RAIDA implementation
#
# returns:
#   AIDA_FOUND        : set to TRUE or FALSE
#   AIDA_INCLUDE_DIRS : paths to AIDA includes
#   AIDA_LIBRARY_DIRS : paths to AIDA libraries
#   AIDA_LIBRARIES    : list of AIDA libraries
#
# @author Jan Engels, DESY
################################################################

SET( AIDA_FOUND FALSE )
MARK_AS_ADVANCED( AIDA_FOUND )

IF( AIDA_DIR )
    SET( RAIDA_DIR "${AIDA_DIR}" CACHE PATH "Path to RAIDA" FORCE )
ENDIF()

FIND_PACKAGE( RAIDA QUIET )

IF( RAIDA_FOUND )
    
    IF( NOT AIDA_FIND_QUIETLY )
        FIND_PACKAGE( RAIDA )
    ENDIF()

    SET( AIDA_FOUND ${RAIDA_FOUND} )
    SET( AIDA_INCLUDE_DIRS ${RAIDA_INCLUDE_DIRS} )
    SET( AIDA_LIBRARY_DIRS ${RAIDA_LIBRARY_DIRS} )
    SET( AIDA_LIBRARIES ${RAIDA_LIBRARIES} )
    SET( AIDA_DEFINITIONS ${RAIDA_DEFINITIONS} )

    MARK_AS_ADVANCED( AIDA_INCLUDE_DIRS AIDA_LIBRARY_DIRS AIDA_LIBRARIES AIDA_DEFINITIONS )

ELSE()
    IF( AIDA_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Check for AIDA -- failed" )
    ENDIF()
    IF( NOT AIDA_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDA -- not found" )
    ENDIF()
ENDIF()

