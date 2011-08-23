

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


# ----- default install prefix set to PROJECT_BINARY_DIR ---------------------
if( NOT DEFINED ilcsoft_install_prefix )
    set( ilcsoft_install_prefix "${PROJECT_BINARY_DIR}/ilcsoft" )
endif()

if( NOT DEFINED external_install_prefix )
    set( external_install_prefix "${ilcsoft_install_prefix}-external" )
endif()
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
file( WRITE ${ilcsoft_reconfigure_script} "read -p\"reconfigure EVERYTHING FROM SCRATCH? This will ERASE DIRECTORY [${ilcsoft_install_prefix}] [y/n] \" r\n" )
file( APPEND ${ilcsoft_reconfigure_script} "test \"\${r:0:1}\" != \"y\" && exit 0\n" )
file( APPEND ${ilcsoft_reconfigure_script} "for i in ${PROJECT_BINARY_DIR}/*-*-prefix; do test -d $i/build && pushd $i/build && make clean uninstall && popd ; done\n" )
file( APPEND ${ilcsoft_reconfigure_script} "find ${PROJECT_BINARY_DIR}/*-*-prefix -name CMakeCache.txt -exec rm -v {} \;\n" )
file( APPEND ${ilcsoft_reconfigure_script} "rm -rf \"${ilcsoft_install_prefix}\"\n" )
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
            else()
                if( "${_pkg_install_flag}" STREQUAL "AUTO" OR "${_pkg_install_flag}" STREQUAL "" )
                    message( STATUS "could not find ${_lpkg_name} (${_pkg_version})" )
                    set( _pkg_install_flag "YES" ) # if the package was not found it should be installed
                else()
                    message( "Warning: ${_lpkg_name} package disabled! Please disable any other packages or options depending on ${_lpkg_name}" )
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
            URL "http://ilcsoft.desy.de/cernlib/cernlib-${cernlib_version}-${GCC_MACHINE}-gcc${GCC_VERSION}.tgz"
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

FIND_PACKAGE( MySQL ${mysql_version} ) #EXACT )

if( NOT MYSQL_FOUND )
    message( "Warning: mysql not found! Please disable any packages or options depending on mysql" )
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
        CONFIGURE_COMMAND ./configure --enable-static=no --prefix=${external_install_prefix}
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

    # export CLHEP_BASE_DIR , CLHEP_INCLUDE_DIR ?
endif()



# ----------------------------------------------------------------------------
# --- ROOT
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( ROOT ${root_version} EXACT )

if( "${install_root}" STREQUAL "YES" )
    ExternalProject_Add( root
        #${common_ep_args}
        ${root_depends}
        URL "ftp://root.cern.ch/root/root_v${root_version}.source.tar.gz"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./configure --fail-on-missing --enable-explicitlink --enable-builtin-pcre --enable-gdml --enable-gsl-shared --enable-minuit2 --enable-roofit --enable-soversion --enable-table --enable-unuran --enable-xrootd --with-gsl-incdir=${GSL_INCLUDE_DIRS} --with-gsl-libdir=${GSL_LIBRARY_DIRS}
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
        CONFIGURE_COMMAND ${fastjet_configure_command}
    )
endif()



# ----------------------------------------------------------------------------
# --- QT4
# ----------------------------------------------------------------------------

FIND_PACKAGE_WRAPPER_DUMMY_TARGET( Qt4 ${qt4_version} ) #EXACT )

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

        # installing qt 4.2.2 under ubuntu 10.04 failed trying to autodetect endianness
        include( TestBigEndian )
        TEST_BIG_ENDIAN( big_endian )
        if( big_endian )
            list( APPEND qt4_cfg_options -big-endian )
        else()
            list( APPEND qt4_cfg_options -little-endian )
        endif()
    
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
    # export QTDIR, QMAKESPEC ?
endif()




# ============================================================================
# ===================== ILCSOFT PACKAGES =====================================
# ============================================================================


