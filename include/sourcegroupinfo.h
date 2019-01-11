#pragma once

#include <string>
#include <vector>

using namespace std;

namespace sdp {

	struct SourceGroupInfo {
		string			semantics;
		vector<int>		ssrcs;

		SourceGroupInfo() {}
		SourceGroupInfo(const string& sema, const vector<int>& ss) :
			semantics(sema),
			ssrcs(ss)
		{}
	};

}
