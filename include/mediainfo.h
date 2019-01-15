#pragma once

#include <string>

#include "direction.h"
#include "codecinfo.h"
#include "ridinfo.h"
#include "simulcastinfo.h"
#include "capability.h"

#include "stringUtil.h"
#include "utils.h"

using namespace std;

namespace sdp {

	struct  MediaInfo {
		string					id;
		string					mtype;			// "audio" | "video"
		Direction				direction;
		map<int, string>		extensions;		// Add rtp header extension support
		map<int, CodecInfo>		codecs;			// key: pt   value:  codec info
		map<string, RIDInfo>	rids;
		bool					simulcast;
		SimulcastInfo*			simulcastInfo;
		int						bitrate;


		MediaInfo& operator=(const MediaInfo& other) {
			id = other.id;
			mtype = other.mtype;
			direction = other.direction;
			extensions = other.extensions;
			codecs = other.codecs;
			rids = other.rids;
			simulcast = other.simulcast;
			simulcastInfo = nullptr;
			utils::setPointerData(simulcastInfo, other.simulcastInfo);
			bitrate = other.bitrate;
			return *this;
		}
		MediaInfo(const MediaInfo &other) {
			*this = other;
		}
		~MediaInfo(void) {
			DeleteObj(simulcastInfo);
		}		

		MediaInfo() :
			direction(SENDRECV),
			simulcast(false),
			simulcastInfo(nullptr),
			bitrate(0)
		{}
		MediaInfo(const string& ID, const string& mt) :
			id(ID),
			mtype(mt),
			direction(SENDRECV),
			simulcast(false),
			simulcastInfo(nullptr),
			bitrate(0)			
		{}

		MediaInfo(const string& mType, const Capability* capability)
		{
			MediaInfo(mType, mType);

			if (capability != nullptr) {
				if (!capability->Codecs.empty()) {
					for (auto& codec : CodecMapFromNames(capability->Codecs, capability->Rtx, capability->Rtcpfbs)) {
						AddCodec(codec.second);
					}
				}

				for (int i = 0; i < capability->Extensions.size(); ++i) {
					AddExtension(i, capability->Extensions[i]);
				}
			}
			else {
				SetDirection(INACTIVE);
			}
		}

		const string& GetType() const {
			return mtype;
		}

		void SetType(const string& t) {
			mtype = t;
		}

		const string& GetID() const {
			return id;
		}

		void SetID(const string& iD) {
			id = iD;
		}
		void AddExtension(int id, const string& name) {
			extensions[id] = name;
		}

		void AddRID(const RIDInfo& ridInfo) {
			rids[ridInfo.id] = ridInfo;
		}

		void AddCodec(const CodecInfo& codecInfo) {
			codecs[codecInfo.ctype] = codecInfo;
		}

		// GetCodec should return array
		CodecInfo* Codec(const string& codec) {
			return const_cast<CodecInfo*>(GetCodec(codec));
		}				
		const CodecInfo* GetCodec(const string& codec) const {
			for (auto& codecInfo : codecs) {
				if (stringUtil::tolower(codecInfo.second.codec) == stringUtil::tolower(codec)) {
					return &codecInfo.second;
				}
			}
			return nullptr;
		}

		
		CodecInfo* CodecForType(int pt) {
			return const_cast<CodecInfo*>(GetCodecForType(pt));
		}		
		bool GetCodecForType(int pt, CodecInfo& codec) const {
			for (auto& codecInfo : codecs) {
				if (codecInfo.second.ctype == pt) {
					codec = codecInfo.second;
					return true;
				}
			}
			return false;
		}
		const CodecInfo* GetCodecForType(int pt) const {
			for (auto& codecInfo : codecs) {
				if (codecInfo.second.ctype == pt) {
					return &codecInfo.second;
				}
			}
			return nullptr;
		}

		map<int, CodecInfo>& Codecs() {
			return codecs;
		}
		const map<int, CodecInfo>& GetCodecs() const {
			return codecs;
		}

		bool HasRTX() {
			for (auto codecInfo : codecs) {
				if (codecInfo.second.HasRTX()) {
					return true;
				}
			}
			return false;
		}

		const map<int, string>& GetExtensions() const {
			return extensions;
		}

		bool HasExtension(const string& uri) const
		{
			for (auto extension : extensions) {
				if (extension.second == uri) {
					return true;
				}
			}
			return false;
		}

		const map<string, RIDInfo>& GetRIDS() const {
			return rids;
		}

