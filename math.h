#ifndef MATH_H
#define MATH_H

#include <vector>

using namespace std;

int modulo(int x, int y);

int modular_exponentiation(int x, int y, int z); //Modular exponent

double random_inclusive(double min, double max);

double random_exclusive(double min, double max);

int ReverseInt(int i);

int hammingdistance(int x1, int x2);

vector<int> find_hammingN(int bitx, int x, int hd);

#endif /* MATH_H */

