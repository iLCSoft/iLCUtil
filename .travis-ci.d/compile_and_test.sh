#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_${COMPILER}
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
make -j2 && \
make install && \
ctest --output-on-failure
