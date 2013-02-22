OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = g++
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS) -lm
#OPENMP FILE FLAGS
OMPBARRIERLIB=libbarrier.a
OMPCFILES=SenseReversingBarrier.cpp TournamentBarrier.cpp
OMPICFILES=DisseminationBarrier.cpp MCSBarrier.cpp
OMPFILES=$(patsubst %.cpp, %.o, $(OMPCFILES))
OMPIFILES=$(patsubst %.cpp, %.o, $(OMPICFILES))

.PHONY: testMP
testMP: $(OMPFILES) testMP.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: testMPI
testMPI: $(OMPIFILES) testMPI.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDLIBS)
	
.PHONY: all
all: openmp openmpi

#create an archive for openmpi barrier objects
.PHONY: openmpi
openmpi: $(OMPIFILES)

$(OMPIFILES) testMPI testMPI.o: CC := mpicxx

#create an archive for openmp barrier objects
.PHONY: openmp
openmp: $(OMPFILES)
	
$(OMPBARRIERLIB): $(OMPFILES)
	$(AR) $(OMPBARRIERLIB) $(OMPFILES)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

clean:
	rm -rf $(OMPFILES) $(OMPIFILES) testMP.o testMP testMPI.o testMPI
