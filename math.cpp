#include <iostream>
#include <cmath>
#include <random>

#include "math.h"

std::random_device rand_dev;
std::mt19937 generator(rand_dev());

int modulo(int a, int m) {
    if (a % m < 0)
        return (int) a % m + m;
    else
        return (int) a % m;

}
//
//long modulo(long x, long y) { // return x mod y
//    long j;
//    if (x >= 0) {
//        return x % y;
//    } else {
//        long i = (abs(x) / y);
//        if (abs(x) % y != 0)
//            j = ((i + 1) * y) + x;
//        else
//            j = (i * y) + x;
//
//        return j;
//    }
//
//}
        
int modular_exponentiation(int x, int y, int z) { //Modular exponent
    int res = 1; // Initialize result  
     
    if (z == 1) {
        return 0;
    }

    x = x % z; // Update x if it is more than or  
    // equal to p 
//    if (x == 0) return 0; // In case x is divisible by p; 

    while (y > 0) {
        // If y is odd, multiply x with result  
        if (y % 2 == 1)
            res = (res * x) % z;

        // y must be even now  
        y = y >> 1; // y = y/2  
        x = (x * x) % z;
    }
    return res;

}

double random_inclusive(double min, double max) { //return random real number  in [ min, max]
    std::uniform_real_distribution<double> distr(min, max);
    return distr(generator);
}

double random_exclusive(double min, double max) { //return random real number  in [ min, max)
    std::uniform_real_distribution<double> distr(min, max - 0.00000000001); ///////// ???????????????????????????
    return distr(generator);
}

int ReverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8)&255;
    ch3 = (i >> 16)&255;
    ch4 = (i >> 24)&255;
    return ((int) ch1 << 24)+((int) ch2 << 16)+((int) ch3 << 8) + ch4;
}

int hammingdistance(int x1, int x2) { 
    int x = x1 ^ x2; 
    int bits = 0; 
  
    while (x > 0) { 
        bits += x & 1; 
        x >>= 1; 
    } 
  
    return bits; 
} 


vector<int> find_hammingN(int bitx,int x,int hd){ 
    vector<int> results;
    unsigned maxdec=0;
    
    if (hd == 0) {
        results.push_back(x);
        return results;
    }
    
    for(int i=0 ; i<bitx ; i++){
        maxdec = maxdec +pow(2,i);
    }
    
    for (unsigned i=0 ; i<=maxdec ; i++){
        if (hammingdistance(x,i)==hd){
            results.push_back(i);
        }
    }
   return results;
}