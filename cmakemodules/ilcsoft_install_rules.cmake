

include( ExternalProject )


# ----- read download urls from external file --------------------------------
# download urls are generated using the variable desy_svn_repository and the
# versions of the individual packages.
include( "ilcsoft_download_urls" )
# ----------------------------------------------------------------------------


# ----- read dependencies from external file ---------------------------------
include( "ilcsoft_dependencies" )
# ----------------------------------------------------------------------------


# ----- macro for setting common external project arguments-------------------
include( MacroExtractNumericVersion )
# ----------------------------------------------------------------------------




# ----- generate script for reconfiguring a package --------------------------
set( ilcsoft_pkg_reconfigure_script "${PROJECT_BINARY_DIR}/reconfigure-pkg.sh" )
file( WRITE ${ilcsoft_pkg_reconfigure_script} "f=$(ls -d ../tmp/*.txt)\n" )
file( APPEND ${ilcsoft_pkg_reconfigure_script} "test -z \"$f\" && { echo \"need to cd into build dir first!\" ; exit 0 ; }\n" )
file( APPEND ${ilcsoft_pkg_reconfigure_script} "echo ; cat $f | tr ';' ' ' | sed 's/-GUnix Makefiles //' | sed 's/%/\\\;/g'\n" )
file( APPEND ${ilcsoft_pkg_reconfigure_script} "echo -e \"\\ncopy-paste cmd into command line and afterwards call 'eval \\$cmd'\\n\"\n" )
execute_process( COMMAND chmod 755 ${ilcsoft_pkg_reconfigure_script} )




# ----- generate script for reconfiguring ilcsoft ----------------------------
set( ilcsoft_reconfigure_script "${PROJECT_BINARY_DIR}/reconfigure-ilcsoft.sh" )
file( WRITE ${ilcsoft_reconfigure_script} "read -p\"reconfigure EVERYTHING FROM SCRATCH?\ [y/n] \" r\n" )
file( APPEND ${ilcsoft_reconfigure_script} "test \"\${r:0:1}\" != \"y\" && exit 0\n" )
file( APPEND ${ilcsoft_reconfigure_script} "for i in ${PROJECT_BINARY_DIR}/*-*-prefix; do test -d $i/build && pushd $i/build && make clean uninstall && popd ; done\n" )
file( APPEND ${ilcsoft_reconfigure_script} "find ${PROJECT_BINARY_DIR}/*-*-prefix -name CMakeCache.txt -exec rm -v {} \;\n" )
file( APPEND ${ilcsoft_reconfigure_script} "echo -e \"\\ntype 'make' to recompile ilcsoft\\n\"\n" )
execute_process( COMMAND chmod 755 ${ilcsoft_reconfigure_script} )




# ----- generate script to initialize environment ----------------------------
set( ilcsoft_env_init_script "${ilcsoft_install_prefix}/init_ilcsoft.sh" )
file( WRITE ${ilcsoft_env_init_script} "\# iLCSoft environment initialization script\n" )
file( APPEND ${ilcsoft_env_init_script} "\# Warning: any changes made to this file will get lost the next time cmake is called !!\n\n\n" ) 
file( APPEND ${ilcsoft_env_init_script} "export CMAKE_PREFIX_PATH=${ilcsoft_install_prefix}:${external_install_prefix}:$CMAKE_PREFIX_PATH\n\n" )
file( APPEND ${ilcsoft_env_init_script} "export PATH=${ilcsoft_install_prefix}/bin:${external_install_prefix}/bin:$PATH\n" )
file( APPEND ${ilcsoft_env_init_script} "export LD_LIBRARY_PATH=${ilcsoft_install_prefix}/lib:${external_install_prefix}/lib:$LD_LIBRARY_PATH\n\n\n" )
# ----------------------------------------------------------------------------




# make sure there is a '/' at the end of ilcsoft_sources_prefix
if( ilcsoft_sources_prefix )
    get_filename_component( ilcsoft_sources_prefix "${ilcsoft_sources_prefix}" ABSOLUTE )
    set( ilcsoft_sources_prefix "${ilcsoft_sources_prefix}/" )
endif()



