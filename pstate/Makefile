COMPILER = g++
CCFLAGS = -Wall -g -pedantic

all: cppmd pstate

cppmd: 1pstate.cpp
	${COMPILER} ${CCFLAGS} -o cppmd 1pstate.cpp

pstate:	battery.cpp
	${COMPILER} ${CCFLAGS} -o pstate battery.cpp
clean:
	rm -f cppmd pstate
