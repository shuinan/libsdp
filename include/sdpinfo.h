#pragma once

#include <string>
#include <map>
#include <vector>

#include <time.h>
#include <iostream>

#include "streaminfo.h"
#include "mediainfo.h"
#include "candidate.h"
#include "sourceinfo.h"
#include "iceinfo.h"
#include "dtlsinfo.h"


#ifdef USE_JSON_TRANSFORM
	#include "sdpstruct.h"
#endif

using namespace std;

namespace sdp {
	
	class SDPInfo {
		int						version_ = 1;
		map<string, StreamInfo>	streams_;
		vector<MediaInfo>		medias_;			// as we need to keep order
		vector<CandidateInfo>	candidates_; 	// keep order
		ICEInfo*				ice_ = nullptr;
		DTLSInfo*				dtls_ = nullptr;

	public:
		SDPInfo();
		SDPInfo(ICEInfo* ice, DTLSInfo* dtls, const vector<CandidateInfo>& candidates, const map<string, Capability>& capabilities);
		SDPInfo& operator=(const SDPInfo& sdp);
		SDPInfo(const SDPInfo& sdp);
		~SDPInfo(void);

		void SetVersion(int v) {
			version_ = v;
		}

		void AddMedia(const MediaInfo& media) {
			this->medias_.push_back(media);
		}

		MediaInfo* GetMedia(const string& mtype);
		bool GetMedia(const string& mtype, MediaInfo& m);

		MediaInfo * GetAudioMedia();

		MediaInfo*  GetVideoMedia();

		vector<MediaInfo> GetMediasByType(const string& mtype);

		MediaInfo* GetMediaByID(const string& mid);

		bool ReplaceMedia(const MediaInfo& media);

		const vector<MediaInfo>& GetMedias() const {
			return this->medias_;
		}

		int GetVersion() const {
			return this->version_;
		}

		const DTLSInfo* GetDTLS() const {
			return this->dtls_;
		}

		void SetDTLS(const DTLSInfo* const d);

		const ICEInfo* GetICE() const {
			return this->ice_;
		}

		void SetICE(const ICEInfo* d);

		void AddCandidate(const CandidateInfo& candi) {
			// todo check
			this->candidates_.push_back(candi);
		}

		void AddCandidates(const vector<CandidateInfo>& candidates) {
			for (auto& candidate : candidates) {
				this->AddCandidate(candidate);
			}
		}

		const vector<CandidateInfo>& GetCandidates() const {
			return this->candidates_;
		}

		StreamInfo* GetStream(const string& id) {
			auto it = streams_.find(id);
			return it == streams_.end() ? nullptr : &it->second;
		}

		const map<string, StreamInfo>& GetStreams() const {
			return this->streams_;
		}

		StreamInfo* GetFirstStream() {
			return streams_.empty() ? nullptr : &streams_.begin()->second;
		}

		void AddStream(const StreamInfo& stream) {
			this->streams_[stream.id] = stream;
		}

		void RemoveStream(const StreamInfo& stream) {
			streams_.erase(stream.id);
		}

		void RemoveAllStreams() {
			streams_.clear();
		}

		TrackInfo* GetTrackByMediaID(const string& mid);

		StreamInfo* GetStreamByMediaID(const string& mid);

		SDPInfo Answer(const ICEInfo* ice, const DTLSInfo* dtls, const vector<CandidateInfo>& candidates, const map<string, Capability>& medias);
		
		string String() const;
	
		bool Parse(const string& sdp);
	
	#ifdef USE_JSON_TRANSFORM	
		//////////////////////ToString, ParseFrom using json/struct transform ////////////////////////////
		string ToString() const;
		bool ParseFrom(const string& sdp);
	#endif
	};
}

