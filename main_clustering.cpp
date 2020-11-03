#include <iostream>
#include <fstream>
#include <vector>

#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ratio>
#include <vector>
#include <bits/stdc++.h>

#include "math.h"
#include "header.h"
#include "ReadMNIST.h"
#include "hashtable.h"
#include "Clustering.h" 


#define X_TYPE unsigned char
using namespace std;

//$./cluster –i <input file> –c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>
//  g++ -o main main_clustering.cpp ReadMNIST.cpp math.cpp
// ./clustering -i 10k-images.idx3-ubyte -c config_file.conf -o results_clustering -m Classic 

int main(int argc, char* argv[]) {
   
    char *input_file = NULL, *config_file = NULL, *output_file = NULL, *method = NULL;
    vector<vector < X_TYPE>> input;
    int number_of_vector_hash_functions = 4, number_of_vector_hash_tables = 5;
    double W = 42748;
    H_values s;
    vector<long int> m_vec;
    bool complete = false;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            input_file = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(input_file, argv[i + 1]);

        }
        if (strcmp(argv[i], "-c") == 0) {
            config_file = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(config_file, argv[i + 1]);

        }
        if (strcmp(argv[i], "-o") == 0) {
            output_file = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(output_file, argv[i + 1]);

        }
        if (strcmp(argv[i], "-complete") == 0) {
            complete = true;
        }
        if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "-method") == 0) {
            method = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(method, argv[i + 1]);
        }
    }

    if (method == NULL) {
        cout << "Method is required" << endl;
        return -1;
    }

    if (input_file == NULL) {
        cout << "Type the input filename: ";
        input_file = (char*) malloc(5000);
        cin >> input_file;
    }

    if (output_file == NULL) {
        cout << "Type the output filename: ";
        output_file = (char*) malloc(5000);
        cin >> output_file;
    }

    int number_of_clusters;
    int max_number_M_hypercube;
    int number_of_hypercube_dimensions;
    int number_of_probes;


    char temp_str[81], *token;  ///read clustering.conf
    ifstream from_file;
    from_file.open(config_file);
    if (!from_file) {
        cout << "Cannot open file ";
        exit(1); //return to the OS with exit code 1
    }
    cout << "This is the text in the file:\n";
    from_file.getline(temp_str, 80);
    while (!from_file.eof()) {
        cout << temp_str << "\n";

        token = strtok(temp_str, " \n\t");

        if (strcmp(token, "number_of_clusters:") == 0) number_of_clusters = atoi(strtok(NULL, " \n\t"));
        if (strcmp(token, "number_of_vector_hash_tables:") == 0) number_of_vector_hash_tables = atoi(token = strtok(NULL, " \n\t"));
        if (strcmp(token, "number_of_vector_hash_functions:") == 0) number_of_vector_hash_functions = atoi(token = strtok(NULL, " \n\t"));
        if (strcmp(token, "max_number_M_hypercube:") == 0) max_number_M_hypercube = atoi(token = strtok(NULL, " \n\t"));
        if (strcmp(token, "number_of_hypercube_dimensions:") == 0) number_of_hypercube_dimensions = atoi(token = strtok(NULL, " \n\t"));
        if (strcmp(token, "number_of_probes:") == 0) number_of_probes = atoi(token = strtok(NULL, " \n\t"));


        from_file.getline(temp_str, 80);
    }

    from_file.close();

    //cout << number_of_clusters << endl;
    //cout << number_of_vector_hash_tables << endl;
    //cout << number_of_vector_hash_functions << endl;
    //cout << max_number_M_hypercube << endl;
    //cout << number_of_hypercube_dimensions << endl;
    //cout << number_of_probes << endl;

    int M_hypercube = max_number_M_hypercube;
    int probes = number_of_probes;
    int NOC = number_of_clusters;

    int input_d = ReadMNIST(input_file, input);   ///read and store input

    if (W == 0) {
        cout << "Calculating W ... please wait. .. \n";
        W = find_W(&input); /// W=42.748
    }

    int k = number_of_vector_hash_functions;
    int M = 1 << (32 / k);
    int m = 1 << (M / 4);
    int L = number_of_vector_hash_tables;
    

    cout << " M is " << M << endl;
    cout << " m is " << m << endl;
    cout << "input d is: " << input_d << endl;

    s.data.resize(number_of_vector_hash_tables);

    for (int j1 = 0; j1 < L; j1++) {
        s.data[j1].resize(k);
        for (int j2 = 0; j2 < k; j2++) {
            s.data[j1][j2].resize(input_d);

            for (int j3 = 0; j3 < input_d; j3++) {
                s.data[j1][j2][j3] = random_exclusive(0, W);
            }
        }
    }

    int N = input.size();
    int T = N / 2;

    if (strcmp(method, "Hypercube") == 0) {
        T = (int) pow(2, k);
    }

    HashTable hashtables[L];

    for (int i = 0; i < L; i++) {
        if (strcmp(method, "LSH") == 0) {
            HashTableCreate(&hashtables[i], i, T);
        }
        if (strcmp(method, "Hypercube") == 0) {
            HashTableCreate(&hashtables[i], i, T, true, M_hypercube, probes);
        }
    }

    if (strcmp(method, "LSH") == 0 || strcmp(method, "Hypercube") == 0) {
        int d = input.at(0).size();
        for (int i = 0; i < d; i++) {
            long int me = modular_exponentiation(m, d - 1 - i, M);
            //        cout << me << endl;
            m_vec.push_back(me);
        }
    }

    cout << "Input file has " << input.size() << " vectors " << endl;

    if (strcmp(method, "LSH") == 0 || strcmp(method, "Hypercube") == 0) {
        for (unsigned i = 0; i < input.size(); i++) {
            if (i % 1000 == 0) {
                cout << i << endl;
            }
            for (int j = 0; j < L; j++) {
                HashTableInsert(&hashtables[j], W, M, m, k, &s, &m_vec, &input[i], i);
            }
        }
    }

    // =================================================================
    //                      Clustering algorithm
    // =================================================================


    vector<vector <X_TYPE>> *centers = ClusteringInitialization(&input, NOC);
    vector<vector <X_TYPE>> *nextcenters;
    vector<vector < int>> * assignments;
    
    int it = 1;

    clock_t begin = clock(); //start time 

    while (true) {   
        cout << "Iteration " << it++ << endl;
        if (it>15) break;
        //for (int i=0;i<NOC;i++) {
        //    cout << "C#" << i << ": ";
        //    for (unsigned j=0;j<centers->at(i).size() && j < 784;j++) {
        //        cout << (int) centers->at(i).at(j) << " ";
        //    }
        //    cout << endl;
        //}
        
        if (strcmp(method, "Classic") == 0) {
            assignments = ClusteringAssignmentLLoyd(&input, centers, NOC);
        }
         
        if (strcmp(method, "LSH") == 0 ) {
            assignments = ClusteringAssignmentLSH(&input, centers, NOC, hashtables, L, W ,M, m, k, &s, &m_vec);
        }
        
        if (strcmp(method, "Hypercube") == 0) {
            assignments = ClusteringAssignmentCUBE(&input, centers, NOC, hashtables, 1, W ,M, m, k, &s, &m_vec);
        }
        
        nextcenters = ClusteringUpdate(&input, centers, NOC, assignments);
        
        if (ClusteringFinishCondition(centers, nextcenters, NOC)) {
            break;
        } else {
            centers = nextcenters;
        }
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC; //end time
    
    
    vector<double> *silhouette=new vector<double> ();
    silhouette = ClusteringSilhouette(&input, centers, NOC, assignments,silhouette);
    
    ClusteringPrint(complete, &input, centers, NOC, assignments,elapsed_secs,method,silhouette,output_file);
    //cout<<"Results"<<endl;
    //for(int i=0;i<NOC+1;i++){
    //    cout<<silhouette->at(i)<<endl;
    //}
    //cout << "Silhouette: " << silhouette << endl;    
    


    if (strcmp(method, "LSH") == 0 || strcmp(method, "Hypercube") == 0) {
        for (int i = 0; i < L; i++) {
            HashTableDestroy(&hashtables[i]);
        }
    }


    if (input_file) {
        free(input_file);
    }

    if (config_file) {
        free(config_file);
    }
    if (output_file) {
        free(output_file);
    }
    if (method) {
        free(method);
    }

    return 0;
}
