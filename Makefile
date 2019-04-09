CXX = g++
CPP = gcc

INCLUDES = -I/usr/include/SDL2
LIBS = -L/usr/lib/x86_64-linux-gnu -lSDL2

HDRS = \
camera.h \
device.h \
image.h \
iimageprocessor.h \
imagewriter.h

SRCS = \
imagecapture.cpp \
camera.cpp \
device.cpp \
image.cpp \
imagewriter.cpp

imagecapture: $(SRCS) $(HDRS)
	$(CXX) --std=c++14 -o imagecapture $(SRCS)
