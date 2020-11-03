#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "Clustering.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <climits>

using namespace std;

vector<vector< X_TYPE>> *ClusteringInitialization(vector<vector < X_TYPE>> *input, int k) {
    vector<vector < X_TYPE>> *centers = new vector<vector < X_TYPE >> ();
    unsigned N = input->size();
    double Pr[N] = {0};

    for (int i = 0; i < k; i++) {
        if (i == 0) { //An einai to prwto kentro to vriskei tuxaia
            int c = rand() % N;
            centers->push_back(input->at(c));
            Pr[c] = -1; //Exei mpei san kentro i eikona c
        } else {
            int latest = -1;
            double sum = 0;

            for (unsigned i = 0; i < N; i++) {
                if (Pr[i] == -1) {
                    continue;
                } else {
                    TrueNNStatistics nearestcenter = true_nearest_neighbor(&(*input)[i], centers, -1);

                    if (latest == -1) {
                        Pr[i] = nearestcenter.distance * nearestcenter.distance;
                    } else {
                        Pr[i] = nearestcenter.distance * nearestcenter.distance + Pr[latest];
                        latest = i;
                    }

                    sum = Pr[i];
                }
            }

            double propability = random_exclusive(0, sum);


            for (unsigned i = 0; i < N; i++) {
                if (propability <= Pr[i] && Pr[i] != -1) {
                    int c = i;
                    centers->push_back(input->at(c));
                    Pr[c] = -1;
                    break;
                }
            }
        }
    }

    return centers;
}

vector<vector < int>> *ClusteringAssignmentLLoyd(vector<vector < X_TYPE>> *input, vector<vector < X_TYPE>> *centers, int k) {
    vector<vector < int>> *assignments = new vector<vector < int >> ();

    unsigned N = input->size();

    for (int i = 0; i < k; i++) {
        assignments->push_back(vector< int>());
    }

    for (unsigned i = 0; i < N; i++) {
        TrueNNStatistics nearestcenter = true_nearest_neighbor(&(*input)[i], centers, -1);

        assignments->at(nearestcenter.offset).push_back(i);


    }

    return assignments;
}


// vector<X_TYPE> * q, struct HashTable * htables, int L,  double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, double R

vector<vector < int>> *ClusteringAssignmentLSH(vector<vector < X_TYPE>> *input, vector<vector < X_TYPE>> *centers, int NOC,
        struct HashTable * htables, int L, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec) {
    vector<vector < int>> *assignments = new vector<vector < int >> ();

    for (unsigned i = 0; i < centers->size(); i++) {
        assignments->push_back(vector< int>());
    }

    double ww = find_W(centers);

    double R = ww / 2;

    double maxdist = 2000000;

    cout << "R = " << R << endl;

    vector<pair<int, double>> p;
    p.resize(input->size());

    for (unsigned i = 0; i < input->size(); i++) {
        p[i].first = -1; // to which centroid
        p[i].second = 0.0; // at which radious
    }

    //    vector<int> warzone;

    while (R < maxdist) {
        int discoveries = 0;

        for (unsigned c = 0; c < centers->size(); c++) {
            ApproximateNNStatisticsNR stats = lsh_nearest_neighborR(&(*centers)[c], htables, L, W, M, m, k, s, m_vec, R);

            for (auto offset_x : stats.nearest_R.offsets) {
                if (p[offset_x].first == (int) c) {
                    continue;
                }
                if (p[offset_x].first == -1) {
                    p[offset_x].first = c;
                    p[offset_x].second = R;
                    maxdist++;
                } else if (p[offset_x].first != -2) {
                    if (p[offset_x].second < R) {
                        // NOP
                    } else {
                        p[offset_x].first = -2;

                        //                        warzone.push_back(offset_x);
                    }
                }
            }
        }

        if (discoveries < 1) {
            break;
        }

        R = R * 2;
    }

    //    for (unsigned i = 0; i < warzone.size(); i++) {
    //        TrueNNStatistics nearestcenter = true_nearest_neighbor(&(*input)[i], centers, -1);
    //        assignments->at(nearestcenter.offset).push_back(i);
    //    }

    for (unsigned i = 0; i < input->size(); i++) {
        if (p[i].first == -1 || p[i].first == -2) {
            TrueNNStatistics nearestcenter = true_nearest_neighbor(&(*input)[i], centers, -1);
            assignments->at(nearestcenter.offset).push_back(i);
        } else {
            assignments->at(p[i].first).push_back(i);
        }
    }


    return assignments;
}

