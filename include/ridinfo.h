#pragma once

#include <string>
#include <vector>
#include <map>

#include <iostream>

#include "directionway.h"

#include "stringUtil.h"

#include "jsonserialize.h"

#include "D:/rtc/third/libsdptransform/include/sdptransform.hpp"

using namespace std;

namespace sdp {

	struct RIDInfo {
		string				id;
		DirectionWay		direction; // TODO	
		vector<int>			formats;
		map<string, string>	params;


		RIDInfo() {}
		RIDInfo(const string& ID, DirectionWay dir) :			
			id(ID),
			direction(dir)
		{}
					
		void SetFormats(const vector<string>& fs) {
			this->formats.clear();
			for (auto& format : fs) {
				try {
					this->formats.push_back(std::atoi(format.c_str()));
				}
				catch (const std::exception& e)
				{
					cout << string("Exception: ") + e.what() + "\r\n";
					continue;
				}
			}
		}

		void AddParam(const string& id, const string& param) {
			this->params[id] = param;
		}	
	};

	static void to_json(json& j, const RIDInfo& d) {
		string str_params;
		if (!d.formats.empty()) {
			//rid.Params = "pt=" + strings.Join(ridInfo.GetFormats(), ",")
			str_params = "pt=" + stringUtil::digitArrayToString(d.formats, ",");
		}
		for (auto param : d.params) {
			if (str_params == "") {
				str_params = param.first + "=" + param.second;
			}
			else {
				str_params = str_params + ";" + param.first + "=" + param.second;
			}
		}

		j["id"] = d.id;
		j["direction"] = DirectionWay_String(d.direction);
		j["params"] = str_params;
	}
	static void from_json(const json& j, RIDInfo& d) {
		mem_def_from(string, id, "id");
		try { d.direction = DirectionWay_ByValue(j.at("directioin").get<string>()); }
		catch (const std::exception&) {}
		try {
			vector<string> formats;
			
			if (j.find("params") != j.end()) {
				// nlohmann::json: STL-like access
				json ps = sdptransform::parseParams(j["params"]);
				for (auto l : ps.items()) {
					if (l.key() == "pt") {
						formats = stringUtil::split(l.value().get<std::string>(), ",");
					}
					else {
						d.params[l.key()] = l.value().get<std::string>();
					}
				}

				d.SetFormats(formats);			
			}
		}
		catch (const std::exception&) {}
	}		
}