# ----- helper macro to set common external project arguments ----------------
macro( SET_PKG_EP_ARGS _pkg_name _pkg_version )

    string( TOLOWER ${_pkg_name} _lpkg_name )

    EXTRACT_NUMERIC_VERSION( "${_pkg_version}" )

    if( NUMERIC_VERSION )
        set( _ep_base ${_lpkg_name}-${NUMERIC_VERSION}-prefix )
    else()
        set( _ep_base ${_lpkg_name}-${_pkg_version}-prefix )
    endif()

    # replace slashes with underscores
    string( REPLACE "/" "_" _ep_base "${_ep_base}" )

    # external project arguments passed to an ilcsoft package
    set( ${_lpkg_name}_ep_args ${${_lpkg_name}_ep_args}
        LIST_SEPARATOR % # needed for CMAKE_PREFIX_PATH
        SOURCE_DIR ${ilcsoft_sources_prefix}${_ep_base}/src
        BINARY_DIR ${_ep_base}/build
        DOWNLOAD_DIR ${_ep_base}/download
        STAMP_DIR ${_ep_base}/stamp
        TMP_DIR ${_ep_base}/tmp
        #INSTALL_DIR ${_ep_base}/install
        INSTALL_DIR ${ilcsoft_install_prefix}
    )

endmacro()
# ----------------------------------------------------------------------------




# ----- wrapper macro for cmake find_package commando ------------------------
# tries to find a package with a given version. If not found it creates
# a dummy custom_target needed to handle the dependencies between the
# external project calls for the different packages
macro( FIND_PACKAGE_WRAPPER_DUMMY_TARGET )

    set( _pkg_name ${ARGV0} )
    set( _pkg_version ${ARGV1} )
    set( _args ${ARGN} )

    # ALL targets and version variables must be lower-case !!
    string( TOLOWER ${_pkg_name} _lpkg_name )
    string( TOUPPER ${_pkg_name} _upkg_name )

    # if install variable not defined set to "NO"
    if( NOT DEFINED install_${_lpkg_name} )
        set( _pkg_install_flag "NO" )
    else()
        # convenience variable for readability
        set( _pkg_install_flag "${install_${_lpkg_name}}" )
    endif()

    if( NOT "${_pkg_install_flag}" STREQUAL "YES" ) # in case it is set to "AUTO" or "NO"

        if( _pkg_version )
            if( "${_pkg_version}" STREQUAL "trunk" OR
                "${_pkg_version}" STREQUAL "HEAD" OR
                "${_pkg_version}" MATCHES "branches/.*" # e.g. "branches/v01-01-pre"
            )
                
                if( "${_pkg_install_flag}" STREQUAL "AUTO" )
                    set( _pkg_install_flag "YES" )
                else()
                    # remove invalid version string from args for FIND_PACKAGE
                    string( REPLACE "${_pkg_version}" "" _args "${_args}" )
                    string( REPLACE "EXACT" "" _args "${_args}" )
                endif()

            else()

                EXTRACT_NUMERIC_VERSION( "${_pkg_version}" )

                if( NUMERIC_VERSION ) # numeric version found
                    #message( "ARGS BEFORE NUMERIC CONVERSION: ${_args}" )
                    # replace version in _args with numeric version, i.e. v01-00 -> 1.0
                    # this is needed to call FIND_PACKAGE with a version argument
                    string( REPLACE "${_pkg_version}" "${NUMERIC_VERSION}" _args "${_args}" )
                    #message( "ARGS AFTER NUMERIC CONVERSION: ${_args}" )
                else()
                    if( "${_pkg_install_flag}" STREQUAL "AUTO" )
                        set( _pkg_install_flag "YES" )
                    else()
                        # remove invalid version string from args for FIND_PACKAGE
                        string( REPLACE "${_pkg_version}" "" _args "${_args}" )
                        string( REPLACE "EXACT" "" _args "${_args}" )
                    endif()
                endif()
            endif()
        endif()

        if( NOT "${_pkg_install_flag}" STREQUAL "YES" )

            message( STATUS "looking for ${_lpkg_name} (${_pkg_version})" )

            find_package( ${_args} QUIET )

            if( ${_pkg_name}_FOUND OR ${_upkg_name}_FOUND )
                find_package( ${_args} ) # repeat find_package without QUIET arg to show debug infos
                message( STATUS "found ${_lpkg_name} (${_pkg_version})" )
            else()
                if( "${_pkg_install_flag}" STREQUAL "AUTO" OR "${_pkg_install_flag}" STREQUAL "" )
                    message( STATUS "could not find ${_lpkg_name} (${_pkg_version})" )
                    set( _pkg_install_flag "YES" ) # if the package was not found it should be installed
                else()
                    message( "Warning: ${_lpkg_name} package disabled!" )
                    set( _pkg_install_flag "SKIP" ) # unless it is set to NO
                endif()
            endif()
        endif()
    endif()

    if( "${_pkg_install_flag}" STREQUAL "YES" )
        if( NOT DEFINED ${_lpkg_name}_version )
                message( FATAL_ERROR "${_lpkg_name}_version not defined" )
        endif()
        message( "++ install ${_lpkg_name} (${${_lpkg_name}_version}) from source" )
    else()

        # create a dummy-target in order to keep the dependencies from ExternalProject's DEPENDS variable happy
        add_custom_command( OUTPUT "/${_lpkg_name}-done" COMMAND ${CMAKE_COMMAND} -E echo "/${_lpkg_name}-done" )
        add_custom_target( ${_lpkg_name} COMMAND ${CMAKE_COMMAND} -E echo "${_lpkg_name} already installed" )
        if( DEFINED ${_lpkg_name}_depends )
            add_dependencies( ${_lpkg_name} ${${_lpkg_name}_depends} )
        endif()

        if( ${_pkg_name}_ROOT )
            file( APPEND ${ilcsoft_env_init_script} "\n\n\#---------------------- ${_upkg_name} -----------------------------------\n" )
            file( APPEND ${ilcsoft_env_init_script} "export ${_upkg_name}=${${_pkg_name}_ROOT}\n" )
            file( APPEND ${ilcsoft_env_init_script} "export PATH=$${_upkg_name}/bin:$PATH\n" )
            file( APPEND ${ilcsoft_env_init_script} "export LD_LIBRARY_PATH=$${_upkg_name}/lib:$LD_LIBRARY_PATH\n" )
        endif()
        # ---------------------------------------------------------------------------------------------------------------------

    endif()

    set( install_${_lpkg_name} "${_pkg_install_flag}" )

