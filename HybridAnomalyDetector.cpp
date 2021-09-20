
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    SimpleAnomalyDetector();
	minthershold = 0.5;

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

Point** HybridAnomalyDetector::TwoArraysTOPoints (float xArray[],float yArray[],int size) {
    Point** points =new Point*[size];
    for (int i=0;i<size;i++) {
        points[i]=new Point(xArray[i],yArray[i]);
    }
    return points;
}
void HybridAnomalyDetector::DeletePointsArray (Point** points, int size) {
    for (int i=0;i<size;i++)
        delete points[i];
    delete points;
}
void HybridAnomalyDetector::pushInCf(float featureA[], float featureB[], string nameA, string nameB, float maxPearson,
                                     TimeSeries ts, int size) {
    if(maxPearson>threshold){
        SimpleAnomalyDetector::pushInCf(featureA, featureB, nameA, nameB, maxPearson, ts, size);
        return;
    }
    if(maxPearson>minthershold) {
        Point ** points = TwoArraysTOPoints(featureA, featureB, size);
        Point ** points2 = points;
        Circle circle = findMinCircle(points2,size);
        correlatedFeatures correlated;
        correlated.feature1 = nameA;
        correlated.feature2 = nameB;
        correlated.corrlation = maxPearson;
        correlated.xCircle = circle.center.x;
        correlated.yCircle = circle.center.y;
        correlated.radius =  circle.radius *1.1;
        cf.push_back(correlated);
        DeletePointsArray(points,size);
    }
}

void HybridAnomalyDetector::report(TimeSeries ts, int row, string feature1, string feature2, correlatedFeatures corralted,
                              vector<AnomalyReport> &reportedVals) {
    if(corralted.corrlation>threshold) {
        SimpleAnomalyDetector::report(ts, row, feature1, feature2, corralted, reportedVals);
        return;
    }
    Point newVal = Point(ts.ReturnValue(feature1,row),ts.ReturnValue(feature2,row));
    if (!isPointInCircle(Circle(Point(corralted.xCircle,corralted.yCircle),corralted.radius),newVal)) {
        string description = feature1 + "-" + feature2;
        long timeStep = row+1;
        reportedVals.push_back(AnomalyReport(description,timeStep));
    }
}