		RIDInfo* GetRID(const string& id) {
			auto it = rids.find(id);
			return it == rids.end() ? nullptr : &it->second;
		}

		int GetBitrate() const {
			return bitrate;
		}

		void SetBitrate(int b) {
			bitrate = b;
		}

		Direction GetDirection() const {
			return direction;
		}

		void SetDirection(Direction d) {
			direction = d;
		}

		bool GetSimulcast() const {
			return simulcast;
		}

		void SetSimulcast(bool s) {
			simulcast = s;
		}

		const SimulcastInfo* GetSimulcastInfo() const {
			return simulcastInfo;
		}
				

		void SetSimulcastInfo(const SimulcastInfo* info) {
			utils::setPointerData(simulcastInfo, info);
		}

		MediaInfo Answer(const MediaInfo& supportedMedia)
		{
			MediaInfo answer(id, mtype);
			answer.SetDirection(Direction_Reverse(direction));

			for (auto codec : codecs) {
				// If we support this codec
				if (supportedMedia.GetCodec(stringUtil::tolower(codec.second.codec)) != nullptr) {
					auto supported = supportedMedia.GetCodec(stringUtil::tolower(codec.second.codec));
					if (supported == nullptr) {
						continue;
					}
					if (supported->codec == "h264" && supported->HasParam("packetization-mode")
						&& supported->GetParam("packetization-mode") != codec.second.GetParam("packetization-mode")) {
						continue;
					}
					if (supported->codec == "h264" && supported->HasParam("profile-level-id")
						&& supported->GetParam("profile-level-id") != codec.second.GetParam("profile-level-id")) {
						continue;
					}

					CodecInfo cloned = *supported;
					cloned.ctype = codec.second.ctype;
					if (cloned.HasRTX()) {
						cloned.rtx = codec.second.rtx;
					}
					cloned.AddParams(codec.second.params);
					answer.AddCodec(cloned);
				}
			}

			//extentions
			int i = 0;
			for (auto& uri : extensions) {
				if (supportedMedia.HasExtension(uri.second)) {
					answer.AddExtension(i, uri.second);
				}
				++i;
			}

			if (supportedMedia.simulcast && simulcastInfo != nullptr) {
				SimulcastInfo* simulcast = new SimulcastInfo(*simulcastInfo);

				for (auto& rid : rids) {
					RIDInfo reversed = rid.second;
					reversed.direction = DirectionWay_Reverse(rid.second.direction);
					answer.AddRID(reversed);
				}
				answer.SetSimulcastInfo(simulcast);
			}

			return answer;
		}

		MediaInfo AnswerCapability(const Capability& cap) const {

			MediaInfo answer(id, mtype);
			answer.SetDirection(DirectionWay_Reverse(direction));

			map<string, CodecInfo> codecs = CodecMapFromNames(cap.Codecs, cap.Rtx, cap.Rtcpfbs);

			for (auto& codec : codecs) {
				// If we support this codec					
				string lowerCodecName = stringUtil::tolower(codec.second.codec);
				if (codecs.find(lowerCodecName) != codecs.end()) {
					CodecInfo& supported = codecs[lowerCodecName];
					if (supported.codec == "h264" && supported.HasParam("packetization-mode")
						&& supported.GetParam("packetization-mode") != codec.second.GetParam("packetization-mode")) {
						continue;
					}
					if (supported.codec == "h264" && supported.HasParam("profile-level-id")
						&& supported.GetParam("profile-level-id") != codec.second.GetParam("profile-level-id")) {
						continue;
					}

					CodecInfo cloned = supported;
					cloned.ctype = codec.second.ctype;
					if (cloned.HasRTX()) {
						cloned.rtx = codec.second.rtx;
					}
					cloned.params = codec.second.params;
					answer.AddCodec(cloned);
				}
			}

			//extentions
			int i = 0;
			for (auto uri : extensions) {
				if (std::find(cap.Extensions.begin(), cap.Extensions.end(), uri.second) != cap.Extensions.end()) {
					answer.AddExtension(i, uri.second);
				}
				++i;
			}

			if (cap.Simulcast && simulcastInfo != nullptr) {
				SimulcastInfo* simulcast = new SimulcastInfo(*simulcastInfo);

				for (auto& rid : rids) {
					RIDInfo reversed = rid.second;
					reversed.direction = DirectionWay_Reverse(rid.second.direction);
					answer.AddRID(reversed);
				}
				answer.SetSimulcastInfo(simulcast);
			}

			return answer;
		}

	};

}