#pragma once

#include <string>

#include "stringUtil.h"

#include "trackinfo.h"

using namespace std;

namespace sdp {


	struct StreamInfo {
		string					id;
		map<string, TrackInfo>	tracks;

		StreamInfo() {}
		StreamInfo(const string& streamID) : id(streamID) {}

		void AddTrack(const TrackInfo& track) {
			tracks[track.id] = track;
		}

		void RemoveTrack(const TrackInfo& track) {
			tracks.erase(track.id);
		}

		void RemoveTrackById(const string& trackId) {
			tracks.erase(trackId);
		}

		TrackInfo* GetFirstTrack(const string& media) {
			for (auto& trak : tracks) {
				if (stringUtil::tolower(trak.second.media) == stringUtil::tolower(media)) {
					return &trak.second;
				}
			}
			return nullptr;
		}

		void RemoveAllTracks() {
			tracks.clear();
		}

		bool GetTrack(const string& trackId, TrackInfo& t) {
			map<string, TrackInfo>::iterator it = tracks.find(trackId);
			if (it == tracks.end())
				return false;
			t = it->second;
			return true;
		}
		TrackInfo* GetTrack(const string& trackId) {
			map<string, TrackInfo>::iterator it = tracks.find(trackId);
			return it == tracks.end() ? nullptr : &it->second;
		}
	};

}