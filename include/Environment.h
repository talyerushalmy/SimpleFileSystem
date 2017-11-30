#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

public:
	Environment();
	void start();
	FileSystem& getFileSystem() const; // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands

	// my additions
	void handleInput(const string& input); // parses the input and executes the command (if possible)
	Environment(const Environment &env_src); // copy constructor
	Environment& operator=(const Environment &env_src); // copy assignment operator
	~Environment(); // destructor
	Environment(Environment &&env_src); // move constructor
	Environment& operator=(Environment &&env_src); // move assignment operator
	void clean_up();
	void duplicate(const Environment &env_src);
	void steal(Environment &env_src);
};

#endif