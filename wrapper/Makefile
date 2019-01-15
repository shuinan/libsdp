TOP        = ../..
SWIGEXE    = swig
SWIG_LIB_DIR = $(TOP)/../$(TOP_BUILDDIR_TO_TOP_SRCDIR)Lib
CXXSRCS    = sdpinfo.cxx
TARGET     = libsdp
INTERFACE  = libsdp.i
SWIGOPT    =
JAVASRCS   = *.java

check: build
	$(MAKE) 

build:
	swig -c++ -java '$(INTERFACE)'
	g++ '$(CXXSRCS)' libsdp_wrap.cxx -I/usr/java/jdk1.8.0_162/include -I/usr/java/jdk1.8.0_162/include/linux -fPIC -shared -o libsdp.so
	javac '$(JAVASRCS)' 

clean:
	$(MAKE) java_clean




