#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <algorithm>

typedef long elem_t;

using namespace std;

void print_array(elem_t* array, int size) {
  for(int i = 0; i < size; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
}
