# All Targets
all: fs

# Tool invocations
# Executable "fs" depends on the files hello.o and run.o.
fs: bin/StringTools.o bin/Environment.o bin/main.o bin/Commands.o bin/FileSystem.o bin/Files.o
	@echo 'Building target: fs'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/fs bin/StringTools.o bin/Environment.o bin/main.o bin/Commands.o bin/FileSystem.o bin/Files.o
	@echo 'Finished building target: fs'
	@echo ' '

# Depends on the source and header files
bin/StringTools.o: src/StringTools.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/StringTools.o src/StringTools.cpp
	
bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp

bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp

bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp


#Clean the build directory
clean: 
	rm -f bin/*