endmacro()
# ----------------------------------------------------------------------------




# ----- gather compiler infos ------------------------------------------------
execute_process( COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_VARIABLE GCC_MACHINE OUTPUT_STRIP_TRAILING_WHITESPACE )
# 4.1.2 --> 41
# 3.4.6 --> 34
string( REPLACE "." ";" _aux "${GCC_VERSION}" )
list( LENGTH _aux _len )
while( _len GREATER 2 )
    list( REMOVE_AT _aux -1 )
    list( LENGTH _aux _len ) # update length
endwhile()
string( REPLACE ";" "" GCC_VERSION "${_aux}" )

# x86_64-redhat-linux --> x86_64
# i386-redhat-linux --> i386
string( REPLACE "-" ";" _aux "${GCC_MACHINE}" )
list( GET _aux 0 GCC_MACHINE )
# ----------------------------------------------------------------------------




# ============================================================================
# ==================== EXTERNAL PACKAGES =====================================
# ============================================================================

# external packages usually need special treatment and therefore custom
# installation rules on a package-per-package basis


# ----------------------------------------------------------------------------
# --- CERNLIB
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( CERNLIB ) # no versioning supported by cernlib

if( "${install_cernlib}" STREQUAL "YES" )

    if( NOT "${cernlib_version}" STREQUAL "2006" )
        message( SEND_ERROR "cernlib install support currently only given for version 2006" )
        set( install_cernlib "NO" )
    endif()

    if( NOT "${GCC_VERSION}" STREQUAL "34" AND
        NOT "${GCC_VERSION}" STREQUAL "41" )
        message( SEND_ERROR "cernlib install support currently only given for gcc34 or gcc41" )
        set( install_cernlib "NO" )
    endif()

    if( NOT "${GCC_MACHINE}" STREQUAL "i386" AND
        NOT "${GCC_MACHINE}" STREQUAL "x86_64" )
        message( SEND_ERROR "cernlib install support currently only given for i386 or x86_64" )
        set( install_cernlib "NO" )
    endif()


    #message( "GCC_VERSION ${GCC_VERSION}" )
    #message( "GCC_MACHINE ${GCC_MACHINE}" )

    if( "${install_cernlib}" STREQUAL "YES" )

        ExternalProject_Add( cernlib
            URL "http://www.desy.de/~engels/cernlib/cernlib-${cernlib_version}-${GCC_MACHINE}-gcc${GCC_VERSION}.tgz"
            BUILD_IN_SOURCE 1
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "no configure needed"
            BUILD_COMMAND ${CMAKE_COMMAND} -E echo "no build needed"
            #INSTALL_COMMAND mkdir -p ${external_install_prefix} && tar --strip-components=1 -xzf ../cernlib-${cernlib_version}-${GCC_MACHINE}-gcc${GCC_VERSION}.tgz -C ${external_install_prefix}
            INSTALL_COMMAND mkdir -p ${external_install_prefix} && tar --strip-components=1 -xzf ../cernlib-${cernlib_version}-${GCC_MACHINE}-gcc${GCC_VERSION}.tgz -C ${external_install_prefix}
        )
    else()
        message( SEND_ERROR "please set install_cernlib to NO or set CMAKE_PREFIX_PATH or CERNLIB_DIR to your own cernlib installation" )
    endif()
