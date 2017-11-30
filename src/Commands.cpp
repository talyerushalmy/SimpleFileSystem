#include "../include/Commands.h"
#include <iostream>
#include "../include/StringTools.h"
#include "../include/GlobalVariables.h"
#include "../include/Environment.h"

BaseCommand::BaseCommand(std::string args) : command_name(), args(args) {}

BaseCommand::BaseCommand(std::string command_name, std::string args) : command_name(command_name), args(args) { }

std::string BaseCommand::getCommandName()
{
	return this->command_name;
}

std::string BaseCommand::getArgs()
{
	return this->args;
}

PwdCommand::PwdCommand(std::string args) : BaseCommand(args) {}

void PwdCommand::execute(FileSystem& fs)
{
	std::cout << fs.getWorkingDirectory().getAbsolutePath() << std::endl;
}

std::string PwdCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

PwdCommand::PwdCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

CdCommand::CdCommand(std::string args) : BaseCommand(args) {}

void CdCommand::execute(FileSystem& fs)
{
	std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

	if (arguments.size() != 1 || arguments[0] == "")
	{
		std::cout << "Usage: cd <path>" << std::endl;
		return;
	}

	Directory* directory = fs.getDirectoryFromPath(arguments[0]);

	if (!directory) // enters if returned directory is nullptr, meaning not found
	{
		std::cout << "The system cannot find the path specified" << std::endl;
		return;
	}

	fs.setWorkingDirectory(directory);
}

std::string CdCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

CdCommand::CdCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

LsCommand::LsCommand(std::string args) : BaseCommand(args) {}

void LsCommand::execute(FileSystem& fs)
{
	std::vector<std::string> words = StringTools::split(this->getArgs(), " ");
	
	if (words.size() != 1 && (words.size() == 2 && words[0] != "-s"))
	{
		std::cout << "Usage: ls [-s] <path>" << std::endl;
		return;
	}

	Directory* dir = fs.getDirectoryFromPath(words.back());
	if (words.back() == "" || words.back() == "-s")
		dir = &fs.getWorkingDirectory();

	if (!dir)
	{
		std::cout << "The system cannot find the path specified" << std::endl;
		return;
	}

	if (words[0] == "-s")
	{
		dir->sortByName();
		dir->sortBySize();
	}
	else
	{
		dir->sortBySize();
		dir->sortByName();
	}

	for (BaseFile* bf : dir->getChildren())
	{
		if (Directory* d = dynamic_cast<Directory*>(bf))
			std::cout << "DIR" << '\t' << d->getName() << '\t' << d->getSize() << std::endl;
		else
			std::cout << "FILE" << '\t' << bf->getName() << '\t' << bf->getSize() << std::endl;
	}
}

std::string LsCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

LsCommand::LsCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

MkdirCommand::MkdirCommand(std::string args) : BaseCommand(args) {}

void MkdirCommand::execute(FileSystem& fs)
{
	std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

	if (arguments.size() != 1 || arguments[0] == "")
	{
		std::cout << "Usage: mkdir <path>" << std::endl;
		return;
	}

	if (fs.getDirectoryFromPath(arguments[0]))
	{
		std::cout << "The directory already exists" << std::endl;
		return;
	}

	Directory* cwd = &fs.getWorkingDirectory(); // push working directory

	if (arguments[0][0] == '/')
		fs.setWorkingDirectory(&fs.getRootDirectory());

	std::vector<std::string> names = StringTools::split(arguments[0], "/");
	uint16_t processed_names = 0;

	for (std::string name : names)
	{
		if (fs.getWorkingDirectory().childDirectoryExists(name))
		{
			CdCommand cd_command(name);
			cd_command.execute(fs);
			processed_names++;
		}
		else if (fs.getWorkingDirectory().childExists(name))
		{
			std::cout << "Invalid path" << std::endl;
			return;
		}
		else break;
	}

	for (size_t i = processed_names; i < names.size(); i++)
	{
		if (names[i] == ".")
			continue;
		if (names[i] == "..")
		{
			fs.setWorkingDirectory(fs.getDirectoryFromPath(".."));
		}
		else if (names[i] != "")
		{
			Directory* dir = new Directory(names[i], &fs.getWorkingDirectory());
			fs.getWorkingDirectory().addFile(dir);
			fs.setWorkingDirectory(dir);
		}
	}

	fs.setWorkingDirectory(cwd); // pop working directory
}

std::string MkdirCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

MkdirCommand::MkdirCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

MkfileCommand::MkfileCommand(std::string args) : BaseCommand(args) {}

