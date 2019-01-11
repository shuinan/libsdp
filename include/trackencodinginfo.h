#pragma once

#include <string>

#include "codecinfo.h"

using namespace std;

namespace sdp {

	struct TrackEncodingInfo {
		string					id;
		bool					paused = false;
		map<string, CodecInfo>	codecs;
		map<string, string>		params;

		TrackEncodingInfo() {}
		TrackEncodingInfo(const string& Id, bool pause)
			: id(Id), paused(pause)
		{}

		void AddCodec(const CodecInfo& codec) {
			codecs[codec.codec] = codec;
		}

		void AddParam(const string& id, const string& param) {
			params[id] = param;
		}

		bool IsPaused() {
			return paused;
		}
	};

}
