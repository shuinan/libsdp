// This example illustrates how C++ classes can be used from Java using SWIG.
// The Java class gets mapped onto the C++ class and behaves as if it is a Java class.

public class test {
  static {
    try {
        System.load("/home/shangrong/tmp/libsdp/wrapper/libsdp.so");
//		System.loadLibrary("libsdp");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" + e);
      System.exit(1);
    }
  }

static String sdpstr = "v=1\r\n" 
+ "o=- 4327261771880257373 2 IN IP4 127.0.0.1\r\n"
+ "s=-\r\n"
+ "t=1 1\r\n"
+ "a=group:BUNDLE audio video\r\n"
+ "a=msid-semantic: WMS xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n"
+ "m=audio 9 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 110 112 113 126\r\n"
+ "c=IN IP4 0.0.0.0\r\n"
+ "a=rtcp:9 IN IP4 0.0.0.0\r\n"
+ "a=ice-ufrag:ez5G\r\n"
+ "a=ice-pwd:1F1qS++jzWLSQi0qQDZkX/QV\r\n"
+ "a=candidate:1 1 UDP 33554431 35.188.215.104 59110 typ host\r\n"
+ "a=fingerprint:sha-256 D2:FA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F\r\n"
+ "a=setup:actpass\r\n"
+ "a=connection:new\r\n"
+ "a=mid:audio\r\n"
+ "a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level\r\n"
+ "a=sendrecv\r\n"
+ "a=rtcp-mux\r\n"
+ "a=rtpmap:111 opus/48000/2\r\n"
+ "a=rtcp-fb:111 transport-cc\r\n"
+ "a=fmtp:111 minptime=10;useinbandfec=1\r\n"
+ "a=rtpmap:103 ISAC/16000\r\n"
+ "a=rtpmap:104 ISAC/32000\r\n"
+ "a=rtpmap:9 G722/8000\r\n"
+ "a=rtpmap:0 PCMU/8000\r\n"
+ "a=rtpmap:8 PCMA/8000\r\n"
+ "a=rtpmap:106 CN/32000\r\n"
+ "a=rtpmap:105 CN/16000\r\n"
+ "a=rtpmap:13 CN/8000\r\n"
+ "a=rtpmap:110 telephone-event/48000\r\n"
+ "a=rtpmap:112 telephone-event/32000\r\n"
+ "a=rtpmap:113 telephone-event/16000\r\n"
+ "a=rtpmap:126 telephone-event/8000\r\n"
+ "a=ssrc:3510681183 cname:loqPWNg7JMmrFUnr\r\n"
+ "a=ssrc:3510681183 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj 7ea47500-22eb-4815-a899-c74ef321b6ee\r\n"
+ "a=ssrc:3510681183 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n"
+ "a=ssrc:3510681183 label:7ea47500-22eb-4815-a899-c74ef321b6ee\r\n"
+ "m=video 9 UDP/TLS/RTP/SAVPF 96 98 100 102 127 125 97 99 101 124\r\n"
+ "c=IN IP4 0.0.0.0\r\n"
+ "a=connection:new\r\n"
+ "a=rtcp:9 IN IP4 0.0.0.0\r\n"
+ "a=ice-ufrag:ez5G\r\n"
+ "a=ice-pwd:1F1qS++jzWLSQi0qQDZkX/QV\r\n"
+ "a=candidate:1 1 UDP 33554431 35.188.215.104 59110 typ host\r\n"
+ "a=fingerprint:sha-256 D2:FA:0E:C3:22:59:5E:14:95:69:92:3D:13:B4:84:24:2C:C2:A2:C0:3E:FD:34:8E:5E:EA:6F:AF:52:CE:E6:0F\r\n"
+ "a=setup:actpass\r\n"
+ "a=mid:video\r\n"
+ "a=extmap:2 urn:ietf:params:rtp-hdrext:toffset\r\n"
+ "a=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\n"
+ "a=extmap:4 urn:3gpp:video-orientation\r\n"
+ "a=extmap:5 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01\r\n"
+ "a=extmap:6 http://www.webrtc.org/experiments/rtp-hdrext/playout-delay\r\n"
+ "a=sendrecv\r\n"
+ "a=rtcp-mux\r\n"
+ "a=rtcp-rsize\r\n"
+ "a=rtpmap:96 VP8/90000\r\n"
+ "a=rtcp-fb:96 ccm fir\r\n"
+ "a=rtcp-fb:96 nack\r\n"
+ "a=rtcp-fb:96 nack pli\r\n"
+ "a=rtcp-fb:96 goog-remb\r\n"
+ "a=rtcp-fb:96 transport-cc\r\n"
+ "a=rtpmap:98 VP9/90000\r\n"
+ "a=rtcp-fb:98 ccm fir\r\n"
+ "a=rtcp-fb:98 nack\r\n"
+ "a=rtcp-fb:98 nack pli\r\n"
+ "a=rtcp-fb:98 goog-remb\r\n"
+ "a=rtcp-fb:98 transport-cc\r\n"
+ "a=rtpmap:100 H264/90000\r\n"
+ "a=rtcp-fb:100 ccm fir\r\n"
+ "a=rtcp-fb:100 nack\r\n"
+ "a=rtcp-fb:100 nack pli\r\n"
+ "a=rtcp-fb:100 goog-remb\r\n"
+ "a=rtcp-fb:100 transport-cc\r\n"
+ "a=fmtp:100 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f\r\n"
+ "a=rtpmap:102 red/90000\r\n"
+ "a=rtpmap:127 ulpfec/90000\r\n"
+ "a=rtpmap:125 flexfec-03/90000\r\n"
+ "a=rtcp-fb:125 ccm fir\r\n"
+ "a=rtcp-fb:125 nack\r\n"
+ "a=rtcp-fb:125 nack pli\r\n"
+ "a=rtcp-fb:125 goog-remb\r\n"
+ "a=rtcp-fb:125 transport-cc\r\n"
+ "a=fmtp:125 repair-window=10000000\r\n"
+ "a=rtpmap:97 rtx/90000\r\n"
+ "a=fmtp:97 apt=96\r\n"
+ "a=rtpmap:99 rtx/90000\r\n"
+ "a=fmtp:99 apt=98\r\n"
+ "a=rtpmap:101 rtx/90000\r\n"
+ "a=fmtp:101 apt=100\r\n"
+ "a=rtpmap:124 rtx/90000\r\n"
+ "a=fmtp:124 apt=102\r\n"
+ "a=ssrc-group:FID 3004364195 1126032854\r\n"
+ "a=ssrc-group:FEC-FR 3004364195 1080772241\r\n"
+ "a=ssrc:3004364195 cname:loqPWNg7JMmrFUnr\r\n"
+ "a=ssrc:3004364195 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n"
+ "a=ssrc:3004364195 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n"
+ "a=ssrc:3004364195 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n"
+ "a=ssrc:1126032854 cname:loqPWNg7JMmrFUnr\r\n"
+ "a=ssrc:1126032854 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n"
+ "a=ssrc:1126032854 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n"
+ "a=ssrc:1126032854 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n"
+ "a=ssrc:1080772241 cname:loqPWNg7JMmrFUnr\r\n"
+ "a=ssrc:1080772241 msid:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n"
+ "a=ssrc:1080772241 mslabel:xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj\r\n"
+ "a=ssrc:1080772241 label:cf093ab0-0b28-4930-8fe1-7ca8d529be25\r\n";

static CodecInfo toCodecInfoObj(SWIGTYPE_p_CodecInfo codecInfo)
{
	return new CodecInfo(SWIGTYPE_p_CodecInfo.getCPtr(codecInfo), false);
}

static void AssertExt(boolean b, String s) {
	if (!b)
	{
		System.out.println( " error for: " + s );
	}
}
  
static void Test_Parse() {
	SDPInfo offer = new SDPInfo();
	AssertExt(offer.Parse(sdpstr), "sdp");


	CandidateInfos candidatesObj = new CandidateInfos(SWIGTYPE_p_std__vectorT_sdp__CandidateInfo_t.getCPtr(offer.GetCandidates()), false);
	CandidateInfo canObj = new CandidateInfo(SWIGTYPE_p_CandidateInfo.getCPtr(candidatesObj.get(0)), false);
	AssertExt(canObj.getIp().equals("35.188.215.104"), "candidate's ip is error");


	CodecInfo codecInfo = toCodecInfoObj(offer.GetMedia("audio").GetCodecForType(111)); 
	AssertExt(codecInfo.getCodec().equals("opus"), "codec is not opus");
	

	MediaInfos medias = new MediaInfos(SWIGTYPE_p_std__vectorT_sdp__MediaInfo_t.getCPtr(offer.GetMedias()), false);
	AssertExt(medias.size() == 2, "medias size is not 2");

	DTLSInfo dtls = offer.GetDTLS();
	AssertExt(dtls.getHash().equals("sha-256"), dtls.getHash());
	System.out.println( "    dtls hash: " +  dtls.getHash());
	StreamInfo stream = offer.GetStream("xIKmAwWv4ft4ULxNJGhkHzvPaCkc8EKo4SGj");
	AssertExt(stream != null, "stream is null");
	System.out.println( "    stream id: " +  stream.getId());
	SWIGTYPE_p_TrackInfo trackPtr = stream.GetTrack("7ea47500-22eb-4815-a899-c74ef321b6ee");
	System.out.println( "    track: " +  SWIGTYPE_p_TrackInfo.getCPtr(trackPtr));
	TrackInfo track = new TrackInfo(SWIGTYPE_p_TrackInfo.getCPtr(trackPtr), false);
	AssertExt(track.getMedia().equals("audio"), "audio");
}


static void Test_SDPTOString() {	
	SDPInfo sdpInfo = new SDPInfo();
	AssertExt(sdpInfo.Parse(sdpstr), "sdp");

	String sdpString = sdpInfo.String();

	SDPInfo sdpInfo2 = new SDPInfo();
	AssertExt(sdpInfo2.Parse(sdpString), "sdp2");

	AssertExt(sdpInfo2.GetFirstStream().getId().equals(sdpInfo.GetFirstStream().getId()), "first stream id is diff");

	MediaInfos medias = new MediaInfos(SWIGTYPE_p_std__vectorT_sdp__MediaInfo_t.getCPtr(sdpInfo.GetMedias()), false);
	MediaInfos medias2 = new MediaInfos(SWIGTYPE_p_std__vectorT_sdp__MediaInfo_t.getCPtr(sdpInfo2.GetMedias()), false);
//	AssertExt(medias2.size() == medias.size());

	AssertExt(sdpInfo2.GetICE().getUfrag().equals(sdpInfo.GetICE().getUfrag()), "ufrag");

	AssertExt(sdpInfo2.GetDTLS().getHash().equals(sdpInfo.GetDTLS().getHash()), "hash");
}



 public static void main(String argv[]) 
  {
	 Test_Parse();
	 Test_SDPTOString();
  }


}
