TOP        = ../..
SWIGEXE    = swig
SWIG_LIB_DIR = $(TOP)/../$(TOP_BUILDDIR_TO_TOP_SRCDIR)Lib
CXXSRCS    = ../src/sdpinfo.cpp ../libsdptransform/src/grammar.cpp ../libsdptransform/src/parser.cpp ../libsdptransform/src/writer.cpp
TARGET     = libsdp
INTERFACE  = libsdp.i
SWIGOPT    =
JAVASRCS   = *.java



build:
	swig -c++ -java $(INTERFACE)
	g++ $(CXXSRCS)  libsdp_wrap.cxx  -I../include -I../json/include -I/usr/java/jdk1.8.0_162/include -I/usr/java/jdk1.8.0_162/include/linux -std=c++11 -fPIC -shared -o libsdp.so
	javac $(JAVASRCS) 

clean:
	$(MAKE) java_clean




