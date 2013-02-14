OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = g++
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS) -lm
#OPENMP FILE FLAGS
OMPBARRIERLIB=libbarrier.a
OMPCFILES=SenseReversingBarrier.cpp TournamentBarrier.cpp
OMPFILES=$(patsubst %.cpp, %.o, $(OMPCFILES))

.PHONY: test
test: $(OMPFILES) test.o
	$(CC) -o $@ $^ $(OMPLIBS)	
	
.PHONY: all
all: openmp openmpi
#create an archive for openmpi barrier objects
.PHONY: openmpi
openmpi:
	echo 'nothing to do'	

#create an archive for openmp barrier objects
.PHONY: openmp
openmp: $(OMPFILES)
	
$(OMPBARRIERLIB): $(OMPFILES)
	$(AR) $(OMPBARRIERLIB) $(OMPFILES)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(OMPFLAGS)

clean:
	rm -rf $(OMPFILES) test.o test
