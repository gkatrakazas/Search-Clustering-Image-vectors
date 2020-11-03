#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <cfloat>
#include <ctime>
#include <queue>
#include <set>
#include "header.h"
#include "TrueNNStatistics.h"
#include "math.h"
#include "ComparePairs.h"
#include "ApproximateNNStatistics.h"
#include "hashtable.h"

unsigned h(double W, long int M, long int m, vector<double>* s, vector<long int> * m_vec, vector < X_TYPE> * ptr) { /// na vgaloume to m 
    int value_h = 0, ai = 0;
    int size = ptr->size();

    for (int i = 0; i < size; i++) {
        ai = (int) floor(((double) ((((*ptr)[i]) - ((*s)[i])) / W)));
        ai = modulo(ai, M);

        long int mi = ((*m_vec)[i]);

        value_h = value_h + (ai * mi) % M;
    }

    return value_h % M;
}

unsigned g(vector<unsigned> h, int k) { //// merge k times tou h se 32bit
    int bits = 32 / k;

    if (h.size() != (unsigned) k) {
        cout << "something went really wrong here \n";
        exit(1);
    }

    int value = h[0];

    for (int i = 1; i < k; i++) {
        value = value << bits;
        value = value + h[i];
    }

    return value;
}

double manhattan_distance(vector<X_TYPE>* x1, vector<X_TYPE>* x2) {
    int dx1 = x1->size();
    double sum = 0;

    for (int i = 0; i < dx1; i++) {

        sum = sum + fabs((double) ((*x1)[i]) - (double) ((*x2)[i]));
    }

    return (sum);
}

double find_W(vector<vector<X_TYPE>> *X) {

    int dx = X->size();
    cout << dx << endl;
    double total_sum = 0.0;

    for (int i = 0; i < dx; i++) {
        TrueNNStatistics stats = true_nearest_neighbor(&X->at(i), X, i);
        total_sum = total_sum + stats.distance;
        cout << i << " " << total_sum << endl;
    }
    return 4 * total_sum / dx;
}

TrueNNStatistics true_nearest_neighbor(vector<X_TYPE> * q, vector<vector<X_TYPE>> *X, int i) {
    TrueNNStatistics stats = {0, 0, 0};

    int dx = X->size();
    int ni;
    double min = DBL_MAX, temp = 0;

    clock_t begin = clock();
    for (int j = 0; j < dx; j++) {
        if (j == i) {
            continue;
        }
        temp = manhattan_distance(q, &X->at(j));
        if (temp < min) {
            min = temp;
            ni = j;
        }
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    stats.distance = min;
    stats.offset = ni;
    stats.t = elapsed_secs; 

    return stats;

}

TrueNNStatisticsNR true_nearest_neighborNR(vector<X_TYPE> * q, vector<vector<X_TYPE>> *X, int N, double R) {
    TrueNNStatisticsNR stats = {0};

    int dx = X->size();

    clock_t begin = clock();
    // --------------------------------

    priority_queue< pair<int, double>, vector<pair<int, double> >, ComparePairs> queue;

    for (int j = 0; j < dx; j++) {
        double temp = manhattan_distance(q, &((*X)[j]));
        if (temp < stats.nearest_1.distance || j == 0) {
            stats.nearest_1.distance = temp;
            stats.nearest_1.offset = j;
        }

        if (R != -1 && temp <= R) {
            stats.nearest_R.distances.push_back(temp);
            stats.nearest_R.offsets.push_back(j);
        }

        if (N != -1) {
            queue.push(pair<int, double>(j, temp));
        }
    }

    if (N != -1) {
        pair<int, double> nn;

        while (N > 0) {
            nn = queue.top();
            queue.pop();
            N--;

            stats.nearest_n.distance.push_back(nn.second); 
            stats.nearest_n.offset.push_back(nn.first);
        }
    }


    // --------------------------------
    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    stats.t = elapsed_secs; 

    return stats;
}

ApproximateNNStatisticsNR lsh_nearest_neighborNR(vector<X_TYPE> * q, struct HashTable * htables, int L, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, int N, double R) {
    ApproximateNNStatisticsNR stats = {0};

    clock_t begin = clock();

    priority_queue< pair<int, double>, vector<pair<int, double> >, ComparePairs> queue;


    for (int i = 0; i < L; i++) {
        ApproximateNNStatisticsNR partialstat = HashTableFindNN(&htables[i], W, M, m, k, s, m_vec, q, &queue, N, R);

        if (i == 0) {
            stats = partialstat;
        } else {
            if (stats.nearest_1.distance > partialstat.nearest_1.distance) {
                stats.nearest_1 = partialstat.nearest_1;
            }

            for (set<int>::iterator it = partialstat.nearest_R.offsets.begin(); it != partialstat.nearest_R.offsets.end(); ++it) {
                stats.nearest_R.offsets.insert(*it);

            }
        }
    }

    if (N != -1) {
        pair<int, double> nn;

        while (N > 0) {
            nn = queue.top();
            queue.pop();
            N--;

            stats.nearest_n.distance.push_back(nn.second); 
            stats.nearest_n.offset.push_back(nn.first);
        }
    }

    // --------------------------------
    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    stats.t = elapsed_secs; 

    return stats;
}

ApproximateNNStatisticsNR lsh_nearest_neighborR(vector<X_TYPE> * q, struct HashTable * htables, int L, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, double R) {
    ApproximateNNStatisticsNR stats = {0};

    clock_t begin = clock();

    for (int i = 0; i < L; i++) {
        ApproximateNNStatisticsNR partialstat = HashTableFindNN(&htables[i], W, M, m, k, s, m_vec, q, NULL, -1, R);

        if (i == 0) {
            stats = partialstat;
        } else {
            for (set<int>::iterator it = partialstat.nearest_R.offsets.begin(); it != partialstat.nearest_R.offsets.end(); ++it) {
                stats.nearest_R.offsets.insert(*it);
            }
        }
    }

    // --------------------------------
    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    stats.t = elapsed_secs; 

    return stats;
};