macro( ADD_ILCSOFT_PACKAGE _pkg_name )

    string( TOLOWER ${_pkg_name} _lpkg_name )


    EXTRACT_NUMERIC_VERSION( "${${_lpkg_name}_version}" )

    if( NUMERIC_VERSION )
        set( ${_lpkg_name}_ep_base ${_lpkg_name}-${NUMERIC_VERSION}-prefix )
    else()
        set( ${_lpkg_name}_ep_base ${_lpkg_name}-${${_lpkg_name}_version}-prefix )
    endif()

    # replace slashes with underscores
    string( REPLACE "/" "_" ${_lpkg_name}_ep_base "${${_lpkg_name}_ep_base}" )

    # external project arguments passed to an ilcsoft package
    list( APPEND ${_lpkg_name}_ep_args
        LIST_SEPARATOR % # needed for CMAKE_PREFIX_PATH
        SOURCE_DIR ${ilcsoft_sources_prefix}${${_lpkg_name}_ep_base}/src
        BINARY_DIR ${${_lpkg_name}_ep_base}/build
        DOWNLOAD_DIR ${${_lpkg_name}_ep_base}/download
        STAMP_DIR ${${_lpkg_name}_ep_base}/stamp
        TMP_DIR ${${_lpkg_name}_ep_base}/tmp
        #INSTALL_DIR ${${_lpkg_name}_ep_base}/install
        INSTALL_DIR ${ilcsoft_install_prefix}
    )

    if( "${install_${_lpkg_name}}" STREQUAL "YES" )

        if( DEFINED ${_lpkg_name}_depends )
            foreach( _dep ${${_lpkg_name}_depends} )
                if( "${install_${_dep}}" STREQUAL "SKIP" )
                    message( "Warning: dependency from ${_lpkg_name} could not be found or installed: ${_dep}" )
                endif()
            endforeach()
        endif()


        ExternalProject_Add( ${_lpkg_name}
            ${${_lpkg_name}_depends}
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
        set( MARLIN_DLL "${ilcsoft_install_prefix}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${_pkg_name}${CMAKE_SHARED_LIBRARY_SUFFIX}:${MARLIN_DLL}" )
    endif()

endmacro()


macro( ADD_ILCSOFT_CONFIG_PACKAGE _pkg_name )

    string( TOLOWER ${_pkg_name} _lpkg_name )

    # no configure, build, install steps needed
    list( APPEND ${_lpkg_name}_ep_args
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
if( "${install_lcio}" STREQUAL "YES" )
    # needed for mokka
    file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- LCIO -----------------------------------\n" )
    file( APPEND ${ilcsoft_env_init_script} "export LCIO=${ilcsoft_install_prefix}\n" )
endif()
ADD_ILCSOFT_CORE_PACKAGE( CED )
ADD_ILCSOFT_CORE_PACKAGE( GEAR )
if( "${install_gear}" STREQUAL "YES" )
    # needed for mokka
    file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- GEAR -----------------------------------\n" )
    file( APPEND ${ilcsoft_env_init_script} "export GEAR=${ilcsoft_install_prefix}\n" )
endif()
ADD_ILCSOFT_CORE_PACKAGE( CondDBMySQL )
#if( "${install_conddbmysql}" STREQUAL "YES" )
file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- CONDDBMYSQL ----------------------------\n" )
file( APPEND ${ilcsoft_env_init_script} "export COND_DB_DEBUGLOG=/dev/stdout\n" )
#endif()
ADD_ILCSOFT_CORE_PACKAGE( LCCD )
ADD_ILCSOFT_CORE_PACKAGE( RAIDA )
ADD_ILCSOFT_CORE_PACKAGE( Marlin )
ADD_ILCSOFT_CORE_PACKAGE( MarlinUtil )
ADD_ILCSOFT_CORE_PACKAGE( KalTest )
ADD_ILCSOFT_CORE_PACKAGE( KalDet )
ADD_ILCSOFT_CORE_PACKAGE( PandoraPFANew )


# ----------------------------------------------------------------------------
# --- Mokka
# ----------------------------------------------------------------------------

if( "${install_mokka}" STREQUAL "YES" )

    message( "++ install mokka (${mokka_version}) from source" )

    list( APPEND mokka_ep_args
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "no configure needed"
        BUILD_COMMAND . ${ilcsoft_env_init_script} && eval ${CMAKE_BUILD_TOOL} -j1 -C $G4WORKDIR/source
        INSTALL_COMMAND . ${ilcsoft_env_init_script} && eval ${CMAKE_COMMAND} -E copy $G4WORKDIR/bin/$G4SYSTEM/Mokka ${ilcsoft_install_prefix}/bin
    )

    #ADD_ILCSOFT_CORE_PACKAGE( Mokka )
    ADD_ILCSOFT_PACKAGE( Mokka )

    #ExternalProject_Add( mokka
    #    ${mokka_depends}
    #    ${mokka_download_url}
    #    BUILD_IN_SOURCE 1
    #    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "no configure needed"
    #    BUILD_COMMAND . ${ilcsoft_env_init_script} && ${CMAKE_BUILD_TOOL} -j1 -C source
    #    INSTALL_COMMAND . ${ilcsoft_env_init_script} && eval ${CMAKE_COMMAND} -E copy $G4WORKDIR/bin/$G4SYSTEM/Mokka ${ilcsoft_install_prefix}/bin
    #)

    if( NOT DEFINED ENV{G4ENV_INIT} )
        message( FATAL_ERROR "please set G4ENV_INIT=/path/to/geant4/env.sh script or install_mokka to NO" )
    endif()

    ExternalProject_Get_Property( mokka source_dir )

    file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- MOKKA ----------------------------------\n" )
    file( APPEND ${ilcsoft_env_init_script} "export G4WORKDIR=${source_dir}\n" )
    file( APPEND ${ilcsoft_env_init_script} ". $ENV{G4ENV_INIT}\n" )
    # disable some graphics drivers
    file( APPEND ${ilcsoft_env_init_script} "unset G4VIS_BUILD_OIX_DRIVER G4VIS_USE_OIX_DRIVER G4VIS_USE_OIX\n" )
    file( APPEND ${ilcsoft_env_init_script} "unset G4VIS_BUILD_OPENGLXM_DRIVER G4VIS_USE_OPENGLXM\n" )
    file( APPEND ${ilcsoft_env_init_script} "unset G4UI_BUILD_XAW_SESSION G4UI_USE_XAW\n" )
    file( APPEND ${ilcsoft_env_init_script} "unset G4UI_BUILD_XM_SESSION G4UI_USE_XM\n" )

endif()


# ---------------------------------------------------------------------------
# --- Druid
# ---------------------------------------------------------------------------

if( "${install_druid}" STREQUAL "YES" )

    message( "++ install druid (${druid_version}) from source" )

    # add as ILCSOFT_PACKAGE ?

    ExternalProject_Add( druid
        ${druid_depends}
        ${druid_download_url}
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo "no configure needed"
        BUILD_COMMAND . ${ilcsoft_env_init_script} && ${CMAKE_BUILD_TOOL} -j1 -C src
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy bin/Druid ${ilcsoft_install_prefix}/bin
    )
endif()


# ----- Marlin plugins -------------------------------------------------------

# MarlinReco needs special treatment because it is both:
# CORE_PACKAGE (due to MarlinRecoConfig.cmake) and a Marlin plugin
ADD_ILCSOFT_CORE_PACKAGE( MarlinReco )
if( "${install_marlinreco}" STREQUAL "YES" )
    set( MARLIN_DLL "${ilcsoft_install_prefix}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}MarlinReco${CMAKE_SHARED_LIBRARY_SUFFIX}:${MARLIN_DLL}" )
endif()

# MarlinTPC needs special treatment because it is both:
# CORE_PACKAGE (due to MarlinTPCConfig.cmake) and a Marlin plugin
ADD_ILCSOFT_CORE_PACKAGE( MarlinTPC )
if( "${install_marlintpc}" STREQUAL "YES" )
    set( MARLIN_DLL "${ilcsoft_install_prefix}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}MarlinTPC${CMAKE_SHARED_LIBRARY_SUFFIX}:${MARLIN_DLL}" )
endif()


ADD_ILCSOFT_MARLIN_PACKAGE( MarlinKinfit )
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

# remove libPandoraMonitoring from MARLIN_DLL
if( "${install_pandoramonitoring}" STREQUAL "YES" )
    string( REPLACE "${ilcsoft_install_prefix}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PandoraMonitoring${CMAKE_SHARED_LIBRARY_SUFFIX}:" "" MARLIN_DLL "${MARLIN_DLL}" )
endif()

ADD_ILCSOFT_MARLIN_PACKAGE( PandoraPFA )
ADD_ILCSOFT_MARLIN_PACKAGE( SiliconDigi )
ADD_ILCSOFT_MARLIN_PACKAGE( FastJetClustering )
#ADD_ILCSOFT_MARLIN_PACKAGE( MarlinTPC )
ADD_ILCSOFT_MARLIN_PACKAGE( Eutelescope )
# millepede2 + eudaq


file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- MARLIN_DLL -----------------------------\n" )
file( APPEND ${ilcsoft_env_init_script} "if test -n \"$MARLIN_DLL\" ; then\n" )
file( APPEND ${ilcsoft_env_init_script} "   echo \"overwritting MARLIN_DLL previously set to $MARLIN_DLL\"\n" )
file( APPEND ${ilcsoft_env_init_script} "fi\n" )
file( APPEND ${ilcsoft_env_init_script} "export MARLIN_DLL=${MARLIN_DLL}\n" )



# ----- Config packages ------------------------------------------------------

ADD_ILCSOFT_CONFIG_PACKAGE( standardconfig )
if( "${install_standardconfig}" STREQUAL "YES" )
    ExternalProject_Get_Property( standardconfig source_dir )
    file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- STANDARDCONFIG -------------------------\n" )
    file( APPEND ${ilcsoft_env_init_script} "export STANDARDCONFIG=${source_dir}\n" )
endif()

ADD_ILCSOFT_CONFIG_PACKAGE( mokkadbconfig )
if( "${install_mokkadbconfig}" STREQUAL "YES" )
    ExternalProject_Get_Property( mokkadbconfig source_dir )
    file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- MOKKADBCONFIG --------------------------\n" )
    file( APPEND ${ilcsoft_env_init_script} "export MOKKADBCONFIG=${source_dir}\n" )
    file( APPEND ${ilcsoft_env_init_script} "export PATH=$MOKKADBCONFIG/scripts:$PATH\n" )
    file( APPEND ${ilcsoft_env_init_script} "export MOKKA_DUMP_FILE=$MOKKADBCONFIG/mokka-dbdump.sql.tgz\n" )
endif()


ADD_ILCSOFT_CONFIG_PACKAGE( lcfimokkabasednets )
if( "${install_lcfimokkabasednets}" STREQUAL "YES" )
    ExternalProject_Get_Property( lcfimokkabasednets source_dir )
    file( APPEND ${ilcsoft_env_init_script} "\n\n#---------------------- LCFIMOKKABASEDNETS ---------------------\n" )
    file( APPEND ${ilcsoft_env_init_script} "export LCFIMOKKABASEDNETS=${source_dir}\n" )
endif()


ADD_ILCSOFT_CONFIG_PACKAGE( cmakemodules )


# ============================================================================



if(APPLE)
    file( APPEND ${ilcsoft_env_init_script} "\nexport DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH\n" )
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

