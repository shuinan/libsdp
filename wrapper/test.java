// This example illustrates how C++ classes can be used from Java using SWIG.
// The Java class gets mapped onto the C++ class and behaves as if it is a Java class.

public class runme {
  static {
    try {
        System.loadLibrary("libsdp");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" + e);
      System.exit(1);
    }
  }

const String sdpstr = String("v=1\r\n") +
String("o=- 4327261771880257373 2 IN IP4 127.0.0.1\r\n") +
String("s=-\r\n") +
String("t=1 1\r\n") +
String("a=group:BUNDLE audio video\r\n") +
String("a=msid-semantic: WMS xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
String("m=audio 9 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 110 112 113 126\r\n") +
String("c=IN IP4 0.0.0.0\r\n") +
String("a=rtcp:9 IN IP4 0.0.0.0\r\n") +
String("a=ice-ufrag:ez5G\r\n") +
String("a=ice-pwd:1F1qS++jzWLSQi0qQDZkX/QV\r\n") +
String("a=candidate:1 1 UDP 33554431 35.188.215.104 59110 typ host\r\n") +
String("a=fingerprint:sha-256 D2:FA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F\r\n") +
String("a=setup:actpass\r\n") +
String("a=connection:new\r\n") +
String("a=mid:audio\r\n") +
String("a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level\r\n") +
String("a=sendrecv\r\n") +
String("a=rtcp-mux\r\n") +
String("a=rtpmap:111 opus/48000/2\r\n") +
String("a=rtcp-fb:111 transport-cc\r\n") +
String("a=fmtp:111 minptime=10;useinbandfec=1\r\n") +
String("a=rtpmap:103 ISAC/16000\r\n") +
String("a=rtpmap:104 ISAC/32000\r\n") +
String("a=rtpmap:9 G722/8000\r\n") +
String("a=rtpmap:0 PCMU/8000\r\n") +
String("a=rtpmap:8 PCMA/8000\r\n") +
String("a=rtpmap:106 CN/32000\r\n") +
String("a=rtpmap:105 CN/16000\r\n") +
String("a=rtpmap:13 CN/8000\r\n") +
String("a=rtpmap:110 telephone-event/48000\r\n") +
String("a=rtpmap:112 telephone-event/32000\r\n") +
String("a=rtpmap:113 telephone-event/16000\r\n") +
String("a=rtpmap:126 telephone-event/8000\r\n") +
String("a=ssrc:3510681183 cname:loqPWNg7JMmrFUnr\r\n") +
String("a=ssrc:3510681183 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj 7ea47500-22eb-4815-a899-c74ef321b6ee\r\n") +
String("a=ssrc:3510681183 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
String("a=ssrc:3510681183 label:7ea47500-22eb-4815-a899-c74ef321b6ee\r\n") +
String("m=video 9 UDP/TLS/RTP/SAVPF 96 98 100 102 127 125 97 99 101 124\r\n") +
String("c=IN IP4 0.0.0.0\r\n") +
String("a=connection:new\r\n") +
String("a=rtcp:9 IN IP4 0.0.0.0\r\n") +
String("a=ice-ufrag:ez5G\r\n") +
String("a=ice-pwd:1F1qS++jzWLSQi0qQDZkX/QV\r\n") +
String("a=candidate:1 1 UDP 33554431 35.188.215.104 59110 typ host\r\n") +
String("a=fingerprint:sha-256 D2:FA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F\r\n") +
String("a=setup:actpass\r\n") +
String("a=mid:video\r\n") +
String("a=extmap:2 urn:ietf:params:rtp-hdrext:toffset\r\n") +
String("a=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\n") +
String("a=extmap:4 urn:3gpp:video-orientation\r\n") +
String("a=extmap:5 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\n") +
String("a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay\r\n") +
String("a=sendrecv\r\n") +
String("a=rtcp-mux\r\n") +
String("a=rtcp-rsize\r\n") +
String("a=rtpmap:96 VP8/90000\r\n") +
String("a=rtcp-fb:96 ccm fir\r\n") +
String("a=rtcp-fb:96 nack\r\n") +
String("a=rtcp-fb:96 nack pli\r\n") +
String("a=rtcp-fb:96 goog-remb\r\n") +
String("a=rtcp-fb:96 transport-cc\r\n") +
String("a=rtpmap:98 VP9/90000\r\n") +
String("a=rtcp-fb:98 ccm fir\r\n") +
String("a=rtcp-fb:98 nack\r\n") +
String("a=rtcp-fb:98 nack pli\r\n") +
String("a=rtcp-fb:98 goog-remb\r\n") +
String("a=rtcp-fb:98 transport-cc\r\n") +
String("a=rtpmap:100 H264/90000\r\n") +
String("a=rtcp-fb:100 ccm fir\r\n") +
String("a=rtcp-fb:100 nack\r\n") +
String("a=rtcp-fb:100 nack pli\r\n") +
String("a=rtcp-fb:100 goog-remb\r\n") +
String("a=rtcp-fb:100 transport-cc\r\n") +
String("a=fmtp:100 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f\r\n") +
String("a=rtpmap:102 red/90000\r\n") +
String("a=rtpmap:127 ulpfec/90000\r\n") +
String("a=rtpmap:125 flexfec-03/90000\r\n") +
String("a=rtcp-fb:125 ccm fir\r\n") +
String("a=rtcp-fb:125 nack\r\n") +
String("a=rtcp-fb:125 nack pli\r\n") +
String("a=rtcp-fb:125 goog-remb\r\n") +
String("a=rtcp-fb:125 transport-cc\r\n") +
String("a=fmtp:125 repair-window=10000000\r\n") +
String("a=rtpmap:97 rtx/90000\r\n") +
String("a=fmtp:97 apt=96\r\n") +
String("a=rtpmap:99 rtx/90000\r\n") +
String("a=fmtp:99 apt=98\r\n") +
String("a=rtpmap:101 rtx/90000\r\n") +
String("a=fmtp:101 apt=100\r\n") +
String("a=rtpmap:124 rtx/90000\r\n") +
String("a=fmtp:124 apt=102\r\n") +
String("a=ssrc-group:FID 3004364195 1126032854\r\n") +
String("a=ssrc-group:FEC-FR 3004364195 1080772241\r\n") +
String("a=ssrc:3004364195 cname:loqPWNg7JMmrFUnr\r\n") +
String("a=ssrc:3004364195 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
String("a=ssrc:3004364195 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
String("a=ssrc:3004364195 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
String("a=ssrc:1126032854 cname:loqPWNg7JMmrFUnr\r\n") +
String("a=ssrc:1126032854 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
String("a=ssrc:1126032854 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
String("a=ssrc:1126032854 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
String("a=ssrc:1080772241 cname:loqPWNg7JMmrFUnr\r\n") +
String("a=ssrc:1080772241 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n") +
String("a=ssrc:1080772241 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n") +
String("a=ssrc:1080772241 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n");

  
void Test_Parse() {
	sdp::SDPInfo offer;
	offer = offer.Parse(sdpstr);


	CandidateInfo candidate = offer.GetCandidates()[0];
	assert(candidate.ip == "35.188.215.104");

	const CodecInfo codecInfo = offer.GetMedia("audio").GetCodecForType(111);
	assert(codecInfo.codec == "opus");

	assert(offer.GetMedias().size(), 2);

	const DTLSInfo dtls = offer.GetDTLS();
	assert(dtls.hash == "sha-256");

	StreamInfo stream = offer.GetStream("xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj");
	TrackInfo track = stream.GetTrack("7ea47500-22eb-4815-a899-c74ef321b6ee");
	assert(track.media == "audio");
}


void Test_SDPTOString() {
	sdp::SDPInfo si;
	sdp::SDPInfo sdpInfo = si.Parse(sdpstr);
	ASSERT_FALSE(sdpInfo == nullptr);

	String sdpString = sdpInfo.String();

	sdp::SDPInfo sdpInfo2 = si.Parse(sdpString);
	EXPECT_NE(sdpInfo2, nullptr);

	assert(sdpInfo2.GetFirstStream().id == sdpInfo.GetFirstStream().id);

	assert(sdpInfo2.GetMedias().size() == sdpInfo.GetMedias().size());

	assert(sdpInfo2.GetICE().ufrag == sdpInfo.GetICE().ufrag);

	assert(sdpInfo2.GetDTLS().hash == sdpInfo.GetDTLS().hash);
}



 public static void main(String argv[]) 
  {
	 Test_Parse();
	 Test_SDPTOString();
  }


}
