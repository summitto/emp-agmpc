%module swigmpc

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%{
#include "mpc_wrapper.h"
%}

// Instantiate templates
namespace std {
  %template(BoolVector) vector<bool>;
}

%exception {
  try {
    $action
  } catch(...) {
    SWIG_exception(SWIG_RuntimeError, "Unknown exception");
  }
}

/* Let's just grab the original header file here */
%include "mpc_wrapper.h"

%exception;
