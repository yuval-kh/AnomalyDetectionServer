#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    vector<Command*> commands;
    CommandInfo* commandinfo;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();

private:
    void initializeCommands();
    void writeTheMenu();
};


#endif /* CLI_H_ */
