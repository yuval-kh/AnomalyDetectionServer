

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
    float minthershold;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

protected:
    void pushInCf(float featureA[], float featureB[], string nameA, string nameB,
                                         float maxPearson, TimeSeries ts, int size);
    Point** TwoArraysTOPoints (float xArray[],float yArray[],int size);
    void DeletePointsArray (Point** points, int size);
    virtual void report(TimeSeries ts, int row, string feature1, string feature2,
                        correlatedFeatures corralted, vector<AnomalyReport> &reportedVals);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
