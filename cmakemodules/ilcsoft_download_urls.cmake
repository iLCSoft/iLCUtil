# this file contains the svn download urls of the ilcsoft packages
# there should be no need to change this file unless a new package is added!

# -- define a default desy svn server if not previously defined --------------
if( NOT DEFINED desy_svn_repository )
    #set( desy_svn_repository "https://svnsrv.desy.de/svn" )    # use grid certificate (pkcs12)
    #set( desy_svn_repository "https://svnsrv.desy.de/desy" )   # use desy account (kerberos)
    set( desy_svn_repository "https://svnsrv.desy.de/public" )  # public (no authentication)
endif()
# ----------------------------------------------------------------------------


## -- define a default lcio cvs user if not previously defined ----------------
#if( NOT DEFINED lcio_cvs_user )
#    set( lcio_cvs_user "anonymous" )
#endif()
## ----------------------------------------------------------------------------
#
## -------- lcio needs special treatment due to cvs ---------------------------
#set( lcio_download_url
#    CVS_REPOSITORY ":pserver:${lcio_cvs_user}@cvs.freehep.org:/cvs/lcio"
#    CVS_MODULE lcio
#    CVS_TAG -r${lcio_version}
#)
## ----------------------------------------------------------------------------

# ============ ilcsoft core packages ================
set( lcio_download_url               SVN_REPOSITORY "svn://svn.freehep.org/lcio/${lcio_version}" )
set( ilcutil_download_url            SVN_REPOSITORY "${desy_svn_repository}/ilctools/ilcutil/${ilcutil_version}" )
set( ced_download_url                SVN_REPOSITORY "${desy_svn_repository}/marlinreco/CED/${ced_version}" )
set( gear_download_url               SVN_REPOSITORY "${desy_svn_repository}/gear/gear/${gear_version}" )
set( conddbmysql_download_url        SVN_REPOSITORY "${desy_svn_repository}/calice/CondDBMySQL/${conddbmysql_version}" )
set( lccd_download_url               SVN_REPOSITORY "${desy_svn_repository}/lccd/lccd/${lccd_version}" )
set( raida_download_url              SVN_REPOSITORY "${desy_svn_repository}/ilctools/RAIDA/${raida_version}" )
set( marlin_download_url             SVN_REPOSITORY "${desy_svn_repository}/marlin/Marlin/${marlin_version}" )
set( marlinutil_download_url         SVN_REPOSITORY "${desy_svn_repository}/marlinreco/MarlinUtil/${marlinutil_version}" )
set( kaltest_download_url            SVN_REPOSITORY "${desy_svn_repository}/kaltest/KalTest/${kaltest_version}" )
set( kaldet_download_url             SVN_REPOSITORY "${desy_svn_repository}/kaltest/KalDet/${kaldet_version}" )
set( pandorapfanew_download_url      SVN_REPOSITORY "${desy_svn_repository}/PandoraPFANew/PandoraPFANew/${pandorapfanew_version}" )

set( mokka_download_url              SVN_REPOSITORY "http://llrforge.in2p3.fr/svn/Mokka/${mokka_version}" )
set( druid_download_url              SVN_REPOSITORY "${desy_svn_repository}/Druid/${druid_version}" )

# ============ ilcsoft marlin plugins ===============
set( marlinreco_download_url         SVN_REPOSITORY "${desy_svn_repository}/marlinreco/MarlinReco/${marlinreco_version}" )
set( marlinkinfit_download_url       SVN_REPOSITORY "${desy_svn_repository}/marlinreco/MarlinKinfit/${marlinkinfit_version}" )
set( cedviewer_download_url          SVN_REPOSITORY "${desy_svn_repository}/marlinreco/CEDViewer/${cedviewer_version}" )
set( overlay_download_url            SVN_REPOSITORY "${desy_svn_repository}/marlin/Overlay/${overlay_version}" )
set( lcfivertex_download_url         SVN_REPOSITORY "${desy_svn_repository}/marlinreco/LCFIVertex/${lcfivertex_version}" )
set( garlic_download_url             SVN_REPOSITORY "${desy_svn_repository}/Garlic/${garlic_version}" )
set( marlinpandora_download_url      SVN_REPOSITORY "${desy_svn_repository}/PandoraPFANew/MarlinPandora/${marlinpandora_version}" )
set( pandoraanalysis_download_url    SVN_REPOSITORY "${desy_svn_repository}/PandoraPFANew/PandoraAnalysis/${pandoraanalysis_version}" )
set( pandoramonitoring_download_url  SVN_REPOSITORY "${desy_svn_repository}/PandoraPFANew/PandoraMonitoring/${pandoramonitoring_version}" )
set( pandorapfa_download_url         SVN_REPOSITORY "${desy_svn_repository}/marlinreco/PandoraPFA/${pandorapfa_version}" )
set( silicondigi_download_url        SVN_REPOSITORY "${desy_svn_repository}/marlinreco/SiliconDigi/${silicondigi_version}" )
set( fastjetclustering_download_url  SVN_REPOSITORY "${desy_svn_repository}/marlinreco/FastJetClustering/${fastjetclustering_version}" )
set( marlintpc_download_url          SVN_REPOSITORY "${desy_svn_repository}/marlintpc/${marlintpc_version}" )
set( eutelescope_download_url        SVN_REPOSITORY "${desy_svn_repository}/eutelescope/Eutelescope/${eutelescope_version}" )

# ============ ilcsoft config packages ==============
set( cmakemodules_download_url       SVN_REPOSITORY "${desy_svn_repository}/ilctools/CMakeModules/${cmakemodules_version}" )
set( standardconfig_download_url     SVN_REPOSITORY "${desy_svn_repository}/marlinreco/StandardConfig/${standardconfig_version}" )
set( mokkadbconfig_download_url      SVN_REPOSITORY "${desy_svn_repository}/ilctools/MokkaDBConfig/${mokkadbconfig_version}" )
set( lcfimokkabasednets_download_url SVN_REPOSITORY "${desy_svn_repository}/tagnet/LCFI_MokkaBasedNets/${lcfimokkabasednets_version}" )

