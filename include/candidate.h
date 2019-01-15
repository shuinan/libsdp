#pragma once

#include <string>

#include "jsonserialize.h"

using namespace std;

namespace sdp {

#define mem_def_CandidateInfo(op)\
	mem_def_(op, string,	foundation)\
	mem_def_(op, int,		component)\
	mem_def_(op, string,	transport)\
	mem_def_(op, int,		priority)\
	mem_def_(op, string,	ip)\
	mem_def_(op, int,		port)\
	mem_def_(op, string,	type)\
	mem_def_(op, string, 	raddr)\
	mem_def_(op, int, 		rport)

	struct CandidateInfo {
		mem_def_CandidateInfo(def)	

		CandidateInfo() : 
			component(0),			
			priority(0),			
			port(0),
			rport(0) {}
		CandidateInfo(const string& found, int compID, const string& transp,
					  int prio, const string& addr, int po, const string& ct, const string& rAddr, int rPort)
			:foundation(found),
			component(compID),
			transport(transp),
			priority(prio),
			ip(addr),
			port(po),
			type(ct),
			raddr(rAddr),
			rport(rPort) {}
	};
	serialize_def(CandidateInfo)
}