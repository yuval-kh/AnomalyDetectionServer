#include "CLI.h"

CLI::~CLI() {
    for(Command* c:this->commands) {
        delete c;
    }
    delete commandinfo;
}

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    initializeCommands();
    commandinfo = new CommandInfo();
}
void CLI::start(){
    writeTheMenu();
    float option;
    dio->read(&option);;
    while(option != commands.size()+1){
        if (option <=0 || option > commands.size()+1)
            dio->write("Wrong option\n");
        else
            commands[option-1]->execute(commandinfo);
        writeTheMenu();
        dio->read(&option);
    }

}

void CLI::initializeCommands(){
    this->commands.push_back(new UploadCommand(dio));
    this->commands.push_back(new AlgorithmSettingsCommand(dio));
    this->commands.push_back(new AnomalyDetectionCommand(dio));
    this->commands.push_back(new DisplayResultsCommand(dio));
    this->commands.push_back(new AnalyzeResultsCommand(dio));
}
void CLI::writeTheMenu(){
    dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
    int i;
    for (i=0;i<commands.size();i++) {
        dio->write(i+1);
        dio->write("."+commands[i]->description);
    }
    dio->write(i+1);
    dio->write(".exit\n");
}
