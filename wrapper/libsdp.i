/* File : libsdp.i */
%module libsdp

%{
#include "../include/sdpinfo.h"
using namespace sdp;
using namespace nlohmann;
%}

/* Let's just grab the original header file here */
%include "../include/jsonserialize.h"
%include "../include/streaminfo.h"
%include "../include/mediainfo.h"
%include "../include/candidate.h"
%include "../include/sourceinfo.h"
%include "../include/iceinfo.h"
%include "../include/dtlsinfo.h"
%include "../include/sdpinfo.h"