vector<vector < int>> * ClusteringAssignmentCUBE(vector<vector < X_TYPE>> * input, vector<vector < X_TYPE>> * centers, int NOC, 
        struct HashTable * htables, int L, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec) {
    vector<vector < int>> *assignments = new vector<vector < int >> ();

    for (unsigned i = 0; i < centers->size(); i++) {
        assignments->push_back(vector< int>());
    }

    double ww = find_W(centers);

    double R = ww / 2;

    double maxdist = 2000000;

    cout << "R = " << R << endl;

    vector<pair<int, double>> p;
    p.resize(input->size());

    for (unsigned i = 0; i < input->size(); i++) {
        p[i].first = -1; // to which centroid
        p[i].second = 0.0; // at which radious
    }

    //    vector<int> warzone;

    while (R < maxdist) {
        int discoveries = 0;

        for (unsigned c = 0; c < centers->size(); c++) {
            ApproximateNNStatisticsNR stats = lsh_nearest_neighborR(&(*centers)[c], htables, L, W, M, m, k, s, m_vec, R);

            for (auto offset_x : stats.nearest_R.offsets) {
                if (p[offset_x].first == (int) c) {
                    continue;
                }
                if (p[offset_x].first == -1) {
                    p[offset_x].first = c;
                    p[offset_x].second = R;
                    maxdist++;
                } else if (p[offset_x].first != -2) {
                    if (p[offset_x].second < R) {
                        // NOP
                    } else {
                        p[offset_x].first = -2;

                        //                        warzone.push_back(offset_x);
                    }
                }
            }
        }

        if (discoveries < 1) {
            break;
        }

        R = R * 2;
    }

    //    for (unsigned i = 0; i < warzone.size(); i++) {
    //        TrueNNStatistics nearestcenter = true_nearest_neighbor(&(*input)[i], centers, -1);
    //        assignments->at(nearestcenter.offset).push_back(i);
    //    }

    for (unsigned i = 0; i < input->size(); i++) {
        if (p[i].first == -1 || p[i].first == -2) {
            TrueNNStatistics nearestcenter = true_nearest_neighbor(&(*input)[i], centers, -1);
            assignments->at(nearestcenter.offset).push_back(i);
        } else {
            assignments->at(p[i].first).push_back(i);
        }
    }


    return assignments;
}

vector<vector < X_TYPE>> *ClusteringUpdate(vector<vector < X_TYPE>> *input, vector<vector < X_TYPE>> *centers, int k, vector<vector < int>> *assignments) {
    vector<vector < X_TYPE>> *nextcenters = new vector<vector < X_TYPE >> ();

    unsigned D = centers->at(0).size();

    for (int i = 0; i < k; i++) { // for each cluster
        unsigned N_cluster = assignments->at(i).size();
        vector<vector < X_TYPE>> coordinates;
        
        for (unsigned d = 0; d < D; d++) {
            coordinates.push_back(vector < X_TYPE>());
        }

        for (unsigned j = 0; j < N_cluster; j++) { // for each point in that cluster
            int pointid = assignments->at(i).at(j);

            for (unsigned d = 0; d < D; d++) {
                coordinates[d].push_back(input->at(pointid)[d]);
            }
        }

        vector < X_TYPE> newcenter;

        for (unsigned d = 0; d < D; d++) {

            sort(coordinates[d].begin(), coordinates[d].end());

            X_TYPE median = coordinates[d][N_cluster / 2];

            newcenter.push_back(median);
        }

        nextcenters->push_back(newcenter);
    }

    return nextcenters;
}

