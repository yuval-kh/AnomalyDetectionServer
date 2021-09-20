/*
 * SimpleAnomalyDetector.cpp
 * Author: Yuval Khanimov
 * ID:318970902
 */
#include <iostream>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}



void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string> features = ts.getNames();
    string nameA,nameB;
    int size = ts.getCopyVectorByfeature(features[0]).size(); //size of every vector
//    float featureAMax[size], featureBMax[size];
    float *featureAMax = new float[size];
    float *featureBMax = new float[size];
    //the loop checks the for every feature all the next features in the array.
    for (int i=0;i<features.size()-1;i++) {
        float maxPearson=0;
        for (int j=i+1;j<features.size();j++) {
            vector<float> vectfeatureA = ts.getCopyVectorByfeature(features[i]);
            vector<float> vectfeatureB = ts.getCopyVectorByfeature(features[j]);
            float featureA[size];//array of the first feature
            float featureB[size];//array of the second feature
            std::copy(vectfeatureA.begin(),vectfeatureA.end() ,featureA);
            std::copy(vectfeatureB.begin(),vectfeatureB.end() ,featureB);
            float currentPearson = pearson(featureA, featureB ,size);
            if (abs(currentPearson)>abs(maxPearson)) {//if the pearson is bigger than the max- its the new max
                //saves all the information needed to represent correlation
                maxPearson = currentPearson;
                nameA = features[i];
                nameB = features[j];
                std::copy(vectfeatureA.begin(), vectfeatureA.end() ,featureAMax);
                std::copy(vectfeatureB.begin(), vectfeatureB.end() ,featureBMax);

            }
        }

        pushInCf(featureAMax, featureBMax, nameA, nameB, maxPearson, ts, size);


    }
}
////////////////////////////////////////
void SimpleAnomalyDetector::pushInCf (float featureA[], float featureB[], string nameA,string nameB,
                                      float maxPearson, TimeSeries ts,int size) {
    if (maxPearson>threshold) {//if the max correlation in the line is bigger than 0.9 - put it into the cf vector.
        float max = maxDistFromLine(featureA,featureB,size);
        Line line = linear_reg(featureA ,featureB, size);
        correlatedFeatures correlated;
        correlated.feature1 = nameA;
        correlated.feature2 = nameB;
        correlated.corrlation = maxPearson;
        correlated.lin_reg = line;
        correlated.threshold = max*1.1;
        cf.push_back(correlated);
    }
}

float SimpleAnomalyDetector::maxDistFromLine (float featureA[], float featureB[], int size) {
    float max = 0;
    Line line = linear_reg(featureA ,featureB, size);
    for (int index=0;index<size;index++) {
        //calculate the max distance from the line
        float current = abs(dev(Point( featureA[index],featureB[index]), line));
        if (current > max)
            max = current;
    }
    return max;
}

//////////////////////
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<string> features = ts.getNames(); //the features names
    vector<AnomalyReport> reportedVals;//will be the result of the not correlated features.
    int lines = ts.getCopyVectorByfeature(features[0]).size();//the size of every vector.
   // for (int i=0;i<lines;i++) {//go to every line
    //    for (int j=0;j<cf.size();j++) {//go to every pair of correlated features.
    for (int j=0;j<cf.size();j++) {
        for (int i=0;i<lines;i++) {
            string feature1 = cf[j].feature1;
            string feature2 = cf[j].feature2;
            //represents a point in the line regression graph
//            Point newVal = Point(ts.ReturnValue(feature1,i),ts.ReturnValue(feature2,i));
//            float dist = dev(newVal,cf[j].lin_reg);//the distance of the point from the line
//            if(dist>cf[j].threshold) {//if the distance is bigger than the threshold report it
//                string description = feature1 + "-" + feature2;
//                long timeStep = i+1;
//                reportedVals.push_back(AnomalyReport(description,timeStep));
//
//            }
            report(ts,i,feature1,feature2,cf[j], reportedVals);
        }
    }
    return reportedVals;
}

void SimpleAnomalyDetector::report(TimeSeries ts, int row, string feature1, string feature2,
                                   correlatedFeatures corralted, vector<AnomalyReport> &reportedVals) {
    Point newVal = Point(ts.ReturnValue(feature1,row),ts.ReturnValue(feature2,row));
    float dist = dev(newVal, corralted.lin_reg);//the distance of the point from the line
    if(dist > corralted.threshold) {//if the distance is bigger than the threshold report it
        string description = feature1 + "-" + feature2;
        long timeStep = row+1;
        reportedVals.push_back(AnomalyReport(description,timeStep));

    }
}

float SimpleAnomalyDetector::getThreshold() const {
    return threshold;
}

void SimpleAnomalyDetector::setThreshold(float threshold) {
    SimpleAnomalyDetector::threshold = threshold;
}
