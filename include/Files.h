#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>

class BaseFile {
private:
	std::string name;
	
public:
	BaseFile(std::string name);
	std::string getName() const;
	void setName(std::string newName);
	virtual int getSize() = 0;
	
	// my additions
	virtual ~BaseFile(); // destructor
};

class File : public BaseFile {
private:
	int size;
		
public:
	File(std::string name, int size); // Constructor
	int getSize(); // Return the size of the file
	
};

class Directory : public BaseFile {
private:
	std::vector<BaseFile*> children;
	Directory *parent;

public:
	Directory(std::string name, Directory *parent); // Constructor
	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(std::string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	std::vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	std::string getAbsolutePath();  //Return the path from the root to this

	// my additions
	Directory(const Directory& dir); // copy constructor
	Directory& operator=(const Directory& dir); // copy assignment operator
	~Directory(); // destructor
	Directory(Directory&& other); // move constructor
	Directory& operator=(Directory&& dir); // move assignment operator
	void steal(Directory& dir); // copy dir to this and delete dir
	void clean_up(); // delete members - clean up
	void duplicate(const Directory& dir); // duplicates the given directory
	bool childExists(std::string name); // check if the directory holds a child by the given name
	bool childDirectoryExists(std::string name); // like childExists but specifically a directory
	BaseFile* getChildByName(std::string name);
};

#endif