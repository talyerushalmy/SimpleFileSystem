#include "../include/FileSystem.h"
#include "../include/StringTools.h"
#include <iostream>
#include "../include/GlobalVariables.h"

FileSystem::FileSystem() : rootDirectory(new Directory("/", NULL)), workingDirectory(rootDirectory) { }

Directory& FileSystem::getRootDirectory() const
{
	return *this->rootDirectory;
}

Directory& FileSystem::getWorkingDirectory() const
{
	return *this->workingDirectory;
}

void FileSystem::setWorkingDirectory(Directory* newWorkingDirectory)
{
	this->workingDirectory = newWorkingDirectory;
}

Directory* FileSystem::getDirectoryFromPath(std::string path)
{
	try
	{
		if (path == "")
			return &this->getWorkingDirectory();

		Directory* directory = &this->getWorkingDirectory();

		if (path[0] == '/')
			directory = &this->getRootDirectory();

		for (std::string name : StringTools::split(path, "/"))
		{
			name = StringTools::trim(name);
			if (name == "" || name == ".")
				continue;
			if (name == "..")
			{
				if (directory == &this->getRootDirectory())
					continue;
				directory = directory->getParent();
			}
			else if (directory->childExists(name))
			{
				if (Directory* d = dynamic_cast<Directory*>(directory->getChildByName(name)))
				{
					directory = d;
				}
				else return NULL;
			}
			else return NULL;
		}
		return directory;
	}
	catch(int e)
	{
		return NULL;
	}
}

// rule of five
// copy constructor
FileSystem::FileSystem(const FileSystem &fs_src) : rootDirectory(fs_src.rootDirectory), workingDirectory(fs_src.workingDirectory) {
	duplicate(fs_src);
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// copy assignment operator
FileSystem &FileSystem::operator=(const FileSystem & fs_src)
{
	if (this != &fs_src)
	{
		clean_up();
		duplicate(fs_src);
	}
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	return *this;
}

// destructor
FileSystem::~FileSystem()
{
	delete rootDirectory;
	workingDirectory = nullptr;
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// move constructor
FileSystem::FileSystem(FileSystem &&other) : rootDirectory(other.rootDirectory), workingDirectory(other.workingDirectory)
{
	steal(other);
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// move assignment operator
FileSystem &FileSystem::operator=(FileSystem&& fs_src)
{
	if (this != &fs_src)
	{
		clean_up();
		steal(fs_src);
	}
	if (verbose == 1 || verbose == 3)
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	return *this;
}

void FileSystem::duplicate(const FileSystem &fs_src)
{
	this->rootDirectory = &fs_src.getRootDirectory();
	this->setWorkingDirectory(&fs_src.getWorkingDirectory());
}

void FileSystem::steal(FileSystem &fs_src)
{
	this->rootDirectory = &fs_src.getRootDirectory();
	this->workingDirectory = &fs_src.getRootDirectory();
}

void FileSystem::clean_up()
{
	this->setWorkingDirectory(this->rootDirectory);
}