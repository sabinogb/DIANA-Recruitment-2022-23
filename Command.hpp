#include <string>

namespace CommandType
{
	enum
	{
		VELOCITY,
		ANGLE
	};
}

/* This class represents a command written by the user in the cmd to set ARDITO's steering angle or velocity. */
class Command
{
	private:
		bool valid;
		int type;
		std::string value;
		bool quit;
		
	public:
		Command(std::string command);
		~Command();
		bool isValid();
		const char* getValue();
		int getType();
		bool isQuit();
};
