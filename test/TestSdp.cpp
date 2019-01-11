#include "stdafx.h"

#include <windows.h>

//#include "xtypes.h"


#include "d:/rtc/third/media-server-go/sdp/sdpinfo.h"

#include "gtest/gtest.h"

#include <string>

using namespace std;
using namespace sdp;

const string sdpstr = string("v=1\r\n") +
	string("o=- 4327261771880257373 2 IN IP4 127.0.0.1\r\n") +
	string("s=-\r\n") +
	string("t=1 1\r\n") +
	string("a=group:BUNDLE audio video\r\n") +
string("a=msid-semantic: WMS xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
string("m=audio 9 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 110 112 113 126\r\n") +
string("c=IN IP4 0.0.0.0\r\n") +
string("a=rtcp:9 IN IP4 0.0.0.0\r\n") +
string("a=ice-ufrag:ez5G\r\n") +
string("a=ice-pwd:1F1qS++jzWLSQi0qQDZkX/QV\r\n") +
string("a=candidate:1 1 UDP 33554431 35.188.215.104 59110 typ host\r\n") +
string("a=fingerprint:sha-256 D2:FA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F\r\n") +
string("a=setup:actpass\r\n") +
string("a=connection:new\r\n") +
string("a=mid:audio\r\n") +
string("a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level\r\n") +
string("a=sendrecv\r\n") +
string("a=rtcp-mux\r\n") +
string("a=rtpmap:111 opus/48000/2\r\n") +
string("a=rtcp-fb:111 transport-cc\r\n") +
string("a=fmtp:111 minptime=10;useinbandfec=1\r\n") +
string("a=rtpmap:103 ISAC/16000\r\n") +
string("a=rtpmap:104 ISAC/32000\r\n") +
string("a=rtpmap:9 G722/8000\r\n") +
string("a=rtpmap:0 PCMU/8000\r\n") +
string("a=rtpmap:8 PCMA/8000\r\n") +
string("a=rtpmap:106 CN/32000\r\n") +
string("a=rtpmap:105 CN/16000\r\n") +
string("a=rtpmap:13 CN/8000\r\n") +
string("a=rtpmap:110 telephone-event/48000\r\n") +
string("a=rtpmap:112 telephone-event/32000\r\n") +
string("a=rtpmap:113 telephone-event/16000\r\n") +
string("a=rtpmap:126 telephone-event/8000\r\n") +
string("a=ssrc:3510681183 cname:loqPWNg7JMmrFUnr\r\n") +
string("a=ssrc:3510681183 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj 7ea47500-22eb-4815-a899-c74ef321b6ee\r\n") +
string("a=ssrc:3510681183 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
string("a=ssrc:3510681183 label:7ea47500-22eb-4815-a899-c74ef321b6ee\r\n") +
string("m=video 9 UDP/TLS/RTP/SAVPF 96 98 100 102 127 125 97 99 101 124\r\n") +
string("c=IN IP4 0.0.0.0\r\n") +
string("a=connection:new\r\n") +
string("a=rtcp:9 IN IP4 0.0.0.0\r\n") +
string("a=ice-ufrag:ez5G\r\n") +
string("a=ice-pwd:1F1qS++jzWLSQi0qQDZkX/QV\r\n") +
string("a=candidate:1 1 UDP 33554431 35.188.215.104 59110 typ host\r\n") +
string("a=fingerprint:sha-256 D2:FA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F\r\n") +
string("a=setup:actpass\r\n") +
string("a=mid:video\r\n") +
string("a=extmap:2 urn:ietf:params:rtp-hdrext:toffset\r\n") +
string("a=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\n") +
string("a=extmap:4 urn:3gpp:video-orientation\r\n") +
string("a=extmap:5 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\n") +
string("a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay\r\n") +
string("a=sendrecv\r\n") +
string("a=rtcp-mux\r\n") +
string("a=rtcp-rsize\r\n") +
string("a=rtpmap:96 VP8/90000\r\n") +
string("a=rtcp-fb:96 ccm fir\r\n") +
string("a=rtcp-fb:96 nack\r\n") +
string("a=rtcp-fb:96 nack pli\r\n") +
string("a=rtcp-fb:96 goog-remb\r\n") +
string("a=rtcp-fb:96 transport-cc\r\n") +
string("a=rtpmap:98 VP9/90000\r\n") +
string("a=rtcp-fb:98 ccm fir\r\n") +
string("a=rtcp-fb:98 nack\r\n") +
string("a=rtcp-fb:98 nack pli\r\n") +
string("a=rtcp-fb:98 goog-remb\r\n") +
string("a=rtcp-fb:98 transport-cc\r\n") +
string("a=rtpmap:100 H264/90000\r\n") +
string("a=rtcp-fb:100 ccm fir\r\n") +
string("a=rtcp-fb:100 nack\r\n") +
string("a=rtcp-fb:100 nack pli\r\n") +
string("a=rtcp-fb:100 goog-remb\r\n") +
string("a=rtcp-fb:100 transport-cc\r\n") +
string("a=fmtp:100 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f\r\n") +
string("a=rtpmap:102 red/90000\r\n") +
string("a=rtpmap:127 ulpfec/90000\r\n") +
string("a=rtpmap:125 flexfec-03/90000\r\n") +
string("a=rtcp-fb:125 ccm fir\r\n") +
string("a=rtcp-fb:125 nack\r\n") +
string("a=rtcp-fb:125 nack pli\r\n") +
string("a=rtcp-fb:125 goog-remb\r\n") +
string("a=rtcp-fb:125 transport-cc\r\n") +
string("a=fmtp:125 repair-window=10000000\r\n") +
string("a=rtpmap:97 rtx/90000\r\n") +
string("a=fmtp:97 apt=96\r\n") +
string("a=rtpmap:99 rtx/90000\r\n") +
string("a=fmtp:99 apt=98\r\n") +
string("a=rtpmap:101 rtx/90000\r\n") +
string("a=fmtp:101 apt=100\r\n") +
string("a=rtpmap:124 rtx/90000\r\n") +
string("a=fmtp:124 apt=102\r\n") +
string("a=ssrc-group:FID 3004364195 1126032854\r\n") +
string("a=ssrc-group:FEC-FR 3004364195 1080772241\r\n") +
string("a=ssrc:3004364195 cname:loqPWNg7JMmrFUnr\r\n") +
string("a=ssrc:3004364195 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
string("a=ssrc:3004364195 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
string("a=ssrc:3004364195 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
string("a=ssrc:1126032854 cname:loqPWNg7JMmrFUnr\r\n") +
string("a=ssrc:1126032854 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
string("a=ssrc:1126032854 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
string("a=ssrc:1126032854 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
string("a=ssrc:1080772241 cname:loqPWNg7JMmrFUnr\r\n") +
string("a=ssrc:1080772241 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
string("a=ssrc:1080772241 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
string("a=ssrc:1080772241 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n");

void Test_Parse() {
	sdp::SDPInfo si;
	sdp::SDPInfo* offer = si.Parse(sdpstr);
	ASSERT_FALSE(offer == nullptr);

	CandidateInfo candidate = offer->GetCandidates()[0];
	ASSERT_TRUE(candidate.address == "35.188.215.104");

	const CodecInfo* codecInfo = offer->GetMedia("audio")->GetCodecForType(111);
	ASSERT_TRUE(codecInfo->codec == "opus");

	EXPECT_EQ(offer->GetMedias().size(), 2);

	const DTLSInfo* dtls = offer->GetDTLS();
	ASSERT_TRUE(dtls->hash == "sha-256");

	StreamInfo* stream = offer->GetStream("xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj");
	EXPECT_NE(stream, nullptr);

	TrackInfo* track = stream->GetTrack("7ea47500-22eb-4815-a899-c74ef321b6ee");
	EXPECT_NE(track, nullptr);
	ASSERT_TRUE(track->media =="audio");
}


void Test_SDPTOString() {
	sdp::SDPInfo si;
	sdp::SDPInfo* sdpInfo = si.Parse(sdpstr);
	ASSERT_FALSE(sdpInfo == nullptr);

	string sdpstring = sdpInfo->String();

	sdp::SDPInfo* sdpInfo2 = si.Parse(sdpstring);
	EXPECT_NE(sdpInfo2, nullptr);

	EXPECT_EQ(sdpInfo2->GetFirstStream()->id, sdpInfo->GetFirstStream()->id);

	EXPECT_EQ(sdpInfo2->GetMedias().size(), sdpInfo->GetMedias().size());

	EXPECT_EQ(sdpInfo2->GetICE()->ufrag, sdpInfo->GetICE()->ufrag);

	EXPECT_EQ(sdpInfo2->GetDTLS()->hash, sdpInfo->GetDTLS()->hash);
}

void Test_SDPAnswer() {
	sdp::SDPInfo si;
	sdp::SDPInfo* offer = si.Parse(sdpstr);
	ASSERT_FALSE(offer == nullptr);

	vector<sdp::RtcpFeedbackInfo> rtcpfbs{ { "goog-remb",{} },
	{ "transport-cc",{} },
	{ "ccm",{ "fir" } },
	{ "nack",{ "pli" } } };

	sdp::Capability acap;
	acap.Codecs.push_back("opus");
	map<string, sdp::Capability> capabilities;
	capabilities["audio"] = acap;
	capabilities["video"] = sdp::Capability{ { "vp8" }, true, rtcpfbs,
	{ "urn:3gpp:video-orientation",
		"http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01",
		"http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time",
		"urn:ietf:params:rtp-hdrext:toffse",
		"urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id",
		"urn:ietf:params:rtp-hdrext:sdes:mid" }
	};

	DTLSInfo dtlsInfo{ SETUPACTPASS, "sha-256", "F2:AA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F" };
	SDPInfo answer = offer->Answer(ICEInfo::Generate(true),
								   &dtlsInfo,
								   vector<CandidateInfo>{ {"1", 1, "UDP", 33554431, "127.0.0.1", 10000, "host", "", 0}},
								   capabilities);

	ASSERT_FALSE(answer.GetMedia("audio") == nullptr);
	ASSERT_FALSE(answer.GetMedia("video") == nullptr);
}


TEST(TestSdp, TestSdp) {
	Test_Parse();
	Test_SDPTOString();
	Test_SDPAnswer();
}



