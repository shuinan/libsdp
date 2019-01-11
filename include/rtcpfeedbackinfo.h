#pragma once

#include <string>
#include <vector>

using namespace std;

namespace sdp {	
	struct RtcpFeedbackInfo {
		string			id; 	//  `json:"id,omitempty"`
		vector<string>	params; //	`json:"params,omitempty"`

		RtcpFeedbackInfo() {}
		RtcpFeedbackInfo(const string& i, const vector<string>& ps) : id(i), params(ps) {}
	};
}