endif()




# ----------------------------------------------------------------------------
# --- MySQL
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( MySQL ${mysql_version} ) #EXACT )

if( "${install_mysql}" STREQUAL "YES" )

    if( NOT "${mysql_version}" STREQUAL "5.0.45" )
        message( SEND_ERROR "mysql install support currently only given for version 5.0.45" )
        set( install_mysql "NO" )
    endif()

    if( NOT "${GCC_VERSION}" STREQUAL "34" AND
        NOT "${GCC_VERSION}" STREQUAL "41" )
        message( SEND_ERROR "mysql install support currently only given for gcc34 or gcc41" )
        set( install_mysql "NO" )
    endif()

    if( NOT "${GCC_MACHINE}" STREQUAL "i386" AND
        NOT "${GCC_MACHINE}" STREQUAL "x86_64" )
        message( SEND_ERROR "mysql install support curreently only given for i386 or x86_64" )
        set( install_mysql "NO" )
    endif()


    if( "${install_mysql}" STREQUAL "YES" )
        ExternalProject_Add( mysql
            URL "http://www.desy.de/~engels/mysql/mysql-${mysql_version}-${GCC_MACHINE}-gcc${GCC_VERSION}.tgz"
            BUILD_IN_SOURCE 1
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "no configure needed"
            BUILD_COMMAND ${CMAKE_COMMAND} -E echo "no build needed"
            INSTALL_COMMAND mkdir -p ${external_install_prefix} && tar --strip-components=1 -xzf ../mysql-${mysql_version}-${GCC_MACHINE}-gcc${GCC_VERSION}.tgz -C ${external_install_prefix}
        )
    else()
        message( SEND_ERROR "please set install_mysql to NO or set CMAKE_PREFIX_PATH or MySQL_DIR to your own mysql installation" )
    endif()
endif()



# ----------------------------------------------------------------------------
# --- GLUT
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( GLUT ${glut_version} )

if( "${install_glut}" STREQUAL "YES" )
    ExternalProject_Add( glut
        URL "http://ignum.dl.sourceforge.net/project/freeglut/freeglut/freeglut-${glut_version}.tar.gz"
        CONFIGURE_COMMAND ./configure --prefix=${external_install_prefix}
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make install
    )
endif()



# ----------------------------------------------------------------------------
# --- GSL
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( GSL ${gsl_version} EXACT )

if( "${install_gsl}" STREQUAL "YES" )
    ExternalProject_Add( gsl
        #${common_ep_args}
        URL "ftp://ftp.gnu.org/gnu/gsl/gsl-${gsl_version}.tar.gz"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./configure --prefix=${external_install_prefix}
    )
    # this variables have to be set in order to compile ROOT from source
    set( GSL_INCLUDE_DIRS "${external_install_prefix}/include" )
    set( GSL_LIBRARY_DIRS "${external_install_prefix}/lib" )
