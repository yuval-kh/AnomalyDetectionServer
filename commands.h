#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
	// you may add additional methods here
//    virtual void read(int* f){};
};

// you may add here helper classes
 class CommandInfo{
 public:
     TimeSeries* TrainCSV;
     TimeSeries* TestCSV;
     HybridAnomalyDetector* hybridDetector;
     vector<AnomalyReport*>* report;
     CommandInfo() {
         TrainCSV = nullptr;
         TestCSV  = nullptr;
         hybridDetector = new HybridAnomalyDetector();
         report = nullptr;
     }

     virtual ~CommandInfo() {
         delete TrainCSV;
         delete TestCSV;
         delete hybridDetector;
         if(report!= nullptr) {
             for (AnomalyReport *r : *this->report)
                 delete r;
         }
         delete report;
     }
 };

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
    string description;
    Command(DefaultIO* dio):dio(dio){}
	virtual void execute(CommandInfo* commandinfo)=0;
	virtual ~Command(){}

};

// implement here your command classes




class UploadCommand : public Command{
    public:
    UploadCommand(DefaultIO *dio) : Command(dio) {
        this->description = "upload a time series csv file\n";
    }

    void createFile(std::ofstream outTrain) {
        string line = dio->read();
        while(line.compare("done")) {
            outTrain<< line + "\n";
            line = dio->read();
        }
        outTrain.close();
    }

    void execute(CommandInfo* commandinfo) {
        dio->write("Please upload your local train CSV file.\n");
        createFile(std::ofstream("anomalyTrain.csv") );
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        createFile(std::ofstream("anomalyTest.csv") );
        dio->write("Upload complete.\n");
        commandinfo->TrainCSV = new TimeSeries("anomalyTrain.csv");
        commandinfo->TestCSV  = new TimeSeries("anomalyTest.csv");

    }
};


class AlgorithmSettingsCommand:public Command {
public:
    AlgorithmSettingsCommand(DefaultIO *dio) : Command(dio) {
        this->description = "algorithm settings\n";
    }

    void execute(CommandInfo* commandinfo ){
//        dio->write("The current correlation threshold is " + std::to_string(commandinfo->hybridDetector->getThreshold())+"\n");
        dio->write("The current correlation threshold is ");
        dio->write(commandinfo->hybridDetector->getThreshold());
        dio->write("\n");
        dio->write("Type a new threshold\n");
        float* newThreshold = new float ;
        dio->read(newThreshold);
        while ( 0> *newThreshold || *newThreshold>1) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(newThreshold);
        }

        commandinfo->hybridDetector->setThreshold(*newThreshold);
        delete newThreshold;

    }
};


class AnomalyDetectionCommand : public Command {
public:
    AnomalyDetectionCommand(DefaultIO *dio) : Command(dio) {
        this->description = "detect anomalies\n";
    }

    void execute(CommandInfo* commandinfo) {
        vector<AnomalyReport> report;
        commandinfo->hybridDetector->learnNormal(*commandinfo->TrainCSV);
        report = commandinfo->hybridDetector->detect(*commandinfo->TestCSV);
        commandinfo->report = new vector<AnomalyReport*> ;
        for (int i=0;i<report.size();i++)
           commandinfo->report->push_back(new AnomalyReport(report[i].description,report[i].timeStep));
        dio->write("anomaly detection complete.\n");
    }
};



class DisplayResultsCommand: public Command {
public:
    DisplayResultsCommand(DefaultIO *dio) : Command(dio) {
        this->description = "display results\n";
    }

    void execute(CommandInfo* commandinfo) {
        for (int i=0;i<(*commandinfo->report).size();i++) {
            dio->write((*commandinfo->report)[i]->timeStep);
            dio->write(" \t" + (*commandinfo->report)[i]->description+"\n");
        }
        dio->write("Done.\n");
    }

};




