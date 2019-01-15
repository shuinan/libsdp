#pragma once

#include <string>
#include <vector>

#include "jsonSerialize.h"

#include "utils.h"

using namespace std;

namespace sdptransform {
	
	#define mem_def_OriginStruct(op)\
		mem_def(op,string,	Username, "username")\
		mem_def(op,string,	SessionId, "sessionId")\
		mem_def(op,int,   	SessionVersion,"sessionVersion")\
		mem_def(op,string,	NetType,"netType")\
		mem_def(op,int,   	IpVer,"ipVer")\
		mem_def(op,string,	Address,"address")
	struct_def(OriginStruct)

		
	#define mem_def_GroupStruct(op)\
		mem_def(op,string,	Type,"type")\
		mem_def(op,string,	Mids,"mids")
	struct_def(GroupStruct)
				
	#define mem_def_TimingStruct(op)\
		mem_def(op,int,	Start,"start")\
		mem_def(op,int,	Stop,"stop")
	struct_def(TimingStruct)

		
	#define mem_def_MsidSemanticStruct(op)\
		mem_def(op,string, Semantic,"semantic")\
		mem_def(op,string, Token,"token")
	struct_def(MsidSemanticStruct)

		
	#define mem_def_ConnectionStruct(op)\
		mem_def(op,int,   	Version,"version")\
		mem_def(op,string,	Ip,"ip")
	struct_def(ConnectionStruct)

		
	#define mem_def_RtpStruct(op)\
		mem_def(op,int,   	Payload,"payload")\
		mem_def(op,string,	Codec,"codec")\
		mem_def(op,int,   	Rate,"rate")\
		mem_def(op,int,   	Encoding,"encoding")
		struct_def(RtpStruct)

		
	#define mem_def_RtcpStruct(op)\
		mem_def(op,int,    	Port,"port")\
		mem_def(op,string, 	NetType,"netType")\
		mem_def(op,int,    	IpVer,"ipVer")\
		mem_def(op,string, 	Address,"address")
		struct_def(RtcpStruct)
				
	#define mem_def_FmtpStruct(op)\
		mem_def(op,int,   	Payload,"payload")\
		mem_def(op,string,	Config,"config")
		struct_def(FmtpStruct)

		
	#define mem_def_FingerprintStruct(op)\
		mem_def(op,string,	Type,"type")\
		mem_def(op,string,	Hash,"hash")
		struct_def(FingerprintStruct)

		
	#define mem_def_ExtStruct(op)\
		mem_def(op,int,   	Value,"value")\
		mem_def(op,string,	Uri,"uri")
		struct_def(ExtStruct)

		
	#define mem_def_RtcpFbStruct(op)\
		mem_def(op,int,   	Payload,"payload")\
		mem_def(op,string,	Type,"type")\
		mem_def(op,string,	Subtype,"subtype")
		struct_def(RtcpFbStruct)

		
	#define mem_def_SsrcGroupStruct(op)\
		mem_def(op,string,	Semantics,"semantics")\
		mem_def(op,string,	Ssrcs,"ssrcs")
		struct_def(SsrcGroupStruct)

		
	#define mem_def_SsrcStruct(op)\
		mem_def(op,uint64_t, 	Id,"id")\
		mem_def(op,string,	Attribute,"attribute")\
		mem_def(op,string,	Value,"value")
		struct_def(SsrcStruct)

		
	#define mem_def_BandwithStruct(op)\
		mem_def(op,string,	Type,"type")\
		mem_def(op,int,   	Limit,"limit")
		struct_def(BandwithStruct)

		
	#define mem_def_CandidateStruct(op)\
		mem_def(op,string,	Foundation,"foundation")\
		mem_def(op,int,   	Component,"component")\
		mem_def(op,string,	Transport,"transport")\
		mem_def(op,int,   	Priority,"priority")\
		mem_def(op,string,	Ip,"ip")\
		mem_def(op,int,   	Port,"port")\
		mem_def(op,string,	Type,"type")\
		mem_def(op,string,	Raddr,"raddr")\
		mem_def(op,int,   	Rport,"rport")
		struct_def(CandidateStruct)

		
	#define mem_def_RidStruct(op)\
		mem_def(op,string,	Id,"id")\
		mem_def(op,string,	Direction,"direction")\
		mem_def(op,string,	Params,"params")
		struct_def(RidStruct)

		
	#define mem_def_SimulcastStruct(op)\
		mem_def(op,string,	Dir1,"dir1")\
		mem_def(op,string,	List1,"list1")\
		mem_def(op,string,	Dir2,"dir2")\
		mem_def(op,string,	List2,"list2")
		struct_def(SimulcastStruct)

		// todo
		//struct_def(Simulcast03Struct) #define mem_def_OriginStruct(op)


	#define mem_def_MediaStruct(op)\
		mem_def(op,vector<RtpStruct>,  	Rtp,"rtp")\
		mem_def(op,vector<FmtpStruct>, 	Fmtp,"fmtp")\
		mem_def(op,string,            	Type,"type")\
		mem_def(op,int,               	Port,"port")\
		mem_def(op,string,            	Protocal,"protocal")\
		mem_def(op,string,            	Payloads,"payloads")\
		mem_ptr_def(op,ConnectionStruct, 	Connection,"connection")\
		mem_ptr_def(op,RtcpStruct,       	Rtcp,"rtcp")\
		mem_def(op,string,            	IceUfrag,"iceUfrag")\
		mem_def(op,string,            	IcePwd,"icePwd")\
		mem_ptr_def(op,FingerprintStruct,	Fingerprint,"fingerprint")\
		mem_def(op,string,            	Setup,"setup")\
		mem_def(op,string,            	Mid,"mid")\
		mem_def(op,string,            	Msid,"msid")\
		mem_def(op,vector<ExtStruct>,  	Ext,"ext")\
		mem_def(op,string,            	Direction,"direction")\
		mem_def(op,string,            	RtcpRsize,"rtcpRsize")\
		mem_def(op,string,            	RtcpMux,"rtcpMux")\
		mem_def(op,vector<RtcpFbStruct>,  	RtcpFb,"rtcpFb")\
		mem_def(op,vector<RidStruct>,      	Rids,"rids")\
		mem_def(op,vector<SsrcGroupStruct>,	SsrcGroups,"ssrcGroups")\
		mem_def(op,vector<SsrcStruct>,     	Ssrcs,"ssrcs")\
		mem_def(op,vector<CandidateStruct>,	Candidates,"candidates")\
		mem_def(op,vector<BandwithStruct>, 	Bandwidth,"bandwidth")\
		mem_ptr_def(op,SimulcastStruct,  	Simulcast,"simulcast")
	struct_comp_def(MediaStruct)
				
	#define mem_def_SdpStruct(op)\
		mem_def(op,int,                		Version,"version")\
		mem_ptr_def(op,OriginStruct,      	Origin,"origin")\
		mem_def(op,string,             		Name,"name")\
		mem_ptr_def(op,TimingStruct,      	Timing,"timing")\
		mem_def(op,vector<GroupStruct>,		Groups,"groups")\
		mem_ptr_def(op,MsidSemanticStruct,	MsidSemantic,"msidSemantic")\
		mem_def(op,vector<MediaStruct>,		Media,"media")\
		mem_ptr_def(op,FingerprintStruct, 	Fingerprint,"fingerprint")\
		mem_ptr_def(op,ConnectionStruct,  	Connection,"connection")\
		mem_def(op,string,             		Icelite,"icelite")
	struct_comp_def(SdpStruct)
			
}
