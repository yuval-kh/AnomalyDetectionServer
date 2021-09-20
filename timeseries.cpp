/*
 * timeseries.cpp
 * Author: Yuval Khanimov
 * ID:318970902
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include "timeseries.h"


vector<string> TimeSeries::getNames() const
{
    return vector<string>(names);//returns the names vector
}
vector<float> TimeSeries::getCopyVectorByfeature(string feature) const {
        int i;
        for (i=0;i<names.size();i++) {
            if (names[i].compare(feature) ==0) { //for every vector checks if it from the needed feature
                return vector<float>(table[i]);//returns a copy if it finds the feature
            }
        }
        throw std::invalid_argument("This feature doesn't exist!");//if its not found the feature - throw error

}

void TimeSeries:: AddLine (string line) {
    vector<string> tokened = TokenizeText(line);//devide the text by ","
    for (int i = 0; i < tokened.size(); i++) {
        //push every part of the text to every vector
        table[i].push_back(std::stof(tokened[i]));
    }
}
void TimeSeries::LoadFromNewCSV (const char *CSVfilename) {
        names.clear();// deleting the previous members and loads the CSV.
        table.clear();
        CSVToString(CSVfilename);
    }

    float TimeSeries::ReturnValue(string feature, int index) const {
    vector<float> featureVector = getCopyVectorByfeature(feature);//returns the needed vector
    return featureVector[index];//returns the needed value from the vector
}

//the function gets a string and splits it to tokens (every token is ending with ",")
vector<string> TimeSeries:: TokenizeText(string text) {
    vector<string> characters; //vector of the toked sentance
    std::istringstream iss(text);
    std::string token;
    while (std::getline(iss, token, ',')) {
        //while the text is not over get the next token and insert it to the vector
        characters.push_back(token);
    }
    return characters;
}

void TimeSeries:: CSVToString(const char *CSVfilename) {
    vector<string> TokenedLine;//will represent every line of the string
    std::ifstream file(CSVfilename);
    if (file.is_open()) { //opens a file
        std::string line;
        std::getline(file, line);//gets the first line - that is the features names
        TokenedLine = TokenizeText(line);
        for (int i = 0; i < TokenedLine.size(); i++) {
            //pushing every feature to the names vector
            names.push_back(TokenedLine[i]);
        }
     table.resize(names.size());//the table size is the number of the features
        while (std::getline(file, line)) {
            TokenedLine = TokenizeText(line); //the tokenized line
            for (int i = 0; i < TokenedLine.size(); i++) {
                //every string in the tokenized line converting to float and then pushing into the correct vector.
                string s = TokenedLine[i];
                table[i].push_back(std::stof(s));
            }
        }
        file.close();
    }
    else {
        throw std:: invalid_argument("The File doesn't exist!");//throw exception if the CSV not found.
    }

}