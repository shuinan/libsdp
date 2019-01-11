#pragma once

#include <string>

using namespace std;

namespace sdp {

	typedef string Setup;

	static const Setup	SETUPACTIVE = "active";
	static const Setup	SETUPPASSIVE = "passive";
	static const Setup	SETUPACTPASS = "actpass";
	static const Setup	SETUPINACTIVE = "inactive";
	
	static Setup Setup_Reverse(const Setup& d) {
		if (d == SETUPACTIVE)
			return SETUPPASSIVE;
		else if (d == SETUPPASSIVE)
			return SETUPACTIVE;
		else if (d == SETUPACTPASS)
			return SETUPPASSIVE;
		else if (d == SETUPINACTIVE)
			return SETUPINACTIVE;
		else
			return "";
	}

	static string Setup_String(const Setup& d) {
		if (d == SETUPACTIVE)
			return "active";
		else if (d == SETUPPASSIVE)
			return "passive";
		else if (d == SETUPACTPASS)
			return "actpass";
		else if (d == SETUPINACTIVE)
			return "inactive";
		else
			return "";
	}


	static Setup Setup_ByValue(const string& s) {
		if (s == "active")
			return SETUPACTIVE;
		else if (s == "passive")
			return SETUPPASSIVE;
		else if (s == "actpass")
			return SETUPACTPASS;
		else if (s == "inactive")
			return SETUPINACTIVE;
		else
			return "";
	}
}