class AnalyzeResultsCommand: public Command {
public:
    AnalyzeResultsCommand(DefaultIO *dio):Command(dio) {
        this->description = "upload anomalies and analyze results\n";
    }
    //convers a string of "num1,num2" to two numbers and returns them as a pair
    pair<int,int> readTwoNums(string input) {
        char token = ',';
        int i = 0;
        char numchar = input[i];
        string first = "";
        string second = "";
        while (numchar != token) {
            first += numchar;
            i++;
            numchar = input[i];
        }
        i++;
        numchar = input[i];
        while (i< input.size()) {
            second += numchar;
            i++;
            numchar = input[i];
        }
        float floatfirst = stof(first);
        float floatsecond = stof(second);
        return pair<int,int>(floatfirst,floatsecond);
    }
//makes the vector from the algorithm's Anomaly report vector
    vector<pair<pair<int,int>,string>> inisializeAlgorithmVector(CommandInfo* commandinfo) {
        vector<pair<pair<int,int>,string>> reportAlgorithm;
        int i=0;
        while (i<(*commandinfo->report).size()) {
            string description = (*commandinfo->report)[i]->description;
            int start = (*commandinfo->report)[i]->timeStep;
            int end = 0;
            while (i < (*commandinfo->report).size() &&
                   description.compare((*commandinfo->report)[i]->description) == 0) {
                end = (*commandinfo->report)[i]->timeStep;
                i++;
            }
            reportAlgorithm.push_back(pair<pair<int, int>, string>(pair<int, int>(start, end), description));
//            i++;
        }
        return reportAlgorithm;
    }

    void execute(CommandInfo* commandinfo) {
        dio->write("Please upload your local anomalies file.\n");
        vector<pair<int, int>> reportInput;
        string pairstring = dio->read();
        while (pairstring.compare("done")) {
            reportInput.push_back(readTwoNums(pairstring));
            pairstring = dio->read();
        }
        dio->write("Upload complete.\n");
        int p = reportInput.size();///
        int tp =0;
        vector<pair<pair<int, int>, string>> reportAlgorithm = inisializeAlgorithmVector(commandinfo);
//        int i = 0;

        int negative = commandinfo->TestCSV->getCopyVectorByfeature(commandinfo->TestCSV->getNames()[0]).size();
        for(int i=0;i<reportInput.size();i++) {
            int startInput = reportInput[i].first;
            int endInput = reportInput[i].second;
            for (int j = 0; j < reportAlgorithm.size(); j++) {
                int startAlgorithm = reportAlgorithm[j].first.first;
                int endAlgorithm = reportAlgorithm[j].first.second;
                if (startInput <= startAlgorithm && endInput >= startAlgorithm ||
                    startInput <= endAlgorithm && endInput >= endAlgorithm ||
                    startInput <= startAlgorithm && endInput >= endAlgorithm ||
                    startInput >= startAlgorithm && endInput <= endAlgorithm) {
                    //if there is intersection between the two vectors its counted as true positive
                    tp++;
                }
            }
            negative-= (reportInput[i].second - reportInput[i].first + 1);
        }
        int fp =reportAlgorithm.size();
        for(int i = 0; i < reportAlgorithm.size(); i++) {
            int startAlgorithm = reportAlgorithm[i].first.first;
            int endAlgorithm = reportAlgorithm[i].first.second;
            for (int j=0;j<reportInput.size();j++)  {
                int startInput = reportInput[j].first;
                int endInput = reportInput[j].second;
                if (startInput <= startAlgorithm && endInput >= startAlgorithm ||
                    startInput <= endAlgorithm && endInput >= endAlgorithm ||
                    startInput <= startAlgorithm && endInput >= endAlgorithm ||
                    startInput >= startAlgorithm && endInput <= endAlgorithm) {
                    //if there is at least one intersection between the two vectors its cant be false positive
                    fp--;
                    continue;
                }
            }
        }
        dio->write("True Positive Rate: ");
        dio->write((floorf((float)tp/p*1000))/1000);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(((floorf((float)fp / negative*1000))/1000));
        dio->write("\n");
    }

    };

























#endif /* COMMANDS_H_ */
