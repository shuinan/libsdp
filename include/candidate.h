#pragma once

#include <string>

using namespace std;

namespace sdp {

	struct CandidateInfo {
		string	foundation;
		int		componentID;
		string	transport;
		int		priority;
		string	address;
		int		port;
		string	ctype;
		string 	relAddr;
		int 	relPort;

		CandidateInfo() : 
			componentID(0),			
			priority(0),			
			port(0),
			relPort(0) {}
		CandidateInfo(const string& found, int compID, const string& transp,
					  int prio, const string& addr, int po, const string& ct, const string& rAddr, int rPort)
			:foundation(found),
			componentID(compID),
			transport(transp),
			priority(prio),
			address(addr),
			port(po),
			ctype(ct),
			relAddr(rAddr),
			relPort(rPort) {}
	};

}