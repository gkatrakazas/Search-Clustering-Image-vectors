#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ratio>
#include <complex>

//#include "header.h"
#include "math.h"
#include "hashtable.h"
#include "ApproximateNNStatistics.h"
#include "ReadMNIST.h"

using namespace std;

// $./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
// $./cube -d train-images.idx3-ubyte -q t10k-images.idx3-ubyte -k 4 -M 10 -probes 100 -o results_cube -Ν 5 -R 100000

//  g++ -o main main_lsh.cpp lsh_function.cpp math.cpp hashtable.cpp
// ./cube -d train-images.idx3-ubyte -q t10k-images.idx3-ubyte -k 3 -L 4 -o results_cube -Ν 5 -R 2

int main(int argc, char* argv[]) {

    char *input_file = NULL, *query_file = NULL, *output_file = NULL;
    int k = 4, L = 1, number_NN = 1; // LSH
    int M_hypercube = 10000, probes = 1000; // cube
    double W = 42748, radius = 1.0;
    vector<vector < X_TYPE>> input;
    H_values s;
    vector<long int> m_vec;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            input_file = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(input_file, argv[i + 1]);
            //cout<<sizeof(input_file)<<endl;
            //cout<<(input_file)<<endl;
        }
        if (strcmp(argv[i], "-q") == 0) {
            query_file = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(query_file, argv[i + 1]);
            //cout<<sizeof(query_file)<<endl;
            //cout<<(query_file)<<endl;
        }
        if (strcmp(argv[i], "-k") == 0) {
            k = atoi(argv[i + 1]);

        }
        if (strcmp(argv[i], "-M") == 0) {
            M_hypercube = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-probes") == 0) {
            probes = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-o") == 0) {
            output_file = (char*) malloc(strlen(argv[i + 1]) * sizeof (char) + 1 * sizeof (char));
            strcpy(output_file, argv[i + 1]);
            //cout<<sizeof(output_file)<<endl;
            //cout<<(output_file)<<endl;
        }
        if (strcmp(argv[i], "-N") == 0) {
            number_NN = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-R") == 0) {
            radius = atoi(argv[i + 1]);
        }
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

    int input_d = ReadMNIST(input_file, input);

    if (W == 0) {
        cout << "Calculating W ... please wait. .. \n";
        W = find_W(&input); /// W=42.748
    }

    int M = 1 << (32 / k);
    int m = 1 << M / 4;

    cout << " M is " << M << endl;
    cout << " m is " << m << endl;
    cout << "input d is: " << input_d << endl;

    s.data.resize(L);

    for (int j1 = 0; j1 < L; j1++) {
        s.data[j1].resize(k);
        for (int j2 = 0; j2 < k; j2++) {
            s.data[j1][j2].resize(input_d);

            for (int j3 = 0; j3 < input_d; j3++) {
                s.data[j1][j2][j3] = random_exclusive(0, W);
            }
        }
    }

    int T = (int) pow(2, k);

    HashTable hashtables[L];

    for (int i = 0; i < L; i++) {
        HashTableCreate(&hashtables[i], i, T, true, M_hypercube, probes);
    }

    cout << "Calculating ... m_vec ...\n";
    int d = input.at(0).size();
    for (int i = 0; i < d; i++) {
        long int me = modular_exponentiation(m, d - 1 - i, M);
        //        cout << me << endl;
        m_vec.push_back(me);
    }
    //    exit(1);

    cout << "Input file has " << input.size() << " vectors " << endl;

    cout << "test hamming" << endl;
    int hd = hammingdistance(9, 14);
    cout << "hm : " << hd << endl;

    vector<int> res;
    res = find_hammingN(16, 50, 3);
    cout << "find hm : " << res.size() << endl;
    //exit(1);


    cout << "Inserting to hashtables ..." << endl;
    for (unsigned i = 0; i < input.size(); i++) {
        if (i % 1000 == 0) {
            cout << i << endl;
        }
        for (int j = 0; j < L; j++) {
            HashTableInsert(&hashtables[j], W, M, m, k, &s, &m_vec, &input[i], i);
        }
    }

    bool repeat = (query_file == NULL);

    do {
        if (query_file == NULL) {
            cout << "Type the query filename: ";
            query_file = (char*) malloc(5000);
            cin >> query_file;

            if (strlen(query_file) == 0) {
                break;
            }
        }

        vector<vector < X_TYPE>> query;

        int query_d = ReadMNIST(query_file, query);

        cout << "query d is: " << query_d << endl;

        if (input_d != query_d) {
            cout << "incompatible data \n";
            continue;
        }


        cout << "Query file has " << query.size() << " vectors " << endl;

        cout << "Starting CUBE ... " << endl;

        // TrueNNStatisticsNR true_nearest_neighborNR(vector<X_TYPE> * q, vector<vector<X_TYPE>> *X, int N, double R) 

        FILE *log;
        char log_name[40];
        strcpy(log_name, output_file);

        strcat(log_name, ".txt");
        log = fopen(log_name, "w");

        for (unsigned i = 0; i < query.size() && i < 30; i++) { // for each query
            TrueNNStatisticsNR stats = true_nearest_neighborNR(&query[i], &input, number_NN);

            ApproximateNNStatisticsNR approxstats = lsh_nearest_neighborNR(&query[i], hashtables, L, W, M, m, k, &s, &m_vec, number_NN, radius);
            
            cout << "Query: " << i << endl;
            for (int i = 0; i < number_NN; i++) {
                if (i == 0) {
                   
                    cout << "Nearest neighbor-1 (true): " << stats.nearest_1.offset << "  ( " << stats.nearest_1.distance << ")" << endl;
                    cout << "Nearest neighbor-1 (cube): " << approxstats.nearest_1.offset << "  (" << approxstats.nearest_1.distance << ")" << endl;
                } else {
                    if (stats.nearest_n.offset.size() > (unsigned) i) {
                        cout << "Nearest neighbor-" << (i + 1) << " (true): " << stats.nearest_n.offset[i] << "  (" << stats.nearest_n.distance[i] << ")" << endl;
                    }
                    if (approxstats.nearest_n.offset.size() > (unsigned) i) {
                        cout << "Nearest neighbor-" << (i + 1) << " (lsh): " << approxstats.nearest_n.offset[i] << "  (" << approxstats.nearest_n.distance[i] << ")" << endl;
                    }
                }
            }



            cout << "tTrue: " << stats.t << endl;
            cout << "tCUBE: " << approxstats.t << endl;
            cout << "tTrue/tcube: " << stats.t / approxstats.t << endl;

            cout << "CUBE R-near neighbors: saved in file" << endl;
            
            fprintf(log, "Query: %d \n\n", i);
            for (int i = 0; i < number_NN; i++) {
                if (i == 0) {
                    //cout << "Query: " << i << endl;
                    //cout << "Nearest neighbor-1 (true): " << stats.nearest_1.offset << "  ( " << stats.nearest_1.distance << ")" << endl;
                    //cout << "Nearest neighbor-1 (cube): " << approxstats.nearest_1.offset << "  (" << approxstats.nearest_1.distance << ")" << endl;

                    
                    fprintf(log, "Nearest neighbor-1: %d \n", approxstats.nearest_1.offset);
                    fprintf(log, "distanceCube: %f \n", approxstats.nearest_1.distance);
                    fprintf(log, "distanceTrue: %f \n\n", stats.nearest_1.distance);
                } else {
                    if (stats.nearest_n.offset.size() > (unsigned) i) {
                        //cout << "Nearest neighbor-" << (i + 1) << " (true): " << stats.nearest_n.offset[i] << "  (" << stats.nearest_n.distance[i] << ")" << endl;

                        fprintf(log, "Nearest neighbor-%d: %d \n", i + 1, stats.nearest_n.offset[i]);
                        fprintf(log, "distanceTrue: %f \n\n", stats.nearest_n.distance[i]);
                    }
                    if (approxstats.nearest_n.offset.size() > (unsigned) i) {
                        //cout << "Nearest neighbor-" << (i + 1) << " (lsh): " << approxstats.nearest_n.offset[i] << "  (" << approxstats.nearest_n.distance[i] << ")" << endl;

                        fprintf(log, "Nearest neighbor-%d: %d \n", i + 1, approxstats.nearest_n.offset[i]);
                        fprintf(log, "distanceCube: %f \n", approxstats.nearest_n.distance[i]);
                    }
                }
            }



            fprintf(log, "tCUBE: %f \n", approxstats.t);
            fprintf(log, "tTrue: %f \n\n", stats.t);

            fprintf(log, "R-near neighbors:\n");
            bool flag = 0;
            for (auto it = approxstats.nearest_R.offsets.begin(); it != approxstats.nearest_R.offsets.end(); ++it) {
                flag = 1;
                fprintf(log, " %d \n", (*it));

            }
            if (flag == 0) {
                fprintf(log, " --- \n");
            }

        }
        fclose(log);


        if (repeat == true) {
            delete [] query_file;
            query_file = NULL;
        }

    } while (repeat == true);





    int value = manhattan_distance(&input[0], &input[1]);
    cout << value << endl;
    //value= manhattan_distance(&ar[0],&ar[2]);
    //cout<<value<<endl;

    // !!!
    //    for (int i=0;i<L;i++) {
    //        HashTableDestroy(&hashtables[i]);
    //    }

    //cout<< modulo(-19, 3)<<endl;

    delete [] input_file;
    delete [] output_file;

    if (query_file != NULL) {
        delete [] query_file;
    }
    //cout<<"W = "<<W<<endl;


    return 0;
}
