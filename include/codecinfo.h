#pragma once

#include <string>
#include <map>
#include <vector>

#include "stringUtil.h"

#include "RtcpFeedbackInfo.h"


using namespace std;

namespace sdp {

	struct CodecInfo {
		string				codec;
		int					ctype;
		int					rtx;
		map<string, string>	params;
		vector<RtcpFeedbackInfo>	rtcpfbs;

		CodecInfo() {}
		CodecInfo(const string& cc, int ct) : codec(cc), ctype(ct), rtx(0) {}


		bool HasParam(const string& key) const {
			return params.find(key) != params.end();
		}

		const string& GetParam(const string& key) const {
			auto it = params.find(key);
			return it == params.end() ? "" : it->second;
		}

		void AddParam(const string& key, const string& value) {
			params[key] = value;
		}

		void AddParams(const map<string, string>& ps) {
			for (auto p : ps) {
				this->params[p.first] = p.second;
			}
		}

		bool HasRTX() const {
			return this->rtx != 0;
		}

		void AddRTCPFeedback(const RtcpFeedbackInfo& rtcpfb) {
			this->rtcpfbs.push_back(rtcpfb);
		}

	};

	static map<string, CodecInfo> CodecMapFromNames(const vector<string>& names, bool rtx, const vector<RtcpFeedbackInfo>& rtcpFbs)
	{
		map<string, CodecInfo> codecInfos;
		int basePt = 96;

		for (auto& name : names) {
			int pt;
			vector<string> params = stringUtil::split(name, ";");
			string codecName = stringUtil::trim(stringUtil::tolower(params[0]));
			if (codecName == "pcmu") {
				pt = 0;
			}
			else if (codecName == "pcma") {
				pt = 8;
			}
			else {
				basePt++;
				pt = basePt;
			}

			CodecInfo codecInfo(codecName, pt);
			if (rtx && codecName != "ulpfec" && codecName != "flexfec-03" && codecName != "red") {
				basePt++;
				codecInfo.rtx = basePt;
			}

			for (auto& rtcpfb : rtcpFbs) {
				codecInfo.AddRTCPFeedback(rtcpfb);
			}


			if (params.size() > 1) {
				params.erase(params.begin());
				for (auto& param : params) {
					vector<string> values = stringUtil::split(param, "=");
					if (values.size() < 2) {
						continue;
					}
					codecInfo.AddParam(values[0], values[1]);
				}
			}

			codecInfos[codecName] = codecInfo;
		}

		return codecInfos;
	}


}