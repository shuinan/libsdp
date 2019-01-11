#pragma once

#include <string>

using namespace std;

namespace sdp {

	struct ICEInfo {
		string	ufrag;
		string	password;
		bool	lite = false;
		bool	endOfCandidates = false;

		ICEInfo() {}
		ICEInfo(const string& uf, const string& pass, bool l, bool can) :
			ufrag(uf),
			password(pass),
			lite(l),
			endOfCandidates(can)
		{}
		ICEInfo(const string& uf, const string& pass) :
			ufrag(uf),
			password(pass),
			lite(false),
			endOfCandidates(false)
		{}

		static ICEInfo* Generate(bool lite) {			
			return new ICEInfo({
				std::to_string(rand()),
				std::to_string(rand()),
				lite,
				false });
		}


		bool IsLite() {
			return this->lite;
		}

		bool IsEndOfCandidate() {
			return this->endOfCandidates;
		}
	};
}