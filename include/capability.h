#pragma once

#include <string>
#include <vector>


using namespace std;

#include "rtcpfeedbackinfo.h"

namespace sdp {

struct Capability {
	vector<string> 	Codecs;			//  `json:"codecs"`
	bool           	Rtx;			//  `json:"rtx,omitempty"`
	vector<RtcpFeedbackInfo>	Rtcpfbs;//  `json:"rtcpfbs,omitempty"`
	vector<string> 	Extensions;		//  `json:"extensions,omitempty"`
	bool           	Simulcast;		//  `json:"simulcast,omitempty"`
};

}
