##############################################################################
# settings for uploading test results to cdash server
#
# author: Jan Engels, Desy - IT
# date:   2011-07-01
##############################################################################


set( cdash_server_url "http://zitxserve1.desy.de/~aplin/CDash/generateCTestConfig.php" )

list( APPEND lcio_ep_args       PATCH_COMMAND wget "${cdash_server_url}?projectid=11" -O CTestConfig.cmake )
list( APPEND ced_ep_args        PATCH_COMMAND wget "${cdash_server_url}?projectid=03" -O CTestConfig.cmake )
list( APPEND gear_ep_args       PATCH_COMMAND wget "${cdash_server_url}?projectid=09" -O CTestConfig.cmake )
list( APPEND lccd_ep_args       PATCH_COMMAND wget "${cdash_server_url}?projectid=10" -O CTestConfig.cmake )
list( APPEND raida_ep_args      PATCH_COMMAND wget "${cdash_server_url}?projectid=08" -O CTestConfig.cmake )
list( APPEND kaltest_ep_args    PATCH_COMMAND wget "${cdash_server_url}?projectid=27" -O CTestConfig.cmake )
list( APPEND kaldet_ep_args     PATCH_COMMAND wget "${cdash_server_url}?projectid=28" -O CTestConfig.cmake )
list( APPEND marlin_ep_args     PATCH_COMMAND wget "${cdash_server_url}?projectid=02" -O CTestConfig.cmake )
list( APPEND marlinutil_ep_args PATCH_COMMAND wget "${cdash_server_url}?projectid=07" -O CTestConfig.cmake )
list( APPEND marlinreco_ep_args PATCH_COMMAND wget "${cdash_server_url}?projectid=06" -O CTestConfig.cmake )
list( APPEND marlintpc_ep_args  PATCH_COMMAND wget "${cdash_server_url}?projectid=29" -O CTestConfig.cmake )
list( APPEND cedviewer_ep_args  PATCH_COMMAND wget "${cdash_server_url}?projectid=04" -O CTestConfig.cmake )
list( APPEND overlay_ep_args    PATCH_COMMAND wget "${cdash_server_url}?projectid=13" -O CTestConfig.cmake )

