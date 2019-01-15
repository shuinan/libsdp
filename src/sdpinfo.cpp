//#pragma once

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

#include <nlohmann/json.hpp>

#ifdef USE_JSON_TRANSFORM
#include "sdpstruct.h"
#endif

#include "../libsdptransform/include/sdptransform.hpp"

using json = nlohmann::json;

#include "stringUtil.h"

#include "sdpinfo.h"

using namespace std;


double defaultValue(double) { return 0; }
int defaultValue(int) { return 0; }
uint64_t defaultValue(const uint64_t) { return 0; }
string defaultValue(const string&) { return ""; }
template<class T> T omitemptyJson(const json& j, const string& name) {
	try {
		return j.at(name).get<T>();
	}
	catch (const std::exception&) {
		return defaultValue(T());
	}
}

bool jsonExist(const json& j, const string& name) {
	return j.find(name) != j.end();
}

namespace sdp {

	SDPInfo::SDPInfo() : ice_(nullptr), dtls_(nullptr) {}
	SDPInfo::SDPInfo(ICEInfo* ice, DTLSInfo* dtls, const vector<CandidateInfo>& candidates, const map<string, Capability>& capabilities)
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
	SDPInfo& SDPInfo::operator=(const SDPInfo& sdp) {
		version_ = sdp.GetVersion();
		streams_ = sdp.GetStreams();
		medias_ = sdp.GetMedias();
		candidates_ = sdp.GetCandidates();
		SetICE(this->GetICE());
		SetDTLS(this->GetDTLS());
		return *this;
	}

	SDPInfo::SDPInfo(const SDPInfo& sdp) {
		*this = sdp;
	}
	SDPInfo::~SDPInfo(void) {
		DeleteObj(ice_);
		DeleteObj(dtls_);
	}


