#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <list>
#include <queue>
#include <unordered_map>

#include "header.h"
#include "ApproximateNNStatistics.h"
#include "ComparePairs.h"

using namespace std;

struct HashTable {
    int id;
    int size;
    list<ImagePairg*> ** lists;
    bool useFmapping;
    
    vector<unordered_map<int,int>> *f;
    int Mcube;
    int probes;
};

void HashTableCreate(struct HashTable * htable, int id, int size, bool useFmapping = false, int Mcube = 0, int probes = 0) ;

void HashTableDestroy(struct HashTable * htable) ;

void HashTableInsert(struct HashTable * htable, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, vector < X_TYPE> * ptr, int offset);

ApproximateNNStatisticsNR HashTableFindNN(struct HashTable * htable, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec,  vector < X_TYPE> * q, priority_queue< pair<int, double>, vector<pair<int, double> >, ComparePairs> *queue, int N = -1, double R = -1) ;



#endif /* HASHTABLE_H */

