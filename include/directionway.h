#pragma once

#include <string>

using namespace std;

namespace sdp {

	typedef uint8_t DirectionWay;

	static const DirectionWay SEND = 1;
	static const DirectionWay RECV = 2;

	static DirectionWay DirectionWay_ByValue(const string& d) {
		if (d == "recv")
			return RECV;
		else if (d == "send")
			return SEND;
		else
			return 0;
	}

	static DirectionWay DirectionWay_Reverse(DirectionWay d) {
		switch (d) {
		case SEND:
			return RECV;
		case RECV:
			return SEND;
		default:
			return 0;
		}
	}

	string DirectionWay_String(DirectionWay d) {
		switch (d) {
		case SEND:
			return "send";
		case RECV:
			return "recv";
		default:
			return "";
		}
	}

}
