VPATH=../src
INCLUDES=-I../src
CPPFLAGS=$(INCLUDES) -Wall -O3 -g
LDFLAGS=-lm
CPP=g++

OBJECTS= \
  ReadWAV.o \
  SoundUtil.o \
  WriteWAV.o

default: $(OBJECTS) $(OTHER_OBJECTS)
	$(CPP) -o ../copy_wav ../samples/copy_wav.cpp \
	   $(OBJECTS) \
	   $(CPPFLAGS) $(LDFLAGS)
	$(CPP) -o ../create_speech ../samples/create_speech.cpp \
	   $(OBJECTS) \
	   $(CPPFLAGS) $(LDFLAGS)

%.o: %.cpp %.h
	$(CPP) -c $< -o $*.o $(CPPFLAGS)

clean:
	@rm -f midi_guitar midi_guitar_int *.o
	@echo "Clean!"

