CXX = g++
CPP = gcc

INCLUDES = -I/usr/include/SDL2
LIBS = -L/usr/lib/x86_64-linux-gnu -lboost_system

HDRS = \
camera.h \
controller.h \
device.h \
image.h

SRCS = \
imagecapture.cpp \
camera.cpp \
controller.cpp \
device.cpp \
image.cpp

imagecapture: $(SRCS) $(HDRS)
	$(CXX) --std=c++14 -o imagecapture $(SRCS) $(LIBS)
