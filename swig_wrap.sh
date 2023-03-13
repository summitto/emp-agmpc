#!/bin/sh

swig=`which swig`
if [ -z ${swig} ]
then
    echo "This package requires swig, aborting"
    return
fi

cd swigmpc
swig -go -intgosize 64 -cgo -c++ swig_mpc.i
cd -

CGO_CXXFLAGS="-mavx2 -maes -ggdb2 -fpic" CGO_LDFLAGS="-lcrypto -lboost_filesystem" go build ./swigmpc
