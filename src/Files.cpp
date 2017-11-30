#include "../include/Files.h"
#include <algorithm>
#include <iostream>
#include "../include/GlobalVariables.h"


// <BaseFile>

BaseFile::BaseFile(std::string name) : name(name) {}

BaseFile::~BaseFile() {}

std::string BaseFile::getName() const
{
	return this->name;
}

void BaseFile::setName(std::string newName)
{
	this->name = newName;
}

// </BaseFile>

// <File>

File::File(std::string name, int size) : BaseFile(name), size(size) {}

int File::getSize()
{
	return this->size;
}

// </File>

// <Directory>

Directory::Directory(std::string name, Directory* parent) : BaseFile(name), children(), parent(parent) {}

// Rule of five
// copy constructor
Directory::Directory(const Directory& dir) : BaseFile(dir.getName()), children(dir.children), parent(dir.parent)
{
	this->duplicate(dir);
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// copy assignment operator
Directory& Directory::operator=(const Directory& dir)
{
	if (this != & dir)
	{
		clean_up();
		duplicate(dir);
	}
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	return *this;
}

// destructor
Directory::~Directory()
{
	this->clean_up();
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// move constructor
Directory::Directory(Directory&& dir) : BaseFile(dir.getName()), children(dir.children), parent(dir.parent)
{
	steal(dir);
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// move assignment operator
Directory& Directory::operator=(Directory && dir) 
{
	if (this != &dir)
	{
		clean_up();
		steal(dir);
	}
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	return *this;
}

Directory* Directory::getParent() const
{
	return this->parent;
}

void Directory::setParent(Directory* newParent)
{
	this->parent = newParent;
}

void Directory::addFile(BaseFile* file)
{
	if (!childExists(file->getName()))
	{
		this->children.push_back(file);
		if (Directory* dir = dynamic_cast<Directory*>(file))
			dir->parent = this;
	}
}

void Directory::removeFile(std::string name)
{
	for (int i = this->children.size() - 1; i >= 0; i--)
	{
		if (this->children[i]->getName().compare(name) == 0)
			this->children.erase(this->children.begin() + i);
	}
}

void Directory::removeFile(BaseFile* file)
{
	for (int i = this->children.size() - 1; i >= 0; i--)
	{
		if (this->children[i] == file)
			this->children.erase(this->children.begin() + i);
	}
}

// for sorting by name
struct name_predicate
{
	inline bool operator() (BaseFile* bf1, BaseFile* bf2)
	{
		return (bf1->getName().compare(bf2->getName()) < 0);
	}
};

// for sorting by size
struct size_predicate
{
	inline  bool operator() (BaseFile* bf1, BaseFile* bf2)
	{
		return (bf1->getSize() < bf2->getSize());
	}
};

void Directory::sortByName()
{
	std::sort(this->children.begin(), this->children.end(), name_predicate());
}

void Directory::sortBySize()
{
	std::sort(this->children.begin(), this->children.end(), size_predicate());
}

std::vector<BaseFile*> Directory::getChildren()
{
	return this->children;
}

int Directory::getSize()
{
	int size = 0;
	for (BaseFile* child : this->children)
		size += child->getSize();
	return size;
}

// TODO: check while loop condition
std::string Directory::getAbsolutePath()
{
	if (this->parent == NULL)
		return this->getName();
	if (parent->getName().compare("/") != 0)
		return parent->getAbsolutePath() + '/' + this->getName();
	return parent->getAbsolutePath() + this->getName();
}

void Directory::steal(Directory& dir)
{
	this->setName(dir.getName());
	this->parent = dir.parent;
	dir.parent = NULL;
	for (BaseFile* bf : dir.children)
	{
		this->children.push_back(bf);
		if (Directory* d = dynamic_cast<Directory*>(bf))
			d->setParent(this);
	}
}


void Directory::clean_up()
{
	std::vector<BaseFile*>::iterator iterator = this->children.begin();
	while (!this->children.empty() && iterator != this->children.end())
	{
		this->removeFile(*iterator);
	}
	this->children.clear();
	this->parent = NULL;
}

void Directory::duplicate(const Directory& dir)
{
	this->children.clear();
	this->setName(dir.getName());
	this->setParent(dir.getParent());
	for (BaseFile* bf : dir.children)
	{
		if (Directory* d = dynamic_cast<Directory*>(bf))
		{
			Directory* new_d = new Directory(*d);
			new_d->setParent(this);
			this->children.push_back(new_d);
		}
		else if (File* f = dynamic_cast<File*>(bf))
		{
			File* new_f = new File(*f);
			this->children.push_back(new_f);
		}
	}
}

bool Directory::childExists(std::string name)
{
	for (BaseFile* child : this->children)
		if (child->getName().compare(name) == 0)
			return true;
	return false;
}

bool Directory::childDirectoryExists(std::string name)
{
	for (BaseFile* child : this->children)
		if (child->getName().compare(name) == 0)
		{
			if (dynamic_cast<Directory*>(child))
				return true;
			return false;
		}
	return false;
}


BaseFile* Directory::getChildByName(std::string name)
{
	for (BaseFile* child : this->children)
		if (child->getName().compare(name) == 0)
			return child;
	return NULL;
}

// </Directory>