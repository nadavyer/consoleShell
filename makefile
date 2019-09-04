# All Targets
all: bin/fs

# Tool invocations
# Executable "205709835-204211056" depends on the files Commands.o and Environment.o and Files.o and FileSystem.o and Main.o.
bin/fs: bin/Files.o bin/FileSystem.o bin/Environment.o bin/Commands.o bin/Main.o
	g++ -o bin/fs bin/Commands.o bin/Environment.o bin/Files.o bin/FileSystem.o bin/Main.o


# Depends on the source and header files
bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp
# Depends on the source and header files
bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp
# Depends on the source and header files
bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp
# Depends on the source and header files
bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp
# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp


#Clean the build directory
clean: 
	rm -f bin/* 




