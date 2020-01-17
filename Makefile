default: executable

executable: executable.o
	g++ *.o -o main

executable.o: code/*.cpp
	g++ -c code/*.cpp 

clean:
	rm -rf *.o