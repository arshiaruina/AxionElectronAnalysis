CC = g++
RCFLAGS = $(shell root-config --cflags)
RLDFLAGS = $(shell root-config --libs)
CFLAGS = -Wall -std=c++0x -c -I./inc $(RCFLAGS) -g
LFLAGS = -Wall $(RLDFLAGS)

ODIR = tmp

_OBJS = axionElectronLimit.o \
       limitCalculator.o \
       signalGenerator.o \
       backgroundExtractor.o \
       dataExtractor.o \
       efficiencyCalculator.o \
       rayTracer.o \
       interpolation.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: AxionElectronLimit

AxionElectronLimit: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o AxionElectronLimit
tmp/signalGenerator.o: src/signalGenerator.cc inc/signalGenerator.h inc/interpolation.h 
	$(CC) $(CFLAGS) src/signalGenerator.cc -o tmp/signalGenerator.o
tmp/rayTracer.o: src/rayTracer.cc inc/rayTracer.h inc/chipRegions.h inc/rootConfig.h 
	$(CC) $(CFLAGS) src/rayTracer.cc -o tmp/rayTracer.o
tmp/limitCalculator.o: src/limitCalculator.cc inc/limitCalculator.h inc/rootConfig.h 
	$(CC) $(CFLAGS) src/limitCalculator.cc -o tmp/limitCalculator.o
tmp/dataExtractor.o: src/dataExtractor.cc inc/dataExtractor.h inc/chipRegions.h 
	$(CC) $(CFLAGS) src/dataExtractor.cc -o tmp/dataExtractor.o
tmp/backgroundExtractor.o: src/backgroundExtractor.cc inc/backgroundExtractor.h inc/dataExtractor.h inc/chipRegions.h 
	$(CC) $(CFLAGS) src/backgroundExtractor.cc -o tmp/backgroundExtractor.o
tmp/efficiencyCalculator.o: src/efficiencyCalculator.cc inc/efficiencyCalculator.h inc/chipRegions.h 
	$(CC) $(CFLAGS) src/efficiencyCalculator.cc -o tmp/efficiencyCalculator.o
tmp/interpolation.o: src/interpolation.cpp inc/interpolation.h 
	$(CC) $(CFLAGS) src/interpolation.cpp -o tmp/interpolation.o
tmp/axionElectronLimit.o : src/axionElectronLimit.cc inc/axionElectronLimit.h inc/rootConfig.h inc/chipRegions.h inc/dataSets.h inc/signalGenerator.h inc/rayTracer.h inc/limitCalculator.h inc/dataExtractor.h inc/backgroundExtractor.h inc/efficiencyCalculator.h inc/interpolation.h 
	$(CC) $(CFLAGS) src/axionElectronLimit.cc  -o tmp/axionElectronLimit.o
clean:
	rm -f ./*~ ./*.o ./AxionElectronLimit ./src/*~ ./inc/*~ ./tmp/*
wipe: clean
	rm -f *.root *.pdf *.eps *.png *.svg *.dat *.txt
