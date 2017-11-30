#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include "../include/FileSystem.h"

enum CommandType
{
	EmptyCommandType, // for an empty input
	UnknownCommandType, // for an unknown command name
	PwdCommandType,
	CdCommandType,
	LsCommandType,
	MkdirCommandType,
	MkfileCommandType,
	CpCommandType,
	MvCommandType,
	RenameCommandType,
	RmCommandType,
	HistoryCommandType,
	VerboseCommandType,
	ExecCommandType
};

class BaseCommand {
private:
	std::string command_name;
	std::string args;

public:
	BaseCommand(std::string args);
	std::string getArgs();
	virtual void execute(FileSystem & fs) = 0;
	virtual std::string toString() = 0;

	// my addition
	BaseCommand(std::string command_name, std::string args);
	std::string getCommandName();
};

class PwdCommand : public BaseCommand {
private:
public:
	PwdCommand(std::string args);
	void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
	virtual std::string toString();

	// my addition
	PwdCommand(std::string command_name, std::string args);
};

class CdCommand : public BaseCommand {
private:
public:
	CdCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	CdCommand(std::string command_name, std::string args);
};

class LsCommand : public BaseCommand {
private:
public:
	LsCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	LsCommand(std::string command_name, std::string args);
};

class MkdirCommand : public BaseCommand {
private:
public:
	MkdirCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	MkdirCommand(std::string command_name, std::string args);
};

class MkfileCommand : public BaseCommand {
private:
public:
	MkfileCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	MkfileCommand(std::string command_name, std::string args);
};

class CpCommand : public BaseCommand {
private:
public:
	CpCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	CpCommand(std::string command_name, std::string args);
};

class MvCommand : public BaseCommand {
private:
public:
	MvCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	MvCommand(std::string command_name, std::string args);
};

class RenameCommand : public BaseCommand {
private:
public:
	RenameCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	RenameCommand(std::string command_name, std::string args);
};

class RmCommand : public BaseCommand {
private:
public:
	RmCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	RmCommand(std::string command_name, std::string args);
};

class HistoryCommand : public BaseCommand {
private:
	const std::vector<BaseCommand *> & history;
public:
	HistoryCommand(std::string args, const std::vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	HistoryCommand(std::string command_name, std::string args, const std::vector<BaseCommand *> & history);
};


class VerboseCommand : public BaseCommand {
private:
public:
	VerboseCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	VerboseCommand(std::string command_name, std::string args);
};

class ErrorCommand : public BaseCommand {
private:
public:
	ErrorCommand(std::string args);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	ErrorCommand(std::string command_name, std::string args);
};

class ExecCommand : public BaseCommand {
private:
	const std::vector<BaseCommand *> & history;
public:
	ExecCommand(std::string args, const std::vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	std::string toString();

	// my addition
	ExecCommand(std::string command_name, std::string args, const std::vector<BaseCommand *> & history);
};

class CommandTools
{
public:
	static CommandType stringToCommand(const std::string& input);
};

#endif
