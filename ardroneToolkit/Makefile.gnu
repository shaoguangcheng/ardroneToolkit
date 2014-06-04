################################################################################
# Compiler settings
################################################################################
CC=gcc
CXX=g++

################################################################################
# OpenCV
# run: 
#   export PKG_CONFIG_PATH=/opt/OpenCV-2.4/lib/pkgconfig
################################################################################
OPENCV_CFLAGS = $(shell pkg-config --cflags opencv)
OPENCV_LIBS   = $(shell pkg-config --libs opencv)

################################################################################
# ffmpeg
# install dir: /opt/ffmpeg-1.0
################################################################################
FFMPEG_DIR    = /opt/ffmpeg-1.0
FFMPEG_CFLAGS = -I$(FFMPEG_DIR)/include
FFMPEG_LIBS   = -L$(FFMPEG_DIR)/lib -lavcodec -lswscale -lavutil 


################################################################################
# overall CFLAGS & LDFLAGS
################################################################################
CXXFLAGS = -D__STDC_CONSTANT_MACROS $(FFMPEG_CFLAGS)
LDFLAGS  = -lSDL -lz -lpthread $(FFMPEG_LIBS)


################################################################################
#
################################################################################
src-all := $(wildcard *.cpp)
obj-all := $(patsubst %.cpp,%.o,$(src-all))
inc-all := $(wildcard *.h)

target  := heli.e


all : $(target)

$(target) : $(obj-all) $(inc-all)
	$(CXX) $(obj-all) -o $@ $(LDFLAGS)

%.e:%.cpp $(inc-all)
	$(CXX) $< -o $@ 

%.o:%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean :
	rm -f *.e *.o



