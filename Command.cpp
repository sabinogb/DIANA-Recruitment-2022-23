#include <regex>
#include <cctype>
#include <iostream>
#include "Command.hpp"

/* CONSTRUCTOR. It takes the string written by the user in the cmd, and makes sure that it has been written in a correct form. */
Command::Command(std::string command)
{
	/* Check wether it's a command to exit the program. */
	if(command == "exit" || command == "quit")
	{
		this->valid = true;
		this->quit = true;
		return;
	}
	
	this->quit = false;
	
	/* Make sure that the user complies with the commands' correct syntax. See the README for a more detailed description about this part. */
	std::regex pattern("\\s*[avAV]\\s*[+-]?[0-9]+([.][0-9]+)?\\s*");
	
	if(std::regex_match(command, pattern) == false)
	{
		this->valid = false;
		return;
	}
	
	/* Get the character ('a' or 'v') and the number from the user's input. */
	char character;
	std::string numberString;
	int i = 0;
	
	for(; i < command.length(); i++)
	{
		/* Skip spaces */
		if(isspace(command.at(i)))
			continue;
		
		/* Get the character ('a' or 'v') */
		if(isalpha(command.at(i)))
		{
			character = command.at(i);
			character = toupper(character);
			continue;
		}
		
		/* Read the remaining substring which contains the number. */
		if(isdigit(command.at(i)) || command.at(i) == '-' || command.at(i) == '+')
		{
			numberString = command.substr(i, command.length() - i);
			
			/* if there are any whitespaces at the end, then remove them. */
			while(isspace(numberString.back()))
				numberString.pop_back();
			
			this->value = numberString;
			break;
		}
	}
	
	if(character == 'A')
		this->type = CommandType::ANGLE;
	else
		this->type = CommandType::VELOCITY;

	/* I'll use a try-catch block to make sure that the number written by the user can be converted to a float value,
       as to avoid "out of range" errors. See the README for a more detailed description about this part. */
	
	float* float_value = (float*) malloc(sizeof(float));
	
	try
	{
		float value = std::stof(numberString);
		*float_value = value;
	}
	catch(std::exception)
	{
		this->valid = false;
		return;
	}

	/* Make sure that the input value is included between the correct intervals. */
	if(this->type == CommandType::ANGLE)
	{
		/* I chose the interval [-40, +40] instead of [-42, +42], as I explain in the README. */
		if(*float_value < -40.f || *float_value > 40.f)
		{
			this->valid = false;
			return;
		}
	}
	else
	{
		if(*float_value < -1.5f || *float_value > 1.5f)
		{
			this->valid = false;
			return;
		}
	}
	
	this->valid = true;
}

/* DESTRUCTOR */
Command::~Command()
{
	
}

bool Command::isValid()
{
	return this->valid;
}

int Command::getType()
{
	return this->type;
}

const char* Command::getValue()
{
	return this->value.c_str();
}

bool Command::isQuit()
{
	return this->quit;
}