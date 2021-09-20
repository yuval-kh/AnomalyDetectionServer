/*
 * SimpleAnomalyDetector.h
 * Author: Yuval Khanimov
 * ID:318970902
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;//max distance of the points.
	int xCircle;
	int yCircle;
	int radius;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);//analyzing all the information given before the detect function.
    //analyzing the values of every line- if it is corralated or not
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    float getThreshold() const;

    void setCorrelationThreshold(float threshold){///////////////////
        this->threshold=threshold;
    }

    vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    void setThreshold(float threshold);

protected:

    float threshold;
    virtual void pushInCf (float featureA[], float featureB[],
                           string nameA,string nameB, float maxPearson, TimeSeries ts,int);

    float maxDistFromLine (float featureA[], float featureB[], int size);

    virtual void report(TimeSeries ts, int row, string feature1, string feature2,
                correlatedFeatures corralted, vector<AnomalyReport> &reportedVals);


    vector<correlatedFeatures> cf;
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
