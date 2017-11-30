#include <iostream>
#include "../include/Environment.h"
#include "../include/StringTools.h"
#include "../include/Commands.h"
#include "../include/GlobalVariables.h"

Environment::Environment() : commandsHistory(), fs()
{
	this->fs.setWorkingDirectory(&this->fs.getRootDirectory());
}

void Environment::start()
{
	string input;

	// get first input
	cout << this->fs.getWorkingDirectory().getAbsolutePath() << ">";
	getline(cin, input);
	input = StringTools::trim(input);

	// loop until the input is "exit"
	while (input.compare("exit") != 0)
	{
		// handle input
		handleInput(input);

		// get next input
		cout << this->fs.getWorkingDirectory().getAbsolutePath() << ">";
		getline(cin, input);
		input = StringTools::trim(input);
	}
}

void Environment::addToHistory(BaseCommand* command)
{
	this->commandsHistory.push_back(command);
}

const vector<BaseCommand*>& Environment::getHistory() const
{
	return this->commandsHistory;
}

void Environment::handleInput(const string& input)
{
	if (verbose == 2 || verbose == 3)
		std::cout << input << std::endl;

	string first_word = input.substr(0, input.find(' '));
	string args = input.substr(first_word.size(), input.size() - first_word.size());

	switch (CommandTools::stringToCommand(first_word))
	{
	case CommandType::EmptyCommandType:
		// do and print nothing
		break;
	case CommandType::CdCommandType:
	{
		CdCommand cd_command(first_word, args);
		cd_command.execute(this->fs);
		this->addToHistory(new CdCommand(cd_command));
	}
	break;
	case CommandType::CpCommandType:
	{
		CpCommand cp_command(first_word, args);
		cp_command.execute(this->fs);
		this->addToHistory(new CpCommand(cp_command));
	}
	break;
	case CommandType::ExecCommandType:
	{
		ExecCommand exec_command(first_word, args, this->getHistory());
		exec_command.execute(this->fs);
		this->addToHistory(new ExecCommand(exec_command));
	}
	break;
	case CommandType::HistoryCommandType:
	{
		HistoryCommand history_command(first_word, args, this->getHistory());
		history_command.execute(this->fs);
		this->addToHistory(new HistoryCommand(history_command));
	}
	break;
	case CommandType::LsCommandType:
	{
		LsCommand ls_command(first_word, args);
		ls_command.execute(this->fs);
		this->addToHistory(new LsCommand(ls_command));
	}
	break;
	case CommandType::MkdirCommandType:
	{
		MkdirCommand mkdir_command(first_word, args);
		mkdir_command.execute(this->fs);
		this->addToHistory(new MkdirCommand(mkdir_command));
	}
	break;
	case CommandType::MkfileCommandType:
	{
		MkfileCommand mkfile_command(first_word, args);
		mkfile_command.execute(this->fs);
		this->addToHistory(new MkfileCommand(mkfile_command));
	}
	break;
	case CommandType::MvCommandType:
	{
		MvCommand mv_command(first_word, args);
		mv_command.execute(this->fs);
		this->addToHistory(new MvCommand(mv_command));
	}
	break;
	case CommandType::PwdCommandType:
	{
		PwdCommand pwd_command(first_word, args);
		pwd_command.execute(this->fs);
		this->addToHistory(new PwdCommand(pwd_command));
	}
	break;
	case CommandType::RenameCommandType:
	{
		RenameCommand rename_command(first_word, args);
		rename_command.execute(this->fs);
		this->addToHistory(new RenameCommand(rename_command));
	}
	break;
	case CommandType::RmCommandType:
	{
		RmCommand rm_command(first_word, args);
		rm_command.execute(this->fs);
		this->addToHistory(new RmCommand(rm_command));
	}
	break;
	case CommandType::VerboseCommandType:
	{
		VerboseCommand verbose_command(first_word, args);
		verbose_command.execute(this->fs);
		this->addToHistory(new VerboseCommand(verbose_command));
	}
	break;
	case CommandType::UnknownCommandType:
	default:
		ErrorCommand error_command(first_word, args);
		error_command.execute(this->fs);
		this->addToHistory(new ErrorCommand(error_command));
		break;
	}
}

// rule of five

// copy constructor
Environment::Environment(const Environment &env_src) : commandsHistory(env_src.commandsHistory), fs(env_src.fs) {
	duplicate(env_src);
	if (verbose == 1 || verbose == 3)
		cout << __PRETTY_FUNCTION__ << endl;
}

// copy assignment operator
Environment& Environment::operator=(const Environment &env_src) {
	if (&env_src != this) {
		clean_up();
		duplicate(env_src);
	}
	if (verbose == 1 || verbose == 3)
		cout << __PRETTY_FUNCTION__ << endl;
	return *this;
}

// destructor
Environment::~Environment() {
	clean_up();
	if (verbose == 1 || verbose == 3)
		cout << __PRETTY_FUNCTION__ << endl;
}

// move constructor
Environment::Environment(Environment &&env_src) : commandsHistory(env_src.commandsHistory), fs(env_src.fs) {
	steal(env_src);
	if (verbose == 1 || verbose == 3)
		cout << __PRETTY_FUNCTION__ << endl;
}

// move assignment operator
Environment& Environment::operator=(Environment &&env_src) {
	if (&env_src != this) {
		clean_up();
		steal(env_src);
	}
	if (verbose == 1 || verbose == 3)
		cout << __PRETTY_FUNCTION__ << endl;
	return *this;
}

void Environment::clean_up() {
	for (BaseCommand* cmd : this->commandsHistory) {
		delete cmd;
	}
	commandsHistory.clear();
}

void Environment::duplicate(const Environment &env_src) {
	this->commandsHistory = env_src.commandsHistory;
	this->fs = env_src.fs;
}

void Environment::steal(Environment &env_src) {
	for (BaseCommand * cmd : env_src.getHistory()) {
		this->commandsHistory.push_back(cmd);
	}
	this->fs = env_src.fs;
}