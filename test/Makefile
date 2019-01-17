CXXSRCS    = main.cpp ../src/sdpinfo.cpp ../libsdptransform/src/grammar.cpp ../libsdptransform/src/parser.cpp ../libsdptransform/src/writer.cpp
TARGET     = testsdp


build:
	g++ $(CXXSRCS)  -I../include -I../json/include -std=c++11 -lgtest -lpthread -o testsdp

