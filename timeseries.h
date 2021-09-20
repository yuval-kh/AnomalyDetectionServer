/*
 * timeseries.h
 * Author: Yuval Khanimov
 * ID:318970902
 */
#include <vector>
#include <string>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_


using namespace std;

class TimeSeries{

public:

	TimeSeries(const char* CSVfileName){
	    LoadFromNewCSV(CSVfileName);
	}
    vector<std::string> getNames() const; //gets a copy of the features array
    vector<float> getCopyVectorByfeature(std::string feature) const; //gets a copy of a vector of specific feature
    void AddLine (string line); //adds a line to the timeseries
    void LoadFromNewCSV (const char *CSVfilename);// loads the information from the CSV file and put in the vectors
    float ReturnValue (string feature, int index) const; //returns a specific value from the vectors

private:
    vector<string> names; //vector that contains all the features in the program.
    vector<vector<float>> table; //vector that contains all the information (every information is in diffrent vector)

     //makes from a string vector of strings- every string that was sperated by ","
    vector<string> TokenizeText(string text);
    void CSVToString(const char *CSVfilename);//loads from a CSV all the information and put it in the vectors



        };



#endif /* TIMESERIES_H_ */
