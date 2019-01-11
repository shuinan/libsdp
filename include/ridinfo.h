#pragma once

#include <string>
#include <vector>
#include <map>

#include <iostream>

#include "directionway.h"

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
}