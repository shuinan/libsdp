#pragma once

#include <string>

using namespace std;

namespace sdp {

	struct SourceInfo {
		uint64_t	ssrc;
		string		cname;
		string		streamID;
		string		trackID;
		
		SourceInfo() : ssrc(0) {}
		SourceInfo(uint64_t ss) : ssrc(ss) {}
	};

}
