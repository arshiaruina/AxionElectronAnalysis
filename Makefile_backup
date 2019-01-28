CC = g++
RCFLAGS = $(shell root-config --cflags)
RLDFLAGS = $(shell root-config --libs)
CFLAGS = -Wall -std=c++0x -c -I./inc $(RCFLAGS) -g
LFLAGS = -Wall $(RLDFLAGS)

ODIR = tmp

OBJS = axionElectronLimit.o \
       limitCalculator.o \
       signalGenerator.o \
       backgroundExtractor.o \
       dataExtractor.o \
       efficiencyCalculator.o \
       rayTracer.o \
       interpolation.o


all: AxionElectronLimit

AxionElectronLimit: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o AxionElectronLimit
signalGenerator.o: src/signalGenerator.cc inc/signalGenerator.h inc/interpolation.h 
	$(CC) $(CFLAGS) src/signalGenerator.cc -o signalGenerator.o
rayTracer.o: src/rayTracer.cc inc/rayTracer.h inc/chipRegions.h inc/rootConfig.h 
	$(CC) $(CFLAGS) src/rayTracer.cc -o rayTracer.o
limitCalculator.o: src/limitCalculator.cc inc/limitCalculator.h inc/rootConfig.h 
	$(CC) $(CFLAGS) src/limitCalculator.cc -o limitCalculator.o
dataExtractor.o: src/dataExtractor.cc inc/dataExtractor.h inc/chipRegions.h 
	$(CC) $(CFLAGS) src/dataExtractor.cc -o dataExtractor.o
backgroundExtractor.o: src/backgroundExtractor.cc inc/backgroundExtractor.h inc/dataExtractor.h inc/chipRegions.h 
	$(CC) $(CFLAGS) src/backgroundExtractor.cc -o backgroundExtractor.o
efficiencyCalculator.o: src/efficiencyCalculator.cc inc/efficiencyCalculator.h inc/chipRegions.h 
	$(CC) $(CFLAGS) src/efficiencyCalculator.cc -o efficiencyCalculator.o
interpolation.o: src/interpolation.cpp inc/interpolation.h 
	$(CC) $(CFLAGS) src/interpolation.cpp -o interpolation.o
axionElectronLimit.o : src/axionElectronLimit.cc inc/axionElectronLimit.h inc/rootConfig.h inc/chipRegions.h inc/dataSets.h inc/signalGenerator.h inc/rayTracer.h inc/limitCalculator.h inc/dataExtractor.h inc/backgroundExtractor.h inc/efficiencyCalculator.h inc/interpolation.h 
	$(CC) $(CFLAGS) src/axionElectronLimit.cc  -o axionElectronLimit.o
clean:
	rm -f ./*~ ./*.o ./AxionElectronLimit ./src/*~ ./inc/*~ ./tmp/*
wipe: clean
	rm -f *.root *.pdf *.eps *.png *.svg *.dat *.txt