endif()



# ----------------------------------------------------------------------------
# --- CLHEP
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( CLHEP ${clhep_version} EXACT )

if( "${install_clhep}" STREQUAL "YES" )
    ExternalProject_Add( clhep
        #${common_ep_args}
        URL "http://proj-clhep.web.cern.ch/proj-clhep/DISTRIBUTION/distributions/clhep-${clhep_version}.tgz"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./CLHEP/configure --prefix=${external_install_prefix}
    )
endif()



# ----------------------------------------------------------------------------
# --- ROOT
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( ROOT ${root_version} EXACT )

if( "${install_root}" STREQUAL "YES" )
    ExternalProject_Add( root
        #${common_ep_args}
        DEPENDS ${root_depends}
        URL "ftp://root.cern.ch/root/root_v${root_version}.source.tar.gz"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./configure --fail-on-missing --enable-explicitlink --enable-gdml --enable-gsl-shared --enable-minuit2 --enable-roofit --enable-soversion --enable-table --enable-unuran --enable-xrootd --with-gsl-incdir=${GSL_INCLUDE_DIRS} --with-gsl-libdir=${GSL_LIBRARY_DIRS}
        INSTALL_COMMAND ROOTSYS=${external_install_prefix} make install
    )

    file( APPEND ${ilcsoft_env_init_script} "export ROOTSYS=${external_install_prefix}\n" )
else()
    file( APPEND ${ilcsoft_env_init_script} "export ROOTSYS=$ROOT\n" )
endif()




# ----------------------------------------------------------------------------
# --- FastJet
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( FastJet ${fastjet_version} EXACT )

set( fastjet_configure_command ./configure --prefix=${external_install_prefix} --enable-shared )
if(NOT APPLE)
    # on mac osx --enable-allplugins fails with error:
    # libtool: compile: unrecognized option `-O2'
    list( APPEND fastjet_configure_command --enable-allplugins )
endif()

if( "${install_fastjet}" STREQUAL "YES" )
    ExternalProject_Add( fastjet
        #${common_ep_args}
        URL "http://www.lpthe.jussieu.fr/~salam/fastjet/repo/fastjet-${fastjet_version}.tar.gz"
        BUILD_IN_SOURCE 1
        #CONFIGURE_COMMAND ./configure --prefix=${external_install_prefix} --enable-shared #--enable-allplugins 
        CONFIGURE_COMMAND ${fastjet_configure_command}
    )
endif()



# ----------------------------------------------------------------------------
# --- QT4
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( Qt4 ${qt4_version} EXACT )

if( "${install_qt4}" STREQUAL "YES" )

    # ------ setting of qt4 download url ----------
    if( "${qt4_version}" VERSION_LESS "4.6" )
        set( qt4_url "ftp://ftp.trolltech.com/qt/source/qt-x11-opensource-src-${qt4_version}.tar.gz" )
        if( APPLE )
            set( qt4_url "ftp://ftp.trolltech.com/qt/source/qt-mac-opensource-src-${qt4_version}.tar.gz" )
        endif()
    else()
        set( qt4_url "ftp://ftp.trolltech.com/qt/source/qt-everywhere-opensource-src-${qt4_version}.tar.gz" )
    endif()

    # ------ setting of qt4 configure options ----------
    set( qt4_cfg_options -prefix-install -fast -make libs -no-separate-debug-info -no-qt3support )

    if( NOT APPLE )
        list( APPEND qt4_cfg_options -no-xkb -no-xinerama )
    endif()
    if( "${qt4_version}" VERSION_LESS "4.5" )
        list( APPEND qt4_cfg_options -no-tablet )
    endif()
    if( "${qt4_version}" VERSION_GREATER "4.3" )
        list( APPEND qt4_cfg_options -no-webkit )
    endif()
    if( "${qt4_version}" VERSION_GREATER "4.5" )
        list( APPEND qt4_cfg_options -opensource )
    endif()

    ExternalProject_Add( qt4
        #${common_ep_args}
        URL "${qt4_url}"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND echo yes | ./configure -prefix ${external_install_prefix} ${qt4_cfg_options}
    )
