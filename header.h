#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <assert.h>
#include <list>

#include "TrueNNStatistics.h"
#include "ApproximateNNStatistics.h"

#define X_TYPE unsigned char
#define S_TYPE double
#define CONSTANT_m ((1L << 32) - 5)

using namespace std;

struct ImagePairg {
    vector < X_TYPE> * ptr;
    double g;
    int offset;
};

struct H_values {
    vector<vector<vector<double>>> data;
};



unsigned h(double W, long int M, long int m, vector<double>* s, vector<long int> * m_vec,  vector < X_TYPE> * ptr);

unsigned g(vector<unsigned> h, int k);

double manhattan_distance(vector<X_TYPE> * x1,vector<X_TYPE> * x2 );

double find_W(vector<vector<X_TYPE>> * X);

TrueNNStatistics true_nearest_neighbor(vector<X_TYPE> * q, vector<vector<X_TYPE>> * X, int i);

TrueNNStatisticsNR true_nearest_neighborNR(vector<X_TYPE> * q, vector<vector<X_TYPE>> * X, int N = -1, double R = -1);

ApproximateNNStatisticsNR lsh_nearest_neighborNR(vector<X_TYPE> * q, struct HashTable * htables, int L,  double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, int N = -1, double R = -1);

ApproximateNNStatisticsNR lsh_nearest_neighborR(vector<X_TYPE> * q, struct HashTable * htables, int L,  double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, double R);
        
#endif /* HEADER_H */

