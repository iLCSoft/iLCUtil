################################################################
# cmake module for finding AIDA
#
# requires AIDA_DIR set to a RAIDA or JAIDA implementation
#
# returns:
#   AIDA_FOUND        : set to TRUE or FALSE
#   AIDA_INCLUDE_DIRS : paths to RAIDA/AIDAJNI includes
#   AIDA_LIBRARY_DIRS : paths to RAIDA/AIDAJNI libraries
#   AIDA_LIBRARIES    : list of RAIDA/AIDAJNI libraries
#
# @author Jan Engels, DESY
################################################################

SET( AIDA_FOUND FALSE )
MARK_AS_ADVANCED( AIDA_FOUND )

# -- fix for backwards compatibility
IF( NOT DEFINED AIDA_DIR AND DEFINED AIDA_HOME )
    SET( AIDA_DIR "${AIDA_HOME}" )
ENDIF()


# first try finding RAIDA
SET( RAIDA_DIR "${AIDA_DIR}" )
FIND_PACKAGE( RAIDA QUIET )

IF( RAIDA_FOUND )
    SET( RAIDA_DIR ${AIDA_DIR} CACHE PATH "Path to RAIDA" FORCE )
    IF( NOT AIDA_FIND_QUIETLY )
        FIND_PACKAGE( RAIDA )
    ENDIF()
    SET( AIDA_FOUND TRUE )
    SET( AIDA_INCLUDE_DIRS ${RAIDA_INCLUDE_DIRS} )
    SET( AIDA_LIBRARY_DIRS ${RAIDA_LIBRARY_DIRS} )
    SET( AIDA_LIBRARIES ${RAIDA_LIBRARIES} )
    SET( AIDA_DEFINITIONS ${RAIDA_DEFINITIONS} )
ENDIF()


# if RAIDA not found try finding JAIDA
IF( NOT AIDA_FOUND )
    
    SET( AIDAJNI_DIR "${AIDA_DIR}" )
    FIND_PACKAGE( AIDAJNI QUIET )

    IF( AIDAJNI_FOUND )
        SET( AIDAJNI_DIR ${AIDA_DIR} CACHE PATH "Path to AIDAJNI" FORCE )
        IF( NOT AIDA_FIND_QUIETLY )
            FIND_PACKAGE( AIDAJNI )
        ENDIF()
        SET( AIDA_FOUND TRUE )
        SET( AIDA_INCLUDE_DIRS ${AIDAJNI_INCLUDE_DIRS} )
        SET( AIDA_LIBRARY_DIRS ${AIDAJNI_LIBRARY_DIRS} )
        SET( AIDA_LIBRARIES ${AIDAJNI_LIBRARIES} )
        SET( AIDA_DEFINITIONS ${AIDAJNI_DEFINITIONS} )
    ENDIF()
ENDIF()
MARK_AS_ADVANCED( AIDA_INCLUDE_DIRS AIDA_LIBRARY_DIRS AIDA_LIBRARIES AIDA_DEFINITIONS )

IF( AIDA_FOUND )
    IF( NOT AIDA_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDA: ${AIDA_DIR} -- ok" )
    ENDIF()
ELSE()
    IF( AIDA_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Check for AIDA -- failed" )
    ENDIF()
    IF( NOT AIDA_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDA -- not found" )
    ENDIF()
ENDIF()