else()
    # set QTDIR from qmake in PATH
    #file( APPEND ${ilcsoft_env_init_script} "\n\n\#---------------------- ${_upkg_name} -----------------------------------\n" )
    #file( APPEND ${ilcsoft_env_init_script} "export ${_upkg_name}=${external_install_prefix}\n" )
    #file( APPEND ${ilcsoft_env_init_script} "export PATH=$${_upkg_name}/bin:$PATH\n" )
    #file( APPEND ${ilcsoft_env_init_script} "export LD_LIBRARY_PATH=$${_upkg_name}/lib:$LD_LIBRARY_PATH\n" )
endif()




# ============================================================================
# ===================== ILCSOFT PACKAGES =====================================
# ============================================================================


macro( ADD_ILCSOFT_PACKAGE _pkg_name )

    string( TOLOWER ${_pkg_name} _lpkg_name )

    SET_PKG_EP_ARGS( ${_pkg_name} ${${_lpkg_name}_version} )

    if( "${install_${_lpkg_name}}" STREQUAL "YES" )

        if( DEFINED ${_lpkg_name}_depends )
            foreach( _dep ${${_lpkg_name}_depends} )
                if( "${install_${_dep}}" STREQUAL "SKIP" )
                    message( "Warning: dependency from ${_lpkg_name} could not be found or installed: ${_dep}" )
                endif()
            endforeach()
        endif()


        ExternalProject_Add( ${_lpkg_name}
            DEPENDS ${${_lpkg_name}_depends}
            ${common_ep_args}
            ${${_lpkg_name}_ep_args}
            ${${_lpkg_name}_download_url}
            CMAKE_ARGS ${common_cmake_args} ${${_lpkg_name}_cmake_args}
        )

    endif()

    file( GLOB _aux ${PROJECT_BINARY_DIR}/${_lpkg_name}-*-prefix )
    list( LENGTH _aux _lenaux )
    if( ${_lenaux} GREATER 1 )
        message( "Warning: more than one ${_lpkg_name}-prefix directory found in: ${PROJECT_BINARY_DIR}" )
    endif()

endmacro()


macro( ADD_ILCSOFT_CORE_PACKAGE _pkg_name )

    string( TOLOWER ${_pkg_name} _lpkg_name )

    FIND_PACKAGE_WRAPPER_DUMMY_TARGET( ${_pkg_name} ${${_lpkg_name}_version} EXACT )

    ADD_ILCSOFT_PACKAGE( ${_pkg_name} )

endmacro()


macro( ADD_ILCSOFT_MARLIN_PACKAGE _pkg_name )

    string( TOLOWER ${_pkg_name} _lpkg_name )

    ADD_ILCSOFT_PACKAGE( ${_pkg_name} )

    if( "${install_${_lpkg_name}}" STREQUAL "YES" )
        message( "++ install ${_lpkg_name} (${${_lpkg_name}_version}) from source" )
        set( MARLIN_DLL "${MARLIN_DLL}:${CMAKE_SHARED_LIBRARY_PREFIX}${_pkg_name}${CMAKE_SHARED_LIBRARY_SUFFIX}" )
    endif()

endmacro()


macro( ADD_ILCSOFT_CONFIG_PACKAGE _pkg_name )

    string( TOLOWER ${_pkg_name} _lpkg_name )

    # no configure, build, install steps needed
    set( ${_lpkg_name}_ep_args ${_lpkg_name}_ep_args
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "no configure needed"
        BUILD_COMMAND ${CMAKE_COMMAND} -E echo "no build needed"
        INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "no install needed"
    )

    ADD_ILCSOFT_PACKAGE( ${_pkg_name} )

    if( "${install_${_lpkg_name}}" STREQUAL "YES" )
        message( "++ install ${_lpkg_name} (${${_lpkg_name}_version}) from source" )
    endif()

endmacro()




# ----- iLCSoft core packages ------------------------------------------------