void MkfileCommand::execute(FileSystem& fs)
{
	try
	{
		std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

		if (arguments.size() != 2)
		{
			std::cout << "Usage: mkfile <path/filename> <size>" << std::endl;
			return;
		}
		for (std::string i : arguments)
		{
			if (StringTools::trim(i) == "")
			{
				std::cout << "Usage: mkfile <path/filename> <size>" << std::endl;
				return;
			}
		}

		unsigned int file_size = std::stoi(arguments[1].c_str());
		std::vector<std::string> path_parts = StringTools::split(arguments[0], "/");
		std::string file_name = path_parts.back();
		if (file_name == "")
		{
			std::cout << "File name not specified" << std::endl;
			return;
		}
		std::string path = arguments[0].substr(0, arguments[0].length() - file_name.length());
		Directory* dir = fs.getDirectoryFromPath(path) ? fs.getDirectoryFromPath(path) : &fs.getWorkingDirectory();
		if (!dir)
		{
			std::cout << "The system cannot find the path specified" << std::endl;
			return;
		}
		if (dir->childExists(file_name))
		{
			std::cout << "File already exists" << std::endl;
			return;
		}

		File* new_file = new File(file_name, file_size);
		dir->addFile(new_file);
	}
	catch (int e)
	{
		std::cout << "Usage: mkfile <path/filename> <size>" << std::endl;
	}
}

std::string MkfileCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

MkfileCommand::MkfileCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

CpCommand::CpCommand(std::string args) : BaseCommand(args) {}

void CpCommand::execute(FileSystem& fs)
{
	std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

	if (arguments.size() != 2)
	{
		std::cout << "Usage: cp <source-path> <destination-path>" << std::endl;
		return;
	}

	std::string src_name = StringTools::split(StringTools::trim(arguments[0]), "/").back();
	std::string src_dir = StringTools::trim(arguments[0]).substr(0, arguments[0].length() - src_name.length());

	Directory* src = fs.getDirectoryFromPath(src_dir);
	if (StringTools::trim(src_dir) == "")
		src = &fs.getWorkingDirectory();
	Directory* dest = fs.getDirectoryFromPath(arguments[1]);
	if (StringTools::trim(arguments[1]) == "")
		dest = &fs.getWorkingDirectory();

	if (!src->childExists(src_name) || !dest)
	{
		std::cout << "No such file or directory" << std::endl;
		return;
	}

	BaseFile* bf = src->getChildByName(src_name);
	if (Directory* d = dynamic_cast<Directory*>(bf))
	{
		Directory* new_d = new Directory(*d);
		dest->addFile(new_d);
	}
	else if (File* f = dynamic_cast<File*>(bf))
	{
		File* new_f = new File(*f);
		dest->addFile(new_f);
	}
}

std::string CpCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

CpCommand::CpCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

MvCommand::MvCommand(std::string args) : BaseCommand(args) {}

void MvCommand::execute(FileSystem& fs)
{
	std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

	if (arguments.size() != 2)
	{
		std::cout << "Usage: mv <source-path/file-name> <destination-path>" << std::endl;
		return;
	}
	if (StringTools::trim(arguments[0]) == "" || StringTools::trim(arguments[1]) == "")
	{
		std::cout << "Usage: mv <source-path/file-name> <destination-path>" << std::endl;
		return;
	}

	std::string src_name = StringTools::split(StringTools::trim(arguments[0]), "/").back();
	std::string src_dir = StringTools::trim(arguments[0]).substr(0, arguments[0].length() - src_name.length());

	Directory* src = fs.getDirectoryFromPath(src_dir);
	if (StringTools::trim(src_dir) == "")
		src = &fs.getWorkingDirectory();
	Directory* dest = fs.getDirectoryFromPath(arguments[1]);
	if (StringTools::trim(arguments[1]) == "")
		dest = &fs.getWorkingDirectory();

	if (!src->childExists(src_name) || !dest)
	{
		std::cout << "No such file or directory" << std::endl;
		return;
	}

	BaseFile* bf = src->getChildByName(src_name);
	if (Directory* d = dynamic_cast<Directory*>(bf))
	{
		Directory* check_cwd_or_parents = &fs.getWorkingDirectory();
		while (check_cwd_or_parents->getParent() != NULL)
		{
			if (d == check_cwd_or_parents)
			{
				std::cout << "Can't move directory" << std::endl;
				return;
			}
			check_cwd_or_parents = check_cwd_or_parents->getParent();
		}
		Directory* new_d = new Directory(*d);
		dest->addFile(new_d);
	}
	else if (File* f = dynamic_cast<File*>(bf))
	{
		File* new_f = new File(*f);
		dest->addFile(new_f);
	}
	src->removeFile(src_name);
}

std::string MvCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

MvCommand::MvCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

RenameCommand::RenameCommand(std::string args) : BaseCommand(args) {}

void RenameCommand::execute(FileSystem& fs)
{
	std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

	if (arguments.size() != 2)
	{
		std::cout << "Usage: rename <path/old-name> <new-name>" << std::endl;
		return;
	}

	std::string src_name = StringTools::split(StringTools::trim(arguments[0]), "/").back();
	std::string src_dir = StringTools::trim(arguments[0]).substr(0, arguments[0].length() - src_name.length());

	Directory* src = fs.getDirectoryFromPath(src_dir);
	if (!src || !src->childExists(src_name))
	{
		std::cout << "No such file or directory" << std::endl;
		return;
	}

	src->getChildByName(src_name)->setName(StringTools::trim(arguments[1]));
}

