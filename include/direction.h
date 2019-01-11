#pragma once

#include <string>

using namespace std;

namespace sdp {

	typedef  uint8_t Direction;

	enum {
		SENDRECV = 1,
		SENDONLY = 2,
		RECVONLY = 3,
		INACTIVE = 4
	};

	static Direction Direction_Reverse(Direction d) {
		switch (d) {
		case SENDRECV:
			return SENDRECV;
		case SENDONLY:
			return RECVONLY;
		case RECVONLY:
			return SENDONLY;
		case INACTIVE:
			return INACTIVE;
		default:
			return 0;
		}
	}

	static string Direction_String(Direction d) {
		switch (d) {
		case SENDRECV:
			return "sendrecv";
		case SENDONLY:
			return "sendonly";
		case RECVONLY:
			return "recvonly";
		case INACTIVE:
			return "inactive";
		default:
			return "";
		}
	}

	static Direction Direction_ByValue(const string& direction) {
		if (direction == "sendrecv") {
			return SENDRECV;
		}
		else if (direction == "sendonly") {
			return SENDONLY;
		}
		else if (direction == "recvonly") {
			return RECVONLY;
		}
		else if (direction == "inactive") {
			return INACTIVE;
		}

		return 0;
	}
}