##############################################################################
# settings for uploading test results to cdash server
#
# author: Jan Engels, Desy - IT
# date:   2011-07-01
##############################################################################


set( cdash_server_url "http://zitxserve1.desy.de/~aplin/CDash/generateCTestConfig.php" )

set( download_cmd wget -O )
if( APPLE )
    set( download_cmd curl -o )
endif()

list( APPEND lcio_ep_args       PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=11" )
list( APPEND ced_ep_args        PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=03" )
list( APPEND gear_ep_args       PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=09" )
list( APPEND lccd_ep_args       PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=10" )
list( APPEND raida_ep_args      PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=08" )
list( APPEND kaltest_ep_args    PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=27" )
list( APPEND kaldet_ep_args     PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=28" )
list( APPEND marlin_ep_args     PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=02" )
list( APPEND marlinutil_ep_args PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=07" )
list( APPEND marlinreco_ep_args PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=06" )
list( APPEND marlintpc_ep_args  PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=29" )
list( APPEND cedviewer_ep_args  PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=04" )
list( APPEND overlay_ep_args    PATCH_COMMAND ${download_cmd} CTestConfig.cmake "${cdash_server_url}?projectid=13" )

