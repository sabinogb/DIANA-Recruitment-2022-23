#include <iostream>
#include "Rover.hpp"
#include "MQTT.hpp"
#include "Command.hpp"

int main(int argc, char **argv)
{
	std::string userInput;
	int probes;
	Command* command;
	
	MQTT::initialize();

	std::cout << "See README for instructions. Type \"exit\" or \"quit\" to exit.\n\n";
	
	do
	{
		std::cout << "> " ;
		
		/* Get user's command from the cmd line. */
		std::getline(std::cin, userInput);
		
		command = new Command(userInput);
		
		/* Make sure user's command is valid. */
		if(command->isValid() == false)
		{
			std::cout << "Command not valid.\n" ;
			delete command;
			continue;
		}
		
		/* Quit the program if the user type "exit" or "quit". */
		if(command->isQuit())
		{
			std::cout << "Quitting the program.\n" ;
			delete command;
			goto quit;
		}
		
		/* PUBLISH data. */
		if(command->getType() == CommandType::VELOCITY)
		{
			Rover::setVelocity(command->getValue());
		}
		else if(command->getType() == CommandType::ANGLE)
		{
			Rover::setSteeringAngle(command->getValue());
		}
		
		delete command;
		
	} while(true);


	quit:
	MQTT::finalize();
	return 0;
}

