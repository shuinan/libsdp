#pragma once

#include <cwctype>
#include <locale>
#include <string>

#include "stringUtil.h"
#include "sourcegroupinfo.h"
#include "trackencodinginfo.h"

using namespace std;

namespace sdp {
	
	struct TrackInfo {
		string					id;
		string					mediaID;
		string					media;     	// "audio" | "video"
		vector<uint64_t>		ssrcs;
		vector<SourceGroupInfo>	groups;
		vector<vector<TrackEncodingInfo> >	encodings;

		TrackInfo() {}
		TrackInfo(const string& ID, const string& med) :
			id(ID),
			media(med)
		{}

		void AddSSRC(uint64_t ssrc) {
			ssrcs.push_back(ssrc);
		}

		void AddSourceGroup(const SourceGroupInfo& group) {
			groups.push_back(group);
		}

		SourceGroupInfo* GetSourceGroup(const string& schematics) {
			for (auto& group : groups) {
				if (stringUtil::tolower(group.semantics) == stringUtil::tolower(schematics)) {
					return &group;
				}
			}
			return nullptr;
		}

		void AddEncoding(const TrackEncodingInfo& encoding) {
			vector<TrackEncodingInfo> ts;
			ts.push_back(encoding);
			encodings.push_back(ts);
		}

		void AddAlternativeEncodings(const vector<TrackEncodingInfo>& alternatives) {
			encodings.push_back(alternatives);
		}
	};
}