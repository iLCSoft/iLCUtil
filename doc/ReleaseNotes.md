# v01-06-02

* 2021-11-03 Thomas Madlener ([PR#22](https://github.com/iLCSoft/iLCUtil/pull/22))
  - Switch to the latest available nightlies in the CI workflows.
  - Remove travis-ci config

* 2021-08-16 Thomas Madlener ([PR#21](https://github.com/iLCSoft/iLCUtil/pull/21))
  - Add a working github actions based CI workflow.
  - Make it possible to use newer cmake versions for installing
    - Fix globbing for dependencies in streamlog doxygen generation, which broke with 3.17
    - Fix name mismatch warning in find_package calls for cmake > 3.17
    - Fix missing project warning in example
  - Bump the minimum required cmake version to 3.14
  - Make the INSTALL_DOC option from streamlog available from the top-level to make it possible to steer this from the outside

# v01-06-01

* 2020-05-13 Frank Gaede ([PR#17](https://github.com/iLCSoft/iLCUtil/pull/17))
  - fix FindMySQL.cmake
          - add correct search path for Ubuntu (>=18.04)

# v01-06

* 2019-08-23 Andre Sailer ([PR#13](https://github.com/iLCSoft/iLCUtil/pull/13))
  - Pass CMAKE_CXX_STANDARD to externalproject_add to allow building packages with standards other than the compiler default or c++11

* 2018-05-08 Andre Sailer ([PR#11](https://github.com/iLCSoft/iLCUtil/pull/11))
  - MacroRootDict: fix constant re-creation of dictionary. ROOT_CINT is no longer creating *.h file but *_rdict.pcm, changing the expected output file will only recreate the dictionary when necessary

# v01-05

* 2017-08-30 Andre Sailer ([PR#6](https://github.com/iLCSoft/ilcutil/pull/6))
  - Logscope: fix for SILENT, which did not get properly restored because of integer overflow issues

* 2018-01-12 Frank Gaede ([PR#7](https://github.com/iLCSoft/ilcutil/pull/7))
  - fix compiler warnings in package streamlog (effc++,...)
  - rm obsolete find macros ( LCIO, ROOT, CLHEP,...)

* 2018-01-16 Frank Gaede ([PR#10](https://github.com/iLCSoft/ilcutil/pull/10))
  - update versions
      - streamlog v00-04
      - cmakemodules v01-17

* 2018-01-16 Frank Gaede ([PR#9](https://github.com/iLCSoft/ilcutil/pull/9))
  - add missing variables to MacroRootDict.cmake
        - used to be defined in obsolete FindROOT.cmake

