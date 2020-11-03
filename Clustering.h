#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <iostream>
#include <vector>

#include "header.h"

using namespace std;

vector<vector < X_TYPE>> * ClusteringInitialization(vector<vector < X_TYPE>> * input , int k);

vector<vector < int>> * ClusteringAssignmentLLoyd(vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int k) ;

vector<vector < int>> * ClusteringAssignmentLSH(vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int NOC, struct HashTable * htables, int L,  double W, int M, int m, int k, H_values* s, vector<long int> * m_vec) ;

vector<vector < int>> * ClusteringAssignmentCUBE(vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int NOC, struct HashTable * htables, int L, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec);

vector<vector < X_TYPE>> *ClusteringUpdate(vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int k,vector<vector < int>> * assignments );

void ClusteringPrint(bool complete, vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int k,vector<vector < int>> * assignments, double elapsed_secs,char * method,vector<double> *sil,char* output_file);

vector<double>  *ClusteringSilhouette(vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int k,vector<vector < int>> * assignments,vector<double> * sil );

bool ClusteringFinishCondition( vector<vector < X_TYPE>> * prevcenters,  vector<vector < X_TYPE>> * nextcenters, int k);

int manhattan_distance2(vector<X_TYPE>* x1, vector<X_TYPE>* x2);

#endif /* CLUSTERING_H */