void ClusteringPrint(bool complete, vector<vector < X_TYPE>> *input, vector<vector < X_TYPE>> *centers, int k, vector<vector < int>> *assignments, double elapsed_secs, char * method,vector<double> *sil,char *output_file) {
    
    FILE *log;
    char log_name[40];
    strcpy(log_name,output_file);
    
    strcat(log_name,".txt");
    log=fopen(log_name,"w");
    
    if (strcmp(method, "Classic") == 0) {
        cout << "\nAlgorithm:  Lloyds" << endl;
        fprintf(log,"Algorithm:  Lloyds \n");
    } else if (strcmp(method, "LSH") == 0) {
        cout << "\nAlgorithm:  Range Search LSH" << endl;
        fprintf(log,"Algorithm:  Range Search LSH \n");
    } else {
        cout << "\nAlgorithm: Range Search Hypercube" << endl;
        fprintf(log,"Algorithm:  Range Search Hypercube \n");
    }

    for (int i = 0; i < k; i++) {
        cout << "\nCLUSTER-" << i << " size:" << assignments->at(i).size() << endl;
        fprintf(log,"\nCLUSTER-%d  size: %ld\n",i,assignments->at(i).size());
        fprintf(log,"Centroid: ");
        for (unsigned j = 0; j < centers->at(i).size(); j++) {
            
            cout << (int) centers->at(i).at(j) << " ";
            fprintf(log,"%d ",(int) centers->at(i).at(j));
        }
        fprintf(log,"\n");
    }

    cout << "\nclustering_time: " << elapsed_secs << " seconds" << endl;
    fprintf(log,"clustering_time: %f seconds\n",elapsed_secs);

    //fprintf(log,"[");
     fprintf(log," *Silhouette* \n");
    for(int i=0;i<k;i++){
        //cout<<sil->at(i)<<endl;
        //fprintf(log," %f ,",sil->at(i));
        fprintf(log," Centroid-%d s= %f \n",i,sil->at(i));
    }
    //fprintf(log," %f ]",sil->at(k+1));
    fprintf(log," Stotal= %f \n",sil->at(k));

}

int manhattan_distance2(vector<X_TYPE>* x1, vector<X_TYPE>* x2) {
    int dx1 = x1->size();
    int sum = 0;

    for (int i = 0; i < dx1; i++) {

        sum = sum + abs((int) ((*x1)[i]) - (int) ((*x2)[i]));
    }

    return (sum);
}

vector<double> *ClusteringSilhouette(vector<vector < X_TYPE>> *input, vector<vector < X_TYPE>> *centers, int k, vector<vector <int>> *assignments, vector<double> * sil ) {
    clock_t begin = clock();
    

    double sum = 0, sum2 = 0, sum_total = 0,max,temp;

    for (int c = 0; c < k; c++) {
        cout << "center : " << c << "  size: " << (*assignments)[c].size() << endl;
        temp=0;
        for (unsigned j = 0; j < assignments->at(c).size(); j++) {
            sum = 0;
            sum2 = 0;
            //cout << "center : " << c <<"  id: "<< j << endl;
            int pointid = assignments->at(c).at(j);

            int min_center = -1;
            int min_distance = INT_MAX;
            for (int c2 = 0; c2 < k; c2++) {
                if (c == c2) {
                    continue;
                }
                int dist = manhattan_distance2(&(*centers)[c], &(*centers)[c2]);
                if (dist < min_distance) {
                    min_distance = dist;
                    min_center = c2;
                }
            }

            for (unsigned j2 = 0; j2 < assignments->at(c).size(); j2++) {
                int pointid2 = assignments->at(c).at(j2);
                sum = sum + manhattan_distance2(&(*input)[pointid2], &(*input)[pointid]);
            }

            for (unsigned j2 = 0; j2 < assignments->at(min_center).size(); j2++) {
                int pointid2 = assignments->at(min_center).at(j2);
                sum2 = sum2 + manhattan_distance2(&(*input)[pointid2], &(*input)[pointid]);
            }

            sum = sum / (assignments->at(c).size() - 1);
            sum2 = sum2 / assignments->at(min_center).size();
            max = sum;
            if (sum2 > sum) max = sum2;
            sum_total = sum_total + ((sum2 - sum) / max);
            temp = temp + ((sum2 - sum) / max);
            
        }

        
        
    cout<< "silhuette : "<<c <<" is:"<<temp/assignments->at(c).size()<<endl; 
    sil->push_back( double(temp/assignments->at(c).size()));
    //cout<<"ccccccc"<<sil->at(c)<<endl;
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Silhouette time = " << elapsed_secs << endl;
    //return sum_total / input->size();
    sil->push_back(sum_total / input->size());
    return sil;

}

bool ClusteringFinishCondition(vector<vector < X_TYPE>> *prevcenters, vector<vector < X_TYPE>> *nextcenters, int k) {

    for (int i = 0; i < k; i++) {
        double distance = manhattan_distance(&prevcenters->at(i), &nextcenters->at(i));
        if ((distance) > 100) {
            return false;
        }
    }
    return true;
}
