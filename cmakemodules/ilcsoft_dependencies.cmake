# chain of dependencies from the ilcsoft tools
# there should be no need to change this file!!

# ===== external tools ================
set( root_depends               gsl  )

# ============ ilcsoft core packages ================
set( lcio_depends               ilcutil root ) # ilcutil ?
set( ced_depends                ilcutil glut )
set( gear_depends               ilcutil root )
set( conddbmysql_depends        ilcutil mysql )
set( lccd_depends               ilcutil lcio conddbmysql )
set( raida_depends              ilcutil root )
set( marlin_depends             ilcutil lcio gear raida lccd clhep qt4 )
set( marlinutil_depends         ilcutil marlin ced gsl )
set( kaltest_depends            ilcutil root )
set( kaldet_depends             marlin kaltest )
set( pandorapfanew_depends      ilcutil )

# ============ ilcsoft marlin plugins ===============
set( marlinreco_depends         marlinutil cernlib )
set( cedviewer_depends          marlinutil )
set( overlay_depends            marlin )
set( lcfivertex_depends         marlinutil )
set( garlic_depends             marlinutil )
set( marlinpandora_depends      marlinutil pandorapfanew )
set( pandoraanalysis_depends    marlin )
set( pandoramonitoring_depends  pandorapfanew root )
set( pandorapfa_depends         marlinutil )   # DEPRECATED
set( silicondigi_depends        marlin gsl )   # DEPRECATED
set( fastjetclustering_depends  marlin fastjet )
set( marlintpc_depends          marlin kaldet gsl )
set( eutelescope_depends        marlinutil )

