headers = functions.h ds.h
objects = functions.o ds.o

dstest: dstest.cpp $(objects) $(headers)
	g++ -Wall -o dstest dstest.cpp $(objects)

functions.o: functions.h functions.cpp ds.h
	g++ -Wall -c functions.cpp

ds.o: ds.h
	g++ -Wall -o ds.o -c ds.h

.PHONY : clean
clean:
	rm -rf $(objects) dstest
