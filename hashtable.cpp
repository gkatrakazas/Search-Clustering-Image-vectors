
#include <iostream>
#include <queue>
#include <ctime>
#include <set>

#include "hashtable.h"
#include "ApproximateNNStatistics.h"
#include "ComparePairs.h"
#include "math.h"

using namespace std;

void HashTableCreate(struct HashTable * htable, int id, int size, bool useFmapping, int Mcube, int probes) {
    htable->size = size;
    htable->id = id;
    htable->useFmapping = useFmapping;
    htable->Mcube = Mcube;
    htable->probes = probes;

    htable->lists = new list<ImagePairg*> *[size];

    for (int i = 0; i < size; i++) {
        htable->lists[i] = new list<ImagePairg*>();
    }

    if (useFmapping) {
        htable->f = new vector<unordered_map<int, int>>();
        htable->f->resize(size);
    } else {
        htable->f = NULL;
    }
}

void HashTableDestroy(struct HashTable * htable) {

    for (int i = 0; i < htable->size; i++) {
        delete htable->lists[i];
    }

    delete [] htable->lists;
}

int fi(struct HashTable * htable, int h, int i) {
    if ((*htable->f)[i].find(h) == (*htable->f)[i].end()) {
        (*htable->f)[i][h] = rand() % 2;
    }
    return (*htable->f)[i][h];
}

void HashTableInsert(struct HashTable * htable, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, vector < X_TYPE> * imageptr, int offset) {
    vector<unsigned> indices;

    // hash:
    for (int i = 0; i < k; i++) {
        unsigned index = h(W, M, m, &s->data[htable->id][i], m_vec, imageptr);

        if (htable->useFmapping == false) {
            indices.push_back(index);
        } else {
            indices.push_back(fi(htable, index, i));
        }
    }

    unsigned lsh_index = g(indices, k);

    //    cout << "lsh_index: " << lsh_index << endl;

    ImagePairg * pair = new ImagePairg;
    pair->ptr = imageptr;
    pair->g = lsh_index;
    pair->offset = offset;

    int table_index = lsh_index % htable->size;

    htable->lists[table_index]->push_back(pair);
}

ApproximateNNStatisticsNR HashTableFindNN(struct HashTable * htable, double W, int M, int m, int k, H_values* s, vector<long int> * m_vec, vector < X_TYPE> * q, priority_queue< pair<int, double>, vector<pair<int, double> >, ComparePairs> *queue, int N, double R) {
    ApproximateNNStatisticsNR stats = {0};

    vector<unsigned> indices;

    for (int i = 0; i < k; i++) {
        unsigned index = h(W, M, m, &s->data[htable->id][i], m_vec, q);

        if (htable->useFmapping == false) {
            indices.push_back(index);
        } else {
            indices.push_back(fi(htable, index, i));
        }
    }

    unsigned lsh_index = g(indices, k);

    unsigned table_index = lsh_index % htable->size;

    int j = 0;

    // --------------------------------

    if (htable->useFmapping == false) {                                 ///Eimaste sto lsh
        list<ImagePairg*> * templist = htable->lists[table_index];

        for (list<ImagePairg*>::iterator it = templist->begin(); it != templist->end(); ++it) {
            if ((*it)->g == lsh_index) {
                double temp = manhattan_distance(q, (*it)->ptr);
                if (temp < stats.nearest_1.distance || j++ == 0) {
                    stats.nearest_1.distance = temp;
                    stats.nearest_1.offset = (*it)->offset;
                    
                }
                //cout << "TO R EINAI : " << R << "KAI TEMP EINAI :" << temp << endl;
                if (R != -1 && temp <= R) {
                    //            stats.nearest_R.distances.push_back(temp);
                   
                    stats.nearest_R.offsets.insert((*it)->offset);
                }

                if (N != -1 && queue != NULL) {
                    queue->push(pair<int, double>((*it)->offset, temp));
                }
            }
        }
  
    } else {                                                ///Eimaste sto cube
        int examined = 0;
        int examined_buckets = 0;
        for (int hd = 0; hd < htable->size; hd++) {
            vector<int> v = find_hammingN(htable->size, table_index, hd);

            for (auto x : v) {
                list<ImagePairg*> * templist = htable->lists[x];

                for (list<ImagePairg*>::iterator it = templist->begin(); it != templist->end(); ++it) {
                    double temp = manhattan_distance(q, (*it)->ptr);
//                    cout<< temp <<" < "<< stats.nearest_1.distance <<"  j "<< j<<endl; 
                    if (temp < stats.nearest_1.distance || j++ == 0) {
                        stats.nearest_1.distance = temp;
                        stats.nearest_1.offset = (*it)->offset;
                    }

                    if (R != -1 && temp <= R) {
                        //            stats.nearest_R.distances.push_back(temp);
                        stats.nearest_R.offsets.insert((*it)->offset);
                    }

                    if (N != -1 && queue != NULL) {
                        queue->push(pair<int, double>((*it)->offset, temp));
                    }

                    examined++;

                    if (examined > htable->Mcube) {
                        break;
                    }
                }
            }

            examined_buckets++;

            if (examined > htable->Mcube || examined_buckets > htable->probes) {
                break;
            }
        }
    }

    return stats;
}
