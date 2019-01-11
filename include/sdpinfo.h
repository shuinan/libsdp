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
#include "../sdp-transform/sdpstruct.h"

#include "D:/rtc/third/libsdptransform/include/sdptransform.hpp"

#include "stringUtil.h"

//	sdptransform "github.com/notedit/media-server-go/sdp-transform"

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
		SDPInfo() : ice_(nullptr), dtls_(nullptr) {}
		SDPInfo(ICEInfo* ice, DTLSInfo* dtls, const vector<CandidateInfo>& candidates, const map<string, Capability>& capabilities)
			: candidates_(candidates), ice_(nullptr), dtls_(nullptr)
		{
			SetICE(ice);
			SetDTLS(dtls);

			int dyn = 96;
			for (auto cap : capabilities) {
				MediaInfo media(cap.first, &cap.second);
				for (auto& codec : media.Codecs()) {
					if (codec.second.ctype >= 96) {
						codec.second.ctype = ++dyn;
					}
					if (codec.second.rtx > 0) {
						codec.second.rtx = ++dyn;
					}
				}
				AddMedia(media);
			}
		}
		SDPInfo& operator=(const SDPInfo& sdp) {
			version_ = sdp.GetVersion();
			streams_ = sdp.GetStreams();
			medias_ = sdp.GetMedias();
			candidates_ = sdp.GetCandidates();
			SetICE(this->GetICE());
			SetDTLS(this->GetDTLS());
			return *this;
		}

		SDPInfo(const SDPInfo& sdp) {
			*this = sdp;
		}
		~SDPInfo(void) {
			DeleteObj(ice_);
			DeleteObj(dtls_);
		}


		void SetVersion(int v) {
			version_ = v;
		}

		void AddMedia(const MediaInfo& media) {
			this->medias_.push_back(media);
		}

		MediaInfo* GetMedia(const string& mtype) {

			for (auto& media : this->medias_) {
				if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
					return &media;
				}
			}
			return nullptr;
		}

		MediaInfo * GetAudioMedia() {
			for (auto& media : this->medias_) {
				if (stringUtil::tolower(media.GetType()) == "audio") {
					return &media;
				}
			}
			return nullptr;
		}

		MediaInfo*  GetVideoMedia() {
			for (auto& media : this->medias_) {
				if (stringUtil::tolower(media.GetType()) == "video") {
					return &media;
				}
			}
			return nullptr;
		}

		vector<MediaInfo> GetMediasByType(const string& mtype)
		{
			vector<MediaInfo> medias;
			for (auto& media : this->medias_) {
				if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
					medias.push_back(media);
				}
			}
			return medias;
		}

		MediaInfo* GetMediaByID(const string& mid) {

			for (auto& media : this->medias_) {
				if (stringUtil::tolower(media.GetID()) == stringUtil::tolower(mid)) {
					return &media;
				}
			}
			return nullptr;
		}

		bool ReplaceMedia(const MediaInfo& media) {

			for (auto& rmedia : this->medias_) {
				if (rmedia.GetID() == media.GetID()) {
					rmedia = media;
					return true;
				}
			}
			return false;
		}

		const vector<MediaInfo>& GetMedias() const {
			return this->medias_;
		}

		int GetVersion() const {
			return this->version_;
		}

		const DTLSInfo* GetDTLS() const {
			return this->dtls_;
		}

		void SetDTLS(const DTLSInfo* const d) {
			if (d != nullptr) {
				if (dtls_ == nullptr) {
					this->dtls_ = new DTLSInfo();
				}
				*dtls_ = *d;
			}
			else {
				delete dtls_;
				dtls_ = nullptr;
			}
		}

		const ICEInfo* GetICE() const {
			return this->ice_;
		}

		void SetICE(const ICEInfo* i) {
			if (i != nullptr) {
				if (ice_ == nullptr) {
					this->ice_ = new ICEInfo();
				}
				*ice_ = *i;
			}
			else {
				delete ice_;
				ice_ = nullptr;
			}
		}

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

		TrackInfo* GetTrackByMediaID(const string& mid) {
			for (auto& stream : streams_) {
				for (auto& track : stream.second.tracks) {
					if (track.second.mediaID == mid) {
						return &track.second;
					}
				}
			}
			return nullptr;
		}

		StreamInfo* GetStreamByMediaID(const string& mid) {

			for (auto& stream : streams_) {
				for (auto& track : stream.second.tracks) {
					if (track.second.mediaID == mid) {
						return &stream.second;
					}
				}
			}
			return nullptr;
		}

		SDPInfo Answer(const ICEInfo* ice, const DTLSInfo* dtls, const vector<CandidateInfo>& candidates, const map<string, Capability>& medias) {
			SDPInfo sdpInfo;

			sdpInfo.SetICE(ice);
			sdpInfo.SetDTLS(dtls);

			sdpInfo.candidates_ = candidates;

			for (auto media : medias_) {
				map<string, Capability>::const_iterator it = medias.find(media.GetType());
				if (it != medias.end()) {
					sdpInfo.AddMedia(media.AnswerCapability(it->second));
				}
			}

			return sdpInfo;
		}

		string String() const {

			sdptransform::SdpStruct sdpMap;
			sdpMap.Version = 0;

			sdpMap.Origin = new sdptransform::OriginStruct();
			sdpMap.Origin->Username = "-";
			sdpMap.Origin->SessionId = std::to_string(time(nullptr));
			sdpMap.Origin->SessionVersion = this->version_;
			sdpMap.Origin->NetType = "IN";
			sdpMap.Origin->Address = "127.0.0.1";

			sdpMap.Connection = new sdptransform::ConnectionStruct({ 4, "0.0.0.0" });

			sdpMap.Name = "media";

			if (ice_ != nullptr && ice_->IsLite()) {
				sdpMap.Icelite = "ice-lite";
			}

			sdpMap.MsidSemantic = new sdptransform::MsidSemanticStruct({ "WMS", "*" });

			vector<string> bundleMids;

			for (auto& media : this->medias_) {
				sdptransform::MediaStruct mediaMap;
				mediaMap.Type = media.GetType();
				mediaMap.Port = 9;
				mediaMap.Protocal = "UDP/TLS/RTP/SAVP";
				mediaMap.Direction = Direction_String(media.GetDirection());
				mediaMap.RtcpMux = "rtcp-mux";
				mediaMap.RtcpRsize = "rtcp-rsize";
				mediaMap.Mid = media.GetID();
								
				bundleMids.push_back(media.GetID());

				if (media.GetBitrate() > 0) {
					mediaMap.Bandwidth.push_back(sdptransform::BandwithStruct({ "AS", media.GetBitrate() }));
				}

				for (auto& candidate : this->GetCandidates()) {
					sdptransform::CandidateStruct candi({
						candidate.foundation,
						candidate.componentID,
						candidate.transport,
						candidate.priority,
						candidate.address,
						candidate.port,
						candidate.ctype,
						candidate.relAddr,
						candidate.relPort });
					mediaMap.Candidates.push_back(candi);
				}

				mediaMap.IceUfrag = this->GetICE()->ufrag;
				mediaMap.IcePwd = this->GetICE()->password;

				mediaMap.Fingerprint = new sdptransform::FingerprintStruct({ dtls_->hash, dtls_->fingerprint });

				mediaMap.Setup = Setup_String(this->GetDTLS()->setup);

				for (auto& codec : media.GetCodecs()) {
					if ("video" == stringUtil::tolower(media.GetType())) {
						mediaMap.Rtp.push_back(sdptransform::RtpStruct({
							codec.second.ctype,
							stringUtil::toupper(codec.second.codec),
							90000,
							0 }));
					}
					else if ("opus" == stringUtil::tolower(codec.second.codec)) {
						mediaMap.Rtp.push_back(sdptransform::RtpStruct({
							codec.second.ctype,
							codec.second.codec,
							48000,
							2 }));
					}
					else {
						mediaMap.Rtp.push_back(sdptransform::RtpStruct({
							codec.second.ctype,
							codec.second.codec,
							8000,
							0 }));
					}

					for (auto& rtcpfb : codec.second.rtcpfbs) {
						mediaMap.RtcpFb.push_back(sdptransform::RtcpFbStruct({
							codec.second.ctype,
							rtcpfb.id,
							stringUtil::join(rtcpfb.params, " ") }));
					}

					if (codec.second.HasRTX()) {
						mediaMap.Rtp.push_back(sdptransform::RtpStruct({
							codec.second.rtx,
							"rtx",
							90000,
							0 }));
						mediaMap.Fmtp.push_back(sdptransform::FmtpStruct({
							codec.second.rtx,
							"apt=" + std::to_string(codec.second.ctype) }));
					}


					if (!codec.second.params.empty()) {
						sdptransform::FmtpStruct fmtp({ codec.second.ctype, "" });

						for (auto param : codec.second.params) {
							if (fmtp.Config != "") {
								fmtp.Config += ";";
							}

							// k and value
							if (param.second != "") {
								fmtp.Config += param.first + "=" + param.second;
							}
							else {
								fmtp.Config += param.first;
							}
						}

						mediaMap.Fmtp.push_back(fmtp);
					}
				}

				vector<int> payloads;
				for (auto& rtp : mediaMap.Rtp) {
					payloads.push_back(rtp.Payload);
				}
				mediaMap.Payloads = stringUtil::digitArrayToString(payloads, " ");

				for (auto ext : media.GetExtensions()) {
					mediaMap.Ext.push_back(sdptransform::ExtStruct({ ext.first, ext.second }));
				}

				for (auto& ridInfo : media.GetRIDS()) {
					sdptransform::RidStruct rid({
						ridInfo.second.id,
						DirectionWay_String(ridInfo.second.direction),
						"" });

					if (!ridInfo.second.params.empty()) {
						//rid.Params = "pt=" + strings.Join(ridInfo.GetFormats(), ",")
						rid.Params = "pt=" + stringUtil::digitArrayToString(ridInfo.second.formats, ",");
					}

					for (auto param : ridInfo.second.params) {
						if (rid.Params == "") {
							rid.Params = param.first + "=" + param.second;
						}
						else {
							rid.Params = rid.Params + ";" + param.first + "=" + param.second;
						}
					}

					mediaMap.Rids.push_back(rid);
				}

				const SimulcastInfo* simulcast = media.GetSimulcastInfo();
				if (simulcast != nullptr) {
					int index = 1;

					const SimulcastStreamInfoss& sendStreams = simulcast->GetSimulcastStreams(SEND);
					const SimulcastStreamInfoss& recvStreams = simulcast->GetSimulcastStreams(RECV);

					if (!sendStreams.empty()) {
						string list = "";
						for (auto& stream : sendStreams) {
							string alternatives;
							for (auto& item : stream) {
								if (alternatives == "") {
									alternatives += item.IsPaused() ? "~" : "" + item.id;
								}
								else {
									alternatives += "," + item.IsPaused() ? "~" : "" + item.id;
								}
							}

							list += (list == "") ? "" : ";" + alternatives;
						}
						if (mediaMap.Simulcast == nullptr) {
							mediaMap.Simulcast = new sdptransform::SimulcastStruct();
						}
						mediaMap.Simulcast->Dir1 = "send";
						mediaMap.Simulcast->List1 = list;
						++index;
					}

					if (!recvStreams.empty()) {
						string list;
						for (auto& stream : recvStreams) {
							string alternatives;
							for (auto& item : stream) {
								if (alternatives == "") {
									alternatives += item.IsPaused() ? "~" : "" + item.id;
								}
								else {
									alternatives = "," + item.IsPaused() ? "~" : "" + item.id;
								}
							}

							list += (list == "") ? "" : ";" + alternatives;
						}

						if (mediaMap.Simulcast == nullptr) {
							mediaMap.Simulcast = new sdptransform::SimulcastStruct();
						}
						if (index == 1) {
							mediaMap.Simulcast->Dir1 = "recv";
							mediaMap.Simulcast->List1 = list;
						}
						else if (index == 2) {
							mediaMap.Simulcast->Dir2 = "recv";
							mediaMap.Simulcast->List2 = list;
						}
					}
				}

				sdpMap.Media.push_back(mediaMap);
			}

			// streams
			for (auto& stream : this->GetStreams()) {
				for (auto& track : stream.second.tracks) {
					for (auto& md : sdpMap.Media) {
						// check if it is unified or plan b
						if (track.second.mediaID != "") {
							if (track.second.mediaID == md.Mid) {
								for (auto& group : track.second.groups) {
									md.SsrcGroups.push_back(sdptransform::SsrcGroupStruct({
										group.semantics,
										stringUtil::digitArrayToString(group.ssrcs, " ")
															}));
								}

								for (auto& ssrc : track.second.ssrcs) {
									md.Ssrcs.push_back(sdptransform::SsrcStruct({
										ssrc,
										"cname",
										stream.second.id
													   }));
									md.Ssrcs.push_back(sdptransform::SsrcStruct({
										ssrc,
										"msid",
										stream.second.id + " " + track.second.id
													   }));
								}
								md.Msid = stream.second.id + " " + track.second.id;

								break;
							}
						}
						else if (stringUtil::tolower(md.Type) == stringUtil::tolower(track.second.media)) {
							for (auto& group : track.second.groups) {
								md.SsrcGroups.push_back(sdptransform::SsrcGroupStruct({
									group.semantics,
									stringUtil::digitArrayToString(group.ssrcs, " ") }));
							}

							for (auto& ssrc : track.second.ssrcs) {
								md.Ssrcs.push_back(sdptransform::SsrcStruct({
									ssrc,
									"cname",
									stream.second.id }));
								md.Ssrcs.push_back(sdptransform::SsrcStruct({
									ssrc,
									"msid",
									stream.second.id + " " + track.second.id }));
							}
							break;
						}
					}
				}
			}
						
			sdpMap.Groups.push_back(sdptransform::GroupStruct({
				stringUtil::join(bundleMids, " "),
				"BUNDLE" }));

			json j = sdpMap;
			return sdptransform::write(j);
		}

	#if 0
		// Unify return an unified plan version of the SDP info;  ......
		func(s *SDPInfo) Unify() *SDPInfo {
		cloned: = NewSDPInfo()

			cloned.version = this->version

			for _, media : = range this->medias {
				cloned.AddMedia(media.Clone())
			}

			medias: = map[string][] * MediaInfo{
				"audio": cloned.GetMediasByType("audio"),
				"video" : cloned.GetMediasByType("video"),
			}

			for _, stream : = range this->streams {
			clonedStream: = stream.Clone()
				for _, clonedTrack : = range clonedStream.GetTracks() {
					var clonedMedia *MediaInfo
						if len(medias[clonedTrack.GetMedia()]) == 0 {
						media: = this->GetMedia(clonedTrack.GetMedia())
							clonedMedia = media.Clone()
							clonedMedia.SetID(clonedTrack.GetID())
							cloned.AddMedia(clonedMedia)
						}
						else {
						mediaList: = medias[clonedTrack.GetMedia()]
							clonedMedia = mediaList[len(mediaList) - 1]
							mediaList = mediaList[:len(mediaList) - 1]
							medias[clonedTrack.GetMedia()] = mediaList
						}
						clonedTrack.SetMediaID(clonedMedia.GetID())
				}
						  cloned.AddStream(clonedStream)
			}

					for _, candidate : = range this->GetCandidates() {
						cloned.AddCandidate(candidate.Clone())
					}

					cloned.SetICE(this->GetICE().Clone())
						cloned.SetDTLS(this->GetDTLS().Clone())

						return cloned
		}
	#endif

		SDPInfo* Parse(const string& sdp) {
			sdptransform::SdpStruct sdpMap;
			try {
				sdpMap = sdptransform::parse(sdp).get<sdptransform::SdpStruct>();
			}
			catch (const std::exception& e)
			{	
				cout << string("Exception: ") + e.what() + "\r\n";				
				cout << sdptransform::parse(sdp).dump() + "\r\n";
				return nullptr;
			}

			SDPInfo* sdpInfo = new SDPInfo();
			sdpInfo->SetVersion(sdpMap.Version);

			for (auto& md : sdpMap.Media) {
				string media = md.Type;;
				string mid = md.Mid;
							
				MediaInfo mediaInfo(mid, md.Type);

				sdpInfo->SetICE(&ICEInfo(md.IceUfrag, md.IcePwd));

				for (auto candiate : md.Candidates) {
					sdpInfo->AddCandidate(CandidateInfo(
						candiate.Foundation,
						candiate.Component,
						candiate.Transport,
						candiate.Priority,
						candiate.Ip,
						candiate.Port,
						candiate.Type,
						candiate.Raddr,
						candiate.Rport)
					);
				}

				sdptransform::FingerprintStruct* fingerpirnt = nullptr;
				if (sdpMap.Fingerprint != nullptr) {
					fingerpirnt = sdpMap.Fingerprint;
				}
				if (md.Fingerprint != nullptr) {
					fingerpirnt = md.Fingerprint;
				}
				string setup = (!md.Setup.empty()) ? Setup_ByValue(md.Setup) : SETUPACTPASS;
				DTLSInfo dtls({ setup, fingerpirnt ? fingerpirnt->Type : "", fingerpirnt ? fingerpirnt->Hash : "" });
				sdpInfo->SetDTLS(&dtls);

				mediaInfo.SetDirection((md.Direction != "") ? Direction_ByValue(md.Direction) : SENDRECV);

				map<int, int> apts;
				for (auto& fmt : md.Rtp) {
					int payload = fmt.Payload;
					string codec = fmt.Codec;

					if ("RED" == stringUtil::toupper(codec) || "ULPFEC" == stringUtil::toupper(codec)) {
						continue;
					}

					map<string, string> params;
					for (auto& fmtp : md.Fmtp) {
						if (fmtp.Payload == payload) {
							for (auto& kv : stringUtil::split(fmtp.Config, ";")) {
								vector<string> param = stringUtil::split(kv, "=");
								if (param.size() < 2) {
									continue;
								}
								params[param[0]] = param[1];
							}
						}
					}

					if ("rtx" == stringUtil::tolower(codec)) {
						if (params.find("apt") != params.end()) {
							try {
								apts[std::atoi(params["apt"].c_str())] = payload;
							}
							catch (...) {
							}
						}
					}
					else {
						CodecInfo codecInfo(codec, payload);
						codecInfo.AddParams(params);
						mediaInfo.AddCodec(codecInfo);
					}
				}

				// rtx
				for (auto pt : apts) {
					CodecInfo* codecInfo = mediaInfo.CodecForType(pt.first);
					if (codecInfo != nullptr) {
						codecInfo->rtx = pt.second;
					}
				}

				// rtcpFb
				if (!md.RtcpFb.empty()) {
					for (auto& rtcfb : md.RtcpFb) {
						CodecInfo* codecInfo = mediaInfo.CodecForType(rtcfb.Payload);
						if (codecInfo != nullptr) {
							vector<string> params;
							if (rtcfb.Subtype != "") {
								params = stringUtil::split(rtcfb.Subtype, " ");
							}
							codecInfo->AddRTCPFeedback(RtcpFeedbackInfo(rtcfb.Type, params));
						}
					}
				}

				// extmap
				for (auto& extmap : md.Ext) {
					mediaInfo.AddExtension(extmap.Value, extmap.Uri);
				}

				for (auto& rid : md.Rids) {
					RIDInfo ridInfo(rid.Id, DirectionWay_ByValue(rid.Direction));

					vector<string> formats;
					map<string, string> params;

					if (rid.Params != "") {
						// nlohmann::json: STL-like access
						for (auto l : sdptransform::parseParams(rid.Params).items()) {
							if (l.key() == "pt") {
								formats = stringUtil::split(l.value().get<std::string>(), ",");
							}
							else {
								params[l.key()] = l.value().get<std::string>();
							}
						}
						ridInfo.SetFormats(formats);
						ridInfo.params = params;
					}

					mediaInfo.AddRID(ridInfo);
				}

				vector<vector<TrackEncodingInfo> > encodings;
				if (md.Simulcast != nullptr) {
					SimulcastInfo simulcast;

					if (md.Simulcast->Dir1 != "") {
						vector<vector<SimulcastStreamInfo>> tei =
							sdptransform::parseSimulcastStreamList(md.Simulcast->List1).get<vector<vector<SimulcastStreamInfo> > >();
						for (auto& streams : tei) {							
							simulcast.AddSimulcastAlternativeStreams(DirectionWay_ByValue(md.Simulcast->Dir1), streams);
						}
					}

					if (md.Simulcast->Dir2 != "") {
						vector<vector<SimulcastStreamInfo>> tei =
							sdptransform::parseSimulcastStreamList(md.Simulcast->List2).get<vector<vector<SimulcastStreamInfo> > >();
						for (auto& streams : tei) {
							simulcast.AddSimulcastAlternativeStreams(DirectionWay_ByValue(md.Simulcast->Dir2), streams);
						}
					}

					// For all sending encodings
					for (auto& streams : simulcast.GetSimulcastStreams(SEND)) {
						vector<TrackEncodingInfo> alternatives;
						for (auto& stream : streams) {
							TrackEncodingInfo encoding(stream.id, stream.IsPaused());
							RIDInfo* ridInfo = mediaInfo.GetRID(encoding.id);
							if (ridInfo != nullptr) {
								//Get associated payloads								
								for (auto format : ridInfo->formats) {
									const CodecInfo* codecInfo = mediaInfo.GetCodecForType(format);
									if (codecInfo != nullptr) {
										encoding.AddCodec(*codecInfo);
									}
								}
								encoding.params = ridInfo->params;
								alternatives.push_back(encoding);
							}
						}

						if (!alternatives.empty()){
							encodings.push_back(alternatives);
						}
					}

					mediaInfo.SetSimulcastInfo(&simulcast);
				}

				map<uint64_t, SourceInfo> sources;
				if (!md.Ssrcs.empty()) {
					for (auto& ssrcAttr : md.Ssrcs) {
						uint64_t& ssrc = ssrcAttr.Id;
						string& key = ssrcAttr.Attribute;
						string& value = ssrcAttr.Value;
												
						if (sources.find(ssrc) == sources.end()) {														
							sources[ssrc] = SourceInfo(ssrc);						
						}						
						SourceInfo* source = &sources[ssrc];
						
						if (stringUtil::tolower(key) == "cname") {
							source->cname = value;
						}
						else if (stringUtil::tolower(key) == "msid") {
							vector<string> ids = stringUtil::split(value, " ");
							// get stream id and track id
							string& streamId = ids[0];
							string& trackId = ids[1];

							source->streamID = streamId;
							source->trackID = trackId;

							auto* stream = sdpInfo->GetStream(streamId);
							if (stream == nullptr) {
								sdpInfo->AddStream(StreamInfo(streamId));
								stream = sdpInfo->GetStream(streamId);
								assert(stream != nullptr);
							}

							TrackInfo* track = stream->GetTrack(trackId);
							if (track == nullptr) {
								stream->AddTrack(TrackInfo(trackId, media));
								track = stream->GetTrack(trackId);
								assert(track != nullptr);
								track->mediaID = mid;
								track->encodings = encodings;
							}
							// Add ssrc
							track->AddSSRC(ssrc);
						}
					}
				}

				// Check if ther is a global msid
				// Why this?
				if (md.Msid != "") {
					vector<string> ids = stringUtil::split(md.Msid, " ");
					string& streamId = ids[0];
					string& trackId = ids[1];

					StreamInfo* stream = sdpInfo->GetStream(streamId);
					if (stream == nullptr) {
						sdpInfo->AddStream(StreamInfo(streamId));
						stream = sdpInfo->GetStream(streamId);
					}

					TrackInfo* track = stream->GetTrack(trackId);
					if (track == nullptr) {
						stream->AddTrack(TrackInfo(trackId, media));
						track = stream->GetTrack(trackId);
						track->media = mid;
						track->encodings = encodings;
					}

					for (auto s : sources) {
						if (s.second.streamID == "") {
							s.second.streamID = streamId;
							s.second.trackID = trackId;
							track->AddSSRC(s.first);
						}
					}
				}

				for (auto s : sources) {
					auto& ssrc = s.first;
					auto& source = s.second;

					if (source.streamID == "") {
						string streamId = source.cname;
						string trackId = mid;

						source.streamID = streamId;
						source.trackID = trackId;

						StreamInfo* stream = sdpInfo->GetStream(streamId);
						if (stream == nullptr) {
							sdpInfo->AddStream(StreamInfo(streamId));
							stream = sdpInfo->GetStream(streamId);
						}

						TrackInfo* track = stream->GetTrack(trackId);
						if (track == nullptr) {
							stream->AddTrack(TrackInfo(trackId, media));
							track = stream->GetTrack(trackId);
							track->mediaID = mid;
							track->encodings = encodings;
						}
						track->AddSSRC(ssrc);
					}
				}

				if (!md.SsrcGroups.empty()) {
					for (auto& ssrcGroupAttr : md.SsrcGroups) {
						vector<string> ssrcs = stringUtil::split(ssrcGroupAttr.Ssrcs, " ");
						vector<int> ssrcsint;
						for (auto& ssrcstr : ssrcs) {
							try {
								ssrcsint.push_back(std::atoi(ssrcstr.c_str()));
							}
							catch (...) {}
						}
						if (ssrcsint.empty()) {
							continue;
						}

						auto& source = sources[ssrcsint[0]];
						StreamInfo* streamInfo = sdpInfo->GetStream(source.streamID);
						if (streamInfo != nullptr && streamInfo->GetTrack(source.trackID) != nullptr) {
							streamInfo->GetTrack(source.trackID)->AddSourceGroup(SourceGroupInfo(ssrcGroupAttr.Semantics, ssrcsint));
						}
					}
				}

				sdpInfo->AddMedia(mediaInfo);
			}

			return sdpInfo;
		}
	};
}