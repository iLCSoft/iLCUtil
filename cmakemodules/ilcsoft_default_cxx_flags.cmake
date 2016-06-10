## Set the default compiler flags for all projects picking up default ilcutil settings
## This runs checks if compilers support the flag and sets them, if they do
## this will create a humongous amount of warnings when compiling :)
SET(COMPILER_FLAGS -Wall -Wextra -Wshadow -Weffc++ -pedantic -Wno-long-long -Wuninitialized -Wl,-no-undefined  )
MESSAGE( STATUS "FLAGS ${COMPILER_FLAGS}" )
FOREACH( FLAG ${COMPILER_FLAGS} )
  CHECK_CXX_COMPILER_FLAG( "${FLAG}" CXX_FLAG_WORKS_${FLAG} )
  IF( ${CXX_FLAG_WORKS_${FLAG}} )
    MESSAGE ( STATUS "Adding ${FLAG} to CXX_FLAGS" )
    ### We prepend the flag, so they are overwritten by whatever the user sets in his own configuration
    SET ( CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS} ")
  ELSE()
    MESSAGE ( STATUS "NOT Adding ${FLAG} to CXX_FLAGS ${CXX_FLAG_WORKS}" )
  ENDIF()
ENDFOREACH()



IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

OPTION( USE_CXX11 "Use CXX Standard 2011" True )
IF( ${USE_CXX11} )
  SET( FLAG "-std=c++11" )
  CHECK_CXX_COMPILER_FLAG( ${FLAG} CXX_FLAG_WORKS_${FLAG} )
  IF( ${CXX_FLAG_WORKS_${FLAG}} )
    SET( CMAKE_CXX_FLAGS " ${FLAG} ${CMAKE_CXX_FLAGS}")
  ELSE()
    MESSAGE( FATAL "NOT Adding ${FLAG} to CXX_FLAGS ${CXX_FLAG_WORKS}" )
  ENDIF()
ENDIF()
