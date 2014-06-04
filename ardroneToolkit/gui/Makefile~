################################################################################
# Compiler settings
################################################################################
CC=gcc
CXX=g++
MOC=moc-qt4

CXXINCLUDE+=-I../
################################################################################
# OpenCV settings
# 	run EXPORT PKG_CONFIG_PATH=/opt/OpenCV-2.4/lib/pkgconfig
################################################################################
OPENCV_CFLAGS=$(shell pkg-config --cflags opencv)
OPENCV_LIBS=$(shell pkg-config --libs opencv)

################################################################################
# Qt settings
################################################################################
QT_CFLAGS = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED \
            -I/usr/share/qt4/mkspecs/linux-g++ \
            -I/usr/include/qt4/QtCore \
            -I/usr/include/qt4/QtGui \
            -I/usr/include/qt4
QT_LIBS   = -lQtGui -lQtCore 

################################################################################

################################################################################
# overall settings
################################################################################
ALL_CFLAGS = $(OPENCV_CFLAGS) $(QT_CFLAGS) $(CXXINCLUDE)
ALL_LDFLAGS = $(OPENCV_LIBS) $(QT_LIBS) 

MOC_CFLAGS = $(QT_CFLAGS)

# if in debug mode
#ALL_CFLAGS += -g -rdynamic
ALL_CFLAGS += -O3

################################################################################
# overall settings
################################################################################
src-cxx-all := $(wildcard *.cpp)
src-c-all := $(wildcard *.c)
bin-all := $(patsubst %.cpp,%.e,$(src-cxx-all)) $(patsubst %.c,%.e,$(src-c-all))


all : ardrone_gui.a


moc_MainWindow.o : MainWindow.h
	$(MOC) MainWindow.h -o moc_MainWindow.cpp $(MOC_CFLAGS)
	$(CXX) -c moc_MainWindow.cpp -o moc_MainWindow.o $(ALL_CFLAGS)

ardrone_gui.a : MainWindow.o moc_MainWindow.o utils.o InputEvent.o 
	$(CXX) -o $@ $? $(ALL_LDFLAGS) 

#%.e:%.cpp
#	$(CXX) $< -o $@ $(ALL_CFLAGS) $(LDFLAGS)

#%.e:%.c
#	$(CXX) $< -o $@ $(ALL_CFLAGS) $(LDFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ $(ALL_CFLAGS)

%.o:%.c
	$(CXX) -c $< -o $@ $(ALL_CFLAGS)

clean :
	rm -f *.e *.o

