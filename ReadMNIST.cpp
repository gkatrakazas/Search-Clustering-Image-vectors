
#include "ReadMNIST.h"
#include "math.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


int ReadMNIST(char * input_file, vector<vector<X_TYPE>> &arr) {

    ifstream file(input_file, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;

        file.read((char*) &magic_number, sizeof (magic_number));
        magic_number = ReverseInt(magic_number);

        file.read((char*) &number_of_images, sizeof (number_of_images));
        number_of_images = ReverseInt(number_of_images);

        file.read((char*) &n_rows, sizeof (n_rows));
        n_rows = ReverseInt(n_rows);

        file.read((char*) &n_cols, sizeof (n_cols));
        n_cols = ReverseInt(n_cols);

        arr.resize(number_of_images, vector<X_TYPE>(n_rows * n_cols));

        int d = n_rows * n_cols;

        cout << "magic_number= " << magic_number << endl;
        cout << "number_of_images= " << number_of_images << endl;
        cout << "n_rows= " << n_rows << endl;
        cout << "n_cols= " << n_cols << endl;

        /////pinakas me ola ta pixel kathe eikonas
        for (int i = 0; i < number_of_images; ++i) {

            for (int r = 0; r < n_rows; ++r) {
                for (int c = 0; c < n_cols; ++c) {
                    unsigned char temp = 0;
                    file.read((char*) &temp, sizeof (temp));
                    arr[i][(n_rows * r) + c] = (X_TYPE) temp;
                    //cout<<arr[i][(n_rows * r) + c]<<endl;

                }
            }
        }

        return d;
    } else {
        return 0;
    }
}