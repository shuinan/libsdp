/* File : libsdp.i */
%module libsdp

%{
#include "sdpinfo.h"
%}

/* Let's just grab the original header file here */
%include "sdpinfo.h"