	MediaInfo* SDPInfo::GetMedia(const string& mtype) {
		for (auto& media : this->medias_) {
			if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
				return &media;
			}
		}
		return nullptr;
	}

	MediaInfo * SDPInfo::GetAudioMedia() {
		for (auto& media : this->medias_) {
			if (stringUtil::tolower(media.GetType()) == "audio") {
				return &media;
			}
		}
		return nullptr;
	}

	MediaInfo*  SDPInfo::GetVideoMedia() {
		for (auto& media : this->medias_) {
			if (stringUtil::tolower(media.GetType()) == "video") {
				return &media;
			}
		}
		return nullptr;
	}

	vector<MediaInfo> SDPInfo::GetMediasByType(const string& mtype)
	{
		vector<MediaInfo> medias;
		for (auto& media : this->medias_) {
			if (stringUtil::tolower(media.GetType()) == stringUtil::tolower(mtype)) {
				medias.push_back(media);
			}
		}
		return medias;
	}

	MediaInfo* SDPInfo::GetMediaByID(const string& mid) {

		for (auto& media : this->medias_) {
			if (stringUtil::tolower(media.GetID()) == stringUtil::tolower(mid)) {
				return &media;
			}
		}
		return nullptr;
	}

	bool SDPInfo::ReplaceMedia(const MediaInfo& media) {
		for (auto& rmedia : this->medias_) {
			if (rmedia.GetID() == media.GetID()) {
				rmedia = media;
				return true;
			}
		}
		return false;
	}


	void SDPInfo::SetDTLS(const DTLSInfo* const d) {
		utils::setPointerData(this->dtls_, d);
	}

	void SDPInfo::SetICE(const ICEInfo* d) {
		utils::setPointerData(this->ice_, d);
	}


	TrackInfo* SDPInfo::GetTrackByMediaID(const string& mid) {
		for (auto& stream : streams_) {
			for (auto& track : stream.second.tracks) {
				if (track.second.mediaID == mid) {
					return &track.second;
				}
			}
		}
		return nullptr;
	}

	StreamInfo* SDPInfo::GetStreamByMediaID(const string& mid) {

		for (auto& stream : streams_) {
			for (auto& track : stream.second.tracks) {
				if (track.second.mediaID == mid) {
					return &stream.second;
				}
			}
		}
		return nullptr;
	}

	SDPInfo SDPInfo::Answer(const ICEInfo* ice, const DTLSInfo* dtls, const vector<CandidateInfo>& candidates, const map<string, Capability>& medias) {
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


	string SDPInfo::String() const {
		json sdpMap;
		sdpMap["version"] = 0;

		sdpMap["origin"]["username"] = "-";
		sdpMap["origin"]["sessionId"] = std::to_string(time(nullptr));
		sdpMap["origin"]["sessionVersion"] = this->version_;
		sdpMap["origin"]["netType"] = "IN";
		sdpMap["origin"]["address"] = "127.0.0.1";

		sdpMap["connection"]["version"] = 4;
		sdpMap["connection"]["ip"] = "0.0.0.0";

		sdpMap["name"] = "media";

		if (ice_ != nullptr && ice_->IsLite()) {
			sdpMap["icelite"] = "ice-lite";
		}

		sdpMap["timing"] = { { "start", 0 },{ "stop", 0 } };

		sdpMap["msidSemantic"]["semantic"] = "WMS";
		sdpMap["msidSemantic"]["token"] = "*";

		vector<string> bundleMids;

		for (auto& media : this->medias_) {
			json mediaMap;
			mediaMap["type"] = media.GetType();
			mediaMap["port"] = 9;
			mediaMap["protocal"] = "UDP/TLS/RTP/SAVP";
			mediaMap["direction"] = Direction_String(media.GetDirection());
			mediaMap["rtcpMux"] = "rtcp-mux";
			mediaMap["rtcpRsize"] = "rtcp-rsize";
			mediaMap["mid"] = media.GetID();

			bundleMids.push_back(media.GetID());

			if (media.GetBitrate() > 0) {
				mediaMap["bandwidth"].push_back({ { "type", "AS" },{ "limit", media.GetBitrate() } });
			}

			mediaMap["candidates"] = GetCandidates();
			mediaMap["iceUfrag"] = this->GetICE()->ufrag;
			mediaMap["icePwd"] = this->GetICE()->password;
			mediaMap["fingerprint"] = { { "type", dtls_->hash },{ "hash", dtls_->fingerprint } };
			mediaMap["setup"] = Setup_String(this->GetDTLS()->setup);


			for (auto& codec : media.GetCodecs()) {
				if ("video" == stringUtil::tolower(media.GetType())) {
					mediaMap["rtp"].push_back({
						{ "payload", codec.second.ctype },
											  { "codec",	stringUtil::toupper(codec.second.codec) },
											  { "rate",	90000 },
											  { "encoding",0 } });
				}
				else {
					if ("opus" == stringUtil::tolower(codec.second.codec)) {
						mediaMap["rtp"].push_back({
							{ "payload", codec.second.ctype },
												  { "codec",	codec.second.codec },
												  { "rate",	48000 },
												  { "encoding",2 } });
					}
					else {
						mediaMap["rtp"].push_back({
							{ "payload", codec.second.ctype },
												  { "codec",	codec.second.codec },
												  { "rate",	8000 },
												  { "encoding",0 } });
					}
				}

				for (auto& rtcpfb : codec.second.rtcpfbs) {
					mediaMap["rtcpFb"].push_back({
						{ "payload", codec.second.ctype },
												 { "type", rtcpfb.id },
												 { "subtype", stringUtil::join(rtcpfb.params, " ") } });
				}

				if (codec.second.HasRTX()) {
					mediaMap["rtp"].push_back({
						{ "payload", codec.second.rtx },
											  { "codec",	"rtx" },
											  { "rate",	90000 },
											  { "encoding",0 } });
					mediaMap["fmtp"].push_back({
						{ "payload", codec.second.rtx },
											   { "config", "apt=" + std::to_string(codec.second.ctype) } });
				}


				if (!codec.second.params.empty()) {
					string config;

					for (auto param : codec.second.params) {
						if (config != "") {
							config += ";";
						}

						// k and value
						if (param.second != "") {
							config += param.first + "=" + param.second;
						}
						else {
							config += param.first;
						}
					}

					mediaMap["fmtp"].push_back({
						{ "payload", codec.second.ctype },
											   { "config", config } });
				}
			}

			vector<int> payloads;
			for (auto& rtp : mediaMap["rtp"]) {
				payloads.push_back(rtp["payload"]);
			}
			mediaMap["payloads"] = stringUtil::digitArrayToString(payloads, " ");

			for (auto ext : media.GetExtensions()) {
				mediaMap["ext"].push_back({ { "value", ext.first },{ "uri", ext.second } });
			}

			mediaMap["rids"] = media.GetRIDS();

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

					mediaMap["simulcast"]["dir1"] = "send";
					mediaMap["simulcast"]["list1"] = list;
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

					if (index == 1) {
						mediaMap["simulcast"]["dir1"] = "recv";
						mediaMap["simulcast"]["list1"] = list;
					}
					else if (index == 2) {
						mediaMap["simulcast"]["dir2"] = "recv";
						mediaMap["simulcast"]["list2"] = list;
					}
				}
			}

			sdpMap["media"].push_back(mediaMap);
		}

		// streams
		for (auto& stream : this->GetStreams()) {
			for (auto& track : stream.second.tracks) {
				for (auto& md : sdpMap["media"]) {
					// check if it is unified or plan b
					if (track.second.mediaID != "") {
						if (track.second.mediaID == md["mid"]) {
							for (auto& group : track.second.groups) {
								md["ssrcGroups"].push_back({
									{ "semantics", group.semantics },
														   { "ssrcs"	, stringUtil::digitArrayToString(group.ssrcs, " ") } });
							}

							for (auto& ssrc : track.second.ssrcs) {
								md["ssrcs"].push_back({
									{ "id"		, ssrc },
													  { "attribute", "cname" },
													  { "value"	, stream.second.id } });
								md["ssrcs"].push_back({
									{ "id"		,ssrc },
													  { "attribute","msid" },
													  { "value"	,stream.second.id + " " + track.second.id } });
							}
							md["msid"] = stream.second.id + " " + track.second.id;

							break;
						}
					}
					else if (stringUtil::tolower(md["type"]) == stringUtil::tolower(track.second.media)) {
						for (auto& group : track.second.groups) {
							md["ssrcGroups"].push_back({
								{ "semantics", group.semantics },
													   { "ssrcs"	, stringUtil::digitArrayToString(group.ssrcs, " ") } });
						}

						for (auto& ssrc : track.second.ssrcs) {
							md["ssrcs"].push_back({
								{ "id"		,ssrc },
												  { "attribute","cname" },
												  { "value"	,stream.second.id } });
							md["ssrcs"].push_back({
								{ "id"		,ssrc },
												  { "attribute","msid" },
												  { "value"	,stream.second.id + " " + track.second.id } });
						}
						break;
					}
				}
			}
		}

		sdpMap["groups"].push_back({
			{ "mids", stringUtil::join(bundleMids, " ") },
								   { "type", "BUNDLE" } });

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


	bool SDPInfo::Parse(const string& sdp) {
		json sdpMap;
		try {
			sdpMap = sdptransform::parse(sdp);
		}
		catch (const std::exception& e)
		{
			cout << string("Exception: ") + e.what() + "\r\n";
			cout << sdptransform::parse(sdp).dump() + "\r\n";
			return false;
		}
				
		this->SetVersion(omitemptyJson<int>(sdpMap, "version"));

		for (auto& md : sdpMap.at("media")) {
			string media = omitemptyJson<string>(md, "type");
			string mid = omitemptyJson<string>(md, "mid");

			MediaInfo mediaInfo(mid, media);

			ICEInfo ii(omitemptyJson<string>(md, "iceUfrag"), omitemptyJson<string>(md, "icePwd"));
			this->SetICE(&ii);
			if (jsonExist(md, "candidates")) {
				this->candidates_ = md["candidates"].get<vector<CandidateInfo> >();
			}
			string fp_type, fp_hash;
			if (sdpMap.find("fingerprint") != sdpMap.end()) {
				fp_type = omitemptyJson<string>(sdpMap["fingerprint"], "type");
				fp_hash = omitemptyJson<string>(sdpMap["fingerprint"], "hash");
			}
			if (jsonExist(md, "fingerprint")) {
				fp_type = omitemptyJson<string>(md["fingerprint"], "type");
				fp_hash = omitemptyJson<string>(md["fingerprint"], "hash");
			}
			string setup = (md.find("setup") != md.end()) ? omitemptyJson<string>(md, "Setup") : "";
			setup = setup.empty() ? SETUPACTPASS : Setup_ByValue(setup);
			DTLSInfo dtls({ setup, fp_type, fp_hash });
			this->SetDTLS(&dtls);

			string direction = (md.find("direction") != md.end()) ? md.at("direction") : "";
			mediaInfo.SetDirection(direction.empty() ? SENDRECV : Direction_ByValue(direction));

			map<int, int> apts;
			if (md.find("rtp") != md.end()) {
				for (auto& fmt : md.at("rtp")) {
					int payload = fmt.at("payload");
					string codec = omitemptyJson<string>(fmt, "codec");
					if ("RED" == stringUtil::toupper(codec) || "ULPFEC" == stringUtil::toupper(codec)) {
						continue;
					}

					map<string, string> params;
					if (md.find("fmtp") != md.end()) {
						for (auto& fmtp : md.at("fmtp")) {
							if (fmtp.at("payload") == payload) {
								if (jsonExist(fmtp, "config")) {
									for (auto& kv : stringUtil::split(fmtp.at("config"), ";")) {
										vector<string> param = stringUtil::split(kv, "=");
										if (param.size() < 2) {
											continue;
										}
										params[param[0]] = param[1];
									}
								}
							}
						}
					}

					if ("rtx" == stringUtil::tolower(codec)) {
						if (params.find("apt") != params.end()) {
							try {
								apts[std::atoi(params.at("apt").c_str())] = payload;
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
			}

			// rtx
			for (auto pt : apts) {
				CodecInfo* codecInfo = mediaInfo.CodecForType(pt.first);
				if (codecInfo != nullptr) {
					codecInfo->rtx = pt.second;
				}
			}

			// rtcpFb
			if (md.find("rtcpFb") != md.end()) {
				for (auto& rtcfb : md.at("rtcpFb")) {
					CodecInfo* codecInfo = mediaInfo.CodecForType(rtcfb.at("payload"));
					if (codecInfo != nullptr) {
						vector<string> params;
						if (rtcfb.find("subtype") != rtcfb.end()) {
							params = stringUtil::split(rtcfb.at("subtype"), " ");
						}
						codecInfo->AddRTCPFeedback(RtcpFeedbackInfo(omitemptyJson<string>(rtcfb, "type"), params));
					}
				}
			}

			// extmap
			if (jsonExist(md, "ext")) {
				for (auto& extmap : md.at("ext")) {
					mediaInfo.AddExtension(extmap.at("value"), extmap.at("uri"));
				}
			}

			if (jsonExist(md, "rids")) {
				for (auto i = md["rids"].begin(); i != md["rids"].end(); ++i) {
					mediaInfo.AddRID(i.value().get<RIDInfo>());
				}
			}

			vector<vector<TrackEncodingInfo> > encodings;
			if (md.find("simulcast") != md.end()) {
				SimulcastInfo simulcast;

				if (jsonExist(md.at("simulcast"), "dir1")) {
					vector<vector<SimulcastStreamInfo>> tei =
						sdptransform::parseSimulcastStreamList(md.at("simulcast").at("list1")).get<vector<vector<SimulcastStreamInfo> > >();
					for (auto& streams : tei) {
						simulcast.AddSimulcastAlternativeStreams(DirectionWay_ByValue(md.at("simulcast").at("dir1")), streams);
					}
				}

				if (jsonExist(md.at("simulcast"), "dir2")) {
					vector<vector<SimulcastStreamInfo>> tei =
						sdptransform::parseSimulcastStreamList(md.at("simulcast").at("list2")).get<vector<vector<SimulcastStreamInfo> > >();
					for (auto& streams : tei) {
						simulcast.AddSimulcastAlternativeStreams(DirectionWay_ByValue(md.at("simulcast").at("dir2")), streams);
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

					if (!alternatives.empty()) {
						encodings.push_back(alternatives);
					}
				}

				mediaInfo.SetSimulcastInfo(&simulcast);
			}

			map<uint64_t, SourceInfo> sources;
			if (md.find("ssrcs") != md.end()) {
				for (auto& ssrcAttr : md.at("ssrcs")) {
					string key = omitemptyJson<string>(ssrcAttr, "attribute");
					uint64_t ssrc = omitemptyJson<uint64_t>(ssrcAttr, "id");
					string value = omitemptyJson<string>(ssrcAttr, "value");

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

						auto* stream = this->GetStream(streamId);
						if (stream == nullptr) {
							this->AddStream(StreamInfo(streamId));
							stream = this->GetStream(streamId);
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
			if (md.find("msid") != md.end()) {
				vector<string> ids = stringUtil::split(md.at("msid"), " ");
				string& streamId = ids[0];
				string& trackId = ids[1];

				StreamInfo* stream = this->GetStream(streamId);
				if (stream == nullptr) {
					this->AddStream(StreamInfo(streamId));
					stream = this->GetStream(streamId);
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

					StreamInfo* stream = this->GetStream(streamId);
					if (stream == nullptr) {
						this->AddStream(StreamInfo(streamId));
						stream = this->GetStream(streamId);
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

			if (md.find("ssrcGroups") != md.end()) {
				for (auto& ssrcGroupAttr : md.at("ssrcGroups")) {
					vector<string> ssrcs = stringUtil::split(omitemptyJson<string>(ssrcGroupAttr, "ssrcs"), " ");
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
					StreamInfo* streamInfo = this->GetStream(source.streamID);
					if (streamInfo != nullptr && streamInfo->GetTrack(source.trackID) != nullptr) {
						streamInfo->GetTrack(source.trackID)->AddSourceGroup(SourceGroupInfo(omitemptyJson<string>(ssrcGroupAttr, "semantics"), ssrcsint));
					}
				}
			}

			this->AddMedia(mediaInfo);
		}

		return true;
	}

#ifdef USE_JSON_TRANSFORM	
	//////////////////////ToString, ParseFrom using json/struct transform ////////////////////////////
	string SDPInfo::ToString() const {

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
					candidate.component,
					candidate.transport,
					candidate.priority,
					candidate.ip,
					candidate.port,
					candidate.type,
					candidate.raddr,
					candidate.rport });
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

				if (!ridInfo.second.formats.empty()) {
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

	bool SDPInfo::ParseFrom(const string& sdp) {
		sdptransform::SdpStruct sdpMap;
		try {
			sdpMap = sdptransform::parse(sdp).get<sdptransform::SdpStruct>();
		}
		catch (const std::exception& e)
		{
			cout << string("Exception: ") + e.what() + "\r\n";
			cout << sdptransform::parse(sdp).dump() + "\r\n";
			return false;
		}
				
		this->SetVersion(sdpMap.Version);

		for (auto& md : sdpMap.Media) {
			string media = md.Type;;
			string mid = md.Mid;

			MediaInfo mediaInfo(mid, md.Type);

			this->SetICE(&ICEInfo(md.IceUfrag, md.IcePwd));

			for (auto candiate : md.Candidates) {
				this->AddCandidate(CandidateInfo(
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
			this->SetDTLS(&dtls);

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

					if (!alternatives.empty()) {
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

						auto* stream = this->GetStream(streamId);
						if (stream == nullptr) {
							this->AddStream(StreamInfo(streamId));
							stream = this->GetStream(streamId);
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

				StreamInfo* stream = this->GetStream(streamId);
				if (stream == nullptr) {
					this->AddStream(StreamInfo(streamId));
					stream = this->GetStream(streamId);
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

					StreamInfo* stream = this->GetStream(streamId);
					if (stream == nullptr) {
						this->AddStream(StreamInfo(streamId));
						stream = this->GetStream(streamId);
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
					StreamInfo* streamInfo = this->GetStream(source.streamID);
					if (streamInfo != nullptr && streamInfo->GetTrack(source.trackID) != nullptr) {
						streamInfo->GetTrack(source.trackID)->AddSourceGroup(SourceGroupInfo(ssrcGroupAttr.Semantics, ssrcsint));
					}
				}
			}

			this->AddMedia(mediaInfo);
		}

		return true;
	}
#endif
}