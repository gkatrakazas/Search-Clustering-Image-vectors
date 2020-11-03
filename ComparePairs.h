#ifndef COMPAREPAIRS_H
#define COMPAREPAIRS_H

struct ComparePairs { 
    bool operator()(pair<int,double> const& p1, pair<int,double> const& p2) { 
        return p1.second < p2.second; 
    } 
}; 

#endif /* COMPAREPAIRS_H */

