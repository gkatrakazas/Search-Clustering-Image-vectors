#ifndef TRUENNSTATISTICS_H
#define TRUENNSTATISTICS_H

#include <vector>

using namespace std;

struct TrueNNStatistics {
    int offset;
    double t;
    double distance;
};

struct TrueNNStatisticsNR {
    struct Nearest {
        int offset;
        double distance;
    } nearest_1;

    struct Nth {
        vector<int> offset;
        vector<double> distance;
    } nearest_n;
    
    struct R {
        vector<int> offsets;
        vector<int> distances;
    } nearest_R;

    double t;
};


#endif /* TRUENNSTATISTICS_H */