ADD_ILCSOFT_CORE_PACKAGE( ILCUTIL )
ADD_ILCSOFT_CORE_PACKAGE( LCIO )
ADD_ILCSOFT_CORE_PACKAGE( CED )
ADD_ILCSOFT_CORE_PACKAGE( GEAR )
ADD_ILCSOFT_CORE_PACKAGE( CondDBMySQL )
file( APPEND ${ilcsoft_env_init_script} "export COND_DB_DEBUGLOG=/dev/stdout\n" )
ADD_ILCSOFT_CORE_PACKAGE( LCCD )
ADD_ILCSOFT_CORE_PACKAGE( RAIDA )
ADD_ILCSOFT_CORE_PACKAGE( Marlin )
ADD_ILCSOFT_CORE_PACKAGE( MarlinUtil )
ADD_ILCSOFT_CORE_PACKAGE( KalTest )
ADD_ILCSOFT_CORE_PACKAGE( KalDet )
ADD_ILCSOFT_CORE_PACKAGE( PandoraPFANew )


# ----- Marlin plugins -------------------------------------------------------

# MarlinReco needs special treatment because it is both:
# CORE_PACKAGE (due to MarlinRecoConfig.cmake) and Marlin plugin
ADD_ILCSOFT_CORE_PACKAGE( MarlinReco )
if( "${install_marlinreco}" STREQUAL "YES" )
    set( MARLIN_DLL "${MARLIN_DLL}:${CMAKE_SHARED_LIBRARY_PREFIX}MarlinReco${CMAKE_SHARED_LIBRARY_SUFFIX}" )
endif()

ADD_ILCSOFT_MARLIN_PACKAGE( CEDViewer )
ADD_ILCSOFT_MARLIN_PACKAGE( Overlay )
ADD_ILCSOFT_MARLIN_PACKAGE( LCFIVertex )
ADD_ILCSOFT_MARLIN_PACKAGE( Garlic )
ADD_ILCSOFT_MARLIN_PACKAGE( MarlinPandora )
ADD_ILCSOFT_MARLIN_PACKAGE( PandoraAnalysis )
# FIXME cyclic dependency to pandorapfanew
if( "${install_pandorapfanew}" STREQUAL "YES" )
    list( APPEND pandoramonitoring_cmake_args "-DPandoraPFANew_DIR=${ilcsoft_install_prefix}" )
else()
    list( APPEND pandoramonitoring_cmake_args "-DPandoraPFANew_DIR=${PandoraPFANew_ROOT}" )
endif()
ADD_ILCSOFT_MARLIN_PACKAGE( PandoraMonitoring )
ADD_ILCSOFT_MARLIN_PACKAGE( PandoraPFA )
ADD_ILCSOFT_MARLIN_PACKAGE( SiliconDigi )
ADD_ILCSOFT_MARLIN_PACKAGE( FastJetClustering )
ADD_ILCSOFT_MARLIN_PACKAGE( MarlinTPC )
ADD_ILCSOFT_MARLIN_PACKAGE( Eutelescope )


# ----- Config packages ------------------------------------------------------

ADD_ILCSOFT_CONFIG_PACKAGE( standardconfig )
ADD_ILCSOFT_CONFIG_PACKAGE( mokkadbconfig )
ADD_ILCSOFT_CONFIG_PACKAGE( lcfimokkabasednets )
ADD_ILCSOFT_CONFIG_PACKAGE( cmakemodules )




file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- MARLIN_DLL -----------------------------\n" )
file( APPEND ${ilcsoft_env_init_script} "if test -n \"$MARLIN_DLL\" ; then\n" )
file( APPEND ${ilcsoft_env_init_script} "   echo \"overwritting MARLIN_DLL previously set to $MARLIN_DLL\"\n" )
file( APPEND ${ilcsoft_env_init_script} "fi\n" )
file( APPEND ${ilcsoft_env_init_script} "export MARLIN_DLL=${MARLIN_DLL}\n" )


if(APPLE)
    file( APPEND ${ilcsoft_env_init_script} "export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH\n" )
endif(APPLE)

file( APPEND ${ilcsoft_env_init_script} "\n\n" )


# display standard cmake variables
include( MacroDisplayStandardVariables )
DISPLAY_STD_VARIABLES()


message( STATUS "" )
message( "------------------------------------------------------------------------" )
message( "   script for initializing iLCSoft environment has been generated under:" )
message( "   ${ilcsoft_env_init_script}" )
message( "------------------------------------------------------------------------" )
message( STATUS "" )

