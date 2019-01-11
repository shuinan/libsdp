#pragma once

#include <string>

#include "simulcaststreaminfo.h"
#include "directionway.h"

using namespace std;

namespace sdp {

	typedef vector<SimulcastStreamInfo>		SimulcastStreamInfos;
	typedef vector<SimulcastStreamInfos>	SimulcastStreamInfoss;

	struct SimulcastInfo {
		SimulcastStreamInfoss send;
		SimulcastStreamInfoss recv;
				
		void AddSimulcastAlternativeStreams(DirectionWay direction, SimulcastStreamInfos streams) {
			if (direction == SEND) {
				send.push_back(streams);
			}
			else {
				recv.push_back(streams);
			}
		}

		void AddSimulcastStream(DirectionWay direction, const SimulcastStreamInfo& stream) {
			SimulcastStreamInfos is;
			is.push_back(stream);
			AddSimulcastAlternativeStreams(direction, is);
		}

		const SimulcastStreamInfoss& GetSimulcastStreams(DirectionWay direction) const {
			return direction == SEND ? send : recv;
		}

	};

}
