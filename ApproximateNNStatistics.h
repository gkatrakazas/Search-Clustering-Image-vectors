#ifndef APPROXIMATENNSTATISTICS_H
#define APPROXIMATENNSTATISTICS_H

#include <set>
#include <vector>

using namespace std;

struct ApproximateNNStatistics {
    int offset;
    double t;
    double distance;
};

struct ApproximateNNStatisticsNR {
    struct Nearest {
        int offset;
        double distance;
    } nearest_1;

    struct Nth {
        vector<int> offset;
        vector<double> distance;
    } nearest_n;
    
    struct R {
        set<int> offsets;
    } nearest_R;

    double t;
};

#endif /* ApproximateNNSTATISTICS_H */

