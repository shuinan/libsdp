#pragma once

#include <string>
#include "setup.h"

using namespace std;

namespace sdp {

	struct DTLSInfo {
		Setup       setup;
		string      hash;
		string      fingerprint;		
	};
}
