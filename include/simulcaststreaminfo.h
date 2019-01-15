#pragma once

#include <string>


//#include "../json/include/nlohmann/json.hpp"
#include "nlohmann/json.hpp"

using namespace std;

namespace sdp {

	struct SimulcastStreamInfo {
		string	id;
		bool	paused;

		SimulcastStreamInfo() {}
		SimulcastStreamInfo(const string ID, bool p) : id(ID), paused(p) {}

		bool IsPaused() const {
			return paused;
		}
	};

	static void to_json(nlohmann::json& j, const SimulcastStreamInfo& d) {
		j = nlohmann::json{ {"scid", d.id},	{"paused", d.paused} };
	}
	static void from_json(const nlohmann::json& j, SimulcastStreamInfo& d) {
		d.id = j.at("scid").get<string>();
		d.paused = j.at("paused").get<bool>();
	}
}