std::string RenameCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

RenameCommand::RenameCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

RmCommand::RmCommand(std::string args) : BaseCommand(args) {}

void RmCommand::execute(FileSystem& fs)
{
	std::vector<std::string> arguments = StringTools::split(this->getArgs(), " ");

	if (arguments.size() != 1 || (arguments.size() > 0 && arguments[0] == ""))
	{
		std::cout << "Usage: rm <path>" << std::endl;
		return;
	}
	
	if (arguments[0] == "/")
	{
		std::cout << "Can't remove directory" << std::endl;
		return;
	}

	std::string src_name = StringTools::split(StringTools::trim(arguments[0]), "/").back();
	std::string src_dir = StringTools::trim(arguments[0]).substr(0, arguments[0].length() - src_name.length());

	Directory* src = fs.getDirectoryFromPath(src_dir);
	if (!src || !src->childExists(src_name))
	{
		std::cout << "No such file or directory" << std::endl;
		return;
	}

	BaseFile* bf = src->getChildByName(src_name);

	if (bf == &fs.getRootDirectory() || bf == &fs.getWorkingDirectory())
	{
		std::cout << "Can't remove directory" << std::endl;
		return;
	}

	src->removeFile(src_name);
}

std::string RmCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

RmCommand::RmCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

HistoryCommand::HistoryCommand(std::string args, const std::vector<BaseCommand*>& history) : BaseCommand(args), history(history) {}

void HistoryCommand::execute(FileSystem& fs)
{
	if (StringTools::trim(this->getArgs()) != "")
	{
		std::cout << "Too many arguments" << std::endl;
	}

	unsigned int counter = 0;
	for (BaseCommand* base_command : this->history)
	{
		std::cout << counter << '\t' << base_command->toString() << std::endl;
		++counter;
	}
}

std::string HistoryCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

HistoryCommand::HistoryCommand(std::string command_name, std::string args, const std::vector<BaseCommand*>& history) : BaseCommand(command_name, args), history(history) {}

VerboseCommand::VerboseCommand(std::string args) : BaseCommand(args) {}

void VerboseCommand::execute(FileSystem& fs)
{
	std::string input = StringTools::trim(this->getArgs());
	if (input == "0")
		verbose = 0;
	else if (input == "1")
		verbose = 1;
	else if (input == "2")
		verbose = 2;
	else if (input == "3")
		verbose = 3;
	else
		std::cout << "Wrong verbose input" << std::endl;
}

std::string VerboseCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

VerboseCommand::VerboseCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

ErrorCommand::ErrorCommand(std::string args) : BaseCommand(args) {}

ErrorCommand::ErrorCommand(std::string command_name, std::string args) : BaseCommand(command_name, args) { }

void ErrorCommand::execute(FileSystem& fs)
{
	std::cout << this->getCommandName() << ": Unknown command" << std::endl;
}

std::string ErrorCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

ExecCommand::ExecCommand(std::string args, const std::vector<BaseCommand*>& history) : BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem& fs)
{
	try
	{
		unsigned int input = stoi(StringTools::trim(this->getArgs()));
		if (input >= this->history.size() || (StringTools::trim(this->getArgs()) != "0" && input == 0))
		{
			std::cout << "Command not found" << std::endl;
			return;
		}
		std::string command = this->history[input]->toString();
		std::cout << command << std::endl;
	}
	catch (int e)
	{
		std::cout << "Command not found" << std::endl;
	}
}

std::string ExecCommand::toString()
{
	return this->getCommandName() + this->getArgs();
}

ExecCommand::ExecCommand(std::string command_name, std::string args, const std::vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}

CommandType CommandTools::stringToCommand(const std::string& input)
{
	if (input == "")
		return CommandType::EmptyCommandType;
	if (input == "pwd")
		return CommandType::PwdCommandType;
	if (input == "cd")
		return CommandType::CdCommandType;
	if (input == "ls")
		return CommandType::LsCommandType;
	if (input == "mkdir")
		return CommandType::MkdirCommandType;
	if (input == "mkfile")
		return CommandType::MkfileCommandType;
	if (input == "cp")
		return CommandType::CpCommandType;
	if (input == "mv")
		return CommandType::MvCommandType;
	if (input == "rename")
		return CommandType::RenameCommandType;
	if (input == "rm")
		return CommandType::RmCommandType;
	if (input == "history")
		return CommandType::HistoryCommandType;
	if (input == "verbose")
		return CommandType::VerboseCommandType;
	if (input == "exec")
		return CommandType::ExecCommandType;
	return CommandType::UnknownCommandType;
}
