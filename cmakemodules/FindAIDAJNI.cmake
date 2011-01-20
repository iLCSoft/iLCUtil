#######################################################################
# cmake module for finding AIDAJNI
#
# requires: (if auto-detection fails)
#   JAVA_HOME
#   JAIDA_HOME
#
# returns:
#   AIDAJNI_FOUND        : set to TRUE or FALSE
#   AIDAJNI_INCLUDE_DIRS : paths to AIDAJNI includes
#   AIDAJNI_LIBRARY_DIRS : paths to AIDAJNI libraries
#   AIDAJNI_LIBRARIES    : list of AIDAJNI libraries
#
# @author Jan Engels, DESY
#######################################################################

SET( AIDAJNI_FOUND FALSE )
MARK_AS_ADVANCED( AIDAJNI_FOUND )

# -- fix for backwards compatibility
IF( NOT DEFINED AIDAJNI_DIR AND DEFINED AIDAJNI_HOME )
    SET( AIDAJNI_DIR "${AIDAJNI_HOME}" )
ENDIF()

IF( NOT AIDAJNI_FIND_QUIETLY )
    MESSAGE( STATUS "Check for AIDAJNI: ${AIDAJNI_DIR}" )
ENDIF()

# Java is required for AIDAJNI
INCLUDE( FindJAVA )
IF( NOT JAVA_FOUND )
    IF( NOT AIDAJNI_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDAJNI -- failed to find Java" )
    ENDIF()
ELSE()
    # write JAVA_HOME to cache
    SET( JAVA_HOME "${JAVA_HOME}" CACHE PATH "Path to JAVA" FORCE )

    # find libjvm.so
    SET( JAVA_LIB_JVM JAVA_LIB_JVM-NOTFOUND )
    MARK_AS_ADVANCED( JAVA_LIB_JVM )
    FIND_LIBRARY( JAVA_LIB_JVM
        NAMES jvm
        PATHS ${JAVA_HOME}
        PATH_SUFFIXES jre/lib/i386/client jre/lib/amd64/client
        NO_DEFAULT_PATH )

    IF( NOT JAVA_LIB_JVM AND NOT AIDAJNI_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDAJNI jvm library -- failed" )
    ENDIF()

    # find libverify.so
    SET( JAVA_LIB_VERIFY JAVA_LIB_VERIFY-NOTFOUND )
    MARK_AS_ADVANCED( JAVA_LIB_VERIFY )
    FIND_LIBRARY( JAVA_LIB_VERIFY
        NAMES verify
        PATHS ${JAVA_HOME}
        PATH_SUFFIXES jre/lib/i386 jre/lib/amd64
        NO_DEFAULT_PATH )

    IF( NOT JAVA_LIB_VERIFY AND NOT AIDAJNI_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDAJNI verify library -- failed" )
    ENDIF()
ENDIF()

# JAIDA is required for AIDAJNI
IF( NOT AIDAJNI_FIND_QUIETLY )
    MESSAGE( STATUS "Check for JAIDA: ${JAIDA_HOME}" )
ENDIF()
IF( NOT DEFINED JAIDA_HOME AND NOT DEFINED ENV{JAIDA_HOME} )
    IF( NOT AIDAJNI_FIND_QUIETLY )
        MESSAGE( STATUS "Check for JAIDA_HOME -- not set -- use: cmake -DJAIDA_HOME=/path/to/JAIDA" )
    ENDIF()
ELSE()
    # if only env var is defined
    IF( NOT DEFINED JAIDA_HOME )
        SET( JAIDA_HOME "$ENV{JAIDA_HOME}" )
    ENDIF()
    # write JAIDA_HOME to cache
    SET( JAIDA_HOME "${JAIDA_HOME}" CACHE PATH "Path to JAIDA" FORCE )
    FILE( GLOB JAIDA_JARS "${JAIDA_HOME}/lib/*.jar" )
    IF( NOT AIDAJNI_FIND_QUIETLY )
        IF( NOT JAIDA_JARS )
            MESSAGE( STATUS "Check for JAIDA -- jars not found!!" )
        ELSE()
            MESSAGE( STATUS "Check for JAIDA -- works" )
        ENDIF()
    ENDIF()
ENDIF()

# include dirs
SET( AIDAJNI_INCLUDE_DIRS AIDAJNI_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( AIDAJNI_INCLUDE_DIRS )

FIND_PATH( AIDAJNI_INCLUDE_DIRS
    NAMES AIDA/AIDA.h
    PATHS ${AIDAJNI_HOME}
    PATH_SUFFIXES include
    NO_DEFAULT_PATH )

IF( NOT AIDAJNI_INCLUDE_DIRS AND NOT AIDAJNI_FIND_QUIETLY )
    MESSAGE( STATUS "Check for AIDAJNI includes -- failed" )
ENDIF()

# new library (since version 3.2.6)
SET( AIDAJNI_LIB AIDAJNI_LIB-NOTFOUND )
# FIXME FIND_LIBRARY does not accept a regex to match the library name (freehep-aidajni-[0-9]+\.[0-9]+\.[0-9]*)
SET( AIDAJNI_LIB_VERSIONS
    freehep-aidajni-3.2.6 freehep-aidajni-3.2.7 freehep-aidajni-3.2.8
    freehep-aidajni-3.3.0 freehep-aidajni-3.3.1 freehep-aidajni-3.3.2
    freehep-aidajni-3.3.3 freehep-aidajni-3.3.4 freehep-aidajni-3.3.5
)
IF( UNIX AND NOT APPLE )
    SET( AIDAJNI_LIB_PATH_SUFFIXES
        Linux-g++
        i386-Linux-g++
        i686-Linux-g++
        amd64-Linux-g++
    )
ELSE()
    IF( APPLE )
        SET( AIDAJNI_LIB_PATH_SUFFIXES
            i386-MacOSX-g++
            ppc-MacOSX-g++
        )
    ELSE()
        SET( AIDAJNI_LIB_PATH_SUFFIXES
            sparc-SunOS-CC
            x86-Windows-msvc
        )
    ENDIF()
ENDIF()
MARK_AS_ADVANCED( AIDAJNI_LIB AIDAJNI_LIB_VERSIONS AIDAJNI_LIB_PATH_SUFFIXES )

FIND_LIBRARY( AIDAJNI_LIB
    NAMES ${AIDAJNI_LIB_VERSIONS}
    PATHS ${AIDAJNI_HOME}/lib
    PATH_SUFFIXES ${AIDAJNI_LIB_PATH_SUFFIXES}
    NO_DEFAULT_PATH )
# END OF FIXME

# old libraries (before version 3.2.6)
SET( AIDAJNI_LIB_NAMES AIDAJNI FHJNI )
SET( AIDAJNI_FINDLIB_FAILED FALSE )
SET( AIDAJNI_LIBRARY_DIRS )
MARK_AS_ADVANCED( AIDAJNI_LIBRARY_DIRS AIDAJNI_LIB_NAMES AIDAJNI_FINDLIB_FAILED )

IF( NOT AIDAJNI_LIB )
    
    FOREACH( libname ${AIDAJNI_LIB_NAMES} )

        SET( AIDAJNI_LIB_${libname} AIDAJNI_LIB_${libname}-NOTFOUND )
        MARK_AS_ADVANCED( AIDAJNI_LIB_${libname} )
        
        FIND_LIBRARY( AIDAJNI_LIB_${libname}
            NAMES ${libname}
            PATHS ${AIDAJNI_HOME}/lib
            PATH_SUFFIXES ${AIDAJNI_LIB_PATH_SUFFIXES}
            NO_DEFAULT_PATH )
        
        IF( NOT AIDAJNI_LIB_${libname} )
            SET( AIDAJNI_FINDLIB_FAILED TRUE )
            IF( NOT AIDAJNI_FIND_QUIETLY )
                MESSAGE( STATUS "Check for AIDAJNI ${libname} library -- failed" )
            ENDIF()
        ELSE()
            LIST( APPEND AIDAJNI_LIBS ${AIDAJNI_LIB_${libname}} )
            GET_FILENAME_COMPONENT( _aux ${AIDAJNI_LIB_${libname}} PATH )
            LIST( APPEND AIDAJNI_LIBRARY_DIRS ${_aux} )
        ENDIF()
    ENDFOREACH()
ELSE()
    SET( AIDAJNI_LIBS ${AIDAJNI_LIB} )
ENDIF()

# freehep-aidajni.jar
SET( AIDAJNI_JAR AIDAJNI_JAR-NOTFOUND )
MARK_AS_ADVANCED( AIDAJNI_JAR )
FIND_FILE( AIDAJNI_JAR NAMES
        freehep-aidajni.jar
        freehep-aidajni-3.2.6.jar
    PATHS ${AIDAJNI_HOME}
    PATH_SUFFIXES lib
    NO_DEFAULT_PATH )

IF( NOT AIDAJNI_JAR AND NOT AIDAJNI_FIND_QUIETLY )
    MESSAGE( STATUS "Check for AIDAJNI freehep-aidajni.jar -- failed" )
ENDIF()

SET( AIDAJNI_SH AIDAJNI_SH-NOTFOUND )
MARK_AS_ADVANCED( AIDAJNI_SH )

# find shell
IF( UNIX )
    FIND_PROGRAM( SH
      sh
      ${CYGWIN_INSTALL_PATH}/bin
      /bin
      /usr/bin
      /usr/local/bin
      /sbin
      # FIXME add path to MacOS here
    )
    MARK_AS_ADVANCED( SH )
    IF( SH )
        SET( AIDAJNI_SH "${SH}" )
    ENDIF()
ENDIF()

# set variables and display results
IF( AIDAJNI_INCLUDE_DIRS AND NOT AIDAJNI_FINDLIB_FAILED AND AIDAJNI_JAR AND
    AIDAJNI_SH AND JAIDA_JARS AND JAVA_LIB_JVM AND JAVA_LIB_VERIFY )
    SET( AIDAJNI_FOUND TRUE )
    SET( AIDAJNI_LIBRARIES ${AIDAJNI_LIBS} ${JAVA_LIB_JVM} ${JAVA_LIB_VERIFY} )
    MARK_AS_ADVANCED( AIDAJNI_LIBRARIES )

    # FIXME deprecated
    SET( AIDAJNI_DEFINITIONS "-DUSE_AIDA -DMARLIN_USE_AIDA -DUSE_JAIDA" )
    MARK_AS_ADVANCED( AIDAJNI_DEFINITIONS )

    # write aidajni-setup.sh
    IF( CYGWIN )
        SET( CLASSPATH_SEP ";" )
    ELSE()
        SET( CLASSPATH_SEP ":" )
    ENDIF()
    IF( UNIX )
        FILE( WRITE "${CMAKE_BINARY_DIR}/aidajni-setup.sh"
            "export CLASSPATH=\"${AIDAJNI_JAR}${CLASSPATH_SEP}\${CLASSPATH}\"\n" )
        FOREACH( jar ${JAIDA_JARS} )
            FILE( APPEND "${CMAKE_BINARY_DIR}/aidajni-setup.sh"
                "export CLASSPATH=\"${jar}${CLASSPATH_SEP}\${CLASSPATH}\"\n" )
        ENDFOREACH()
        # install the env script to bin
        INSTALL( FILES "${CMAKE_BINARY_DIR}/aidajni-setup.sh" DESTINATION bin )
        IF( NOT AIDAJNI_FIND_QUIETLY )
            MESSAGE( STATUS "Generating aidajni-setup.sh environment script..." )
            MESSAGE( STATUS "===============================================================================" )
            MESSAGE( STATUS "Be sure to source one of the following files BEFORE you run your ${PROJECT_NAME} binary!\n"
                            "-- ${CMAKE_INSTALL_PREFIX}/bin/aidajni-setup.sh\n"
                            "-- ${CMAKE_BINARY_DIR}/aidajni-setup.sh" )
            MESSAGE( STATUS "===============================================================================" )
        ENDIF()
    ENDIF()
    IF( WIN32 )
        #FIXME add code for generating aidajni-setup.bat (WIN32)
    ENDIF()

    IF( NOT AIDAJNI_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDAJNI -- works" )
    ENDIF()
ELSE()
    IF( AIDAJNI_FIND_REQUIRED )
        MESSAGE( FATAL_ERROR "Check for AIDAJNI -- failed" )
    ENDIF()
    IF( NOT AIDAJNI_FIND_QUIETLY )
        MESSAGE( STATUS "Check for AIDAJNI -- failed -- skip AIDAJNI package..." )
    ENDIF()
ENDIF()
