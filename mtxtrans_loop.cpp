#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

int main(int argc, char * argv[]) {
  timespec start, end; // start and end of timer
  timespec start_o, end_o; // start and end of timer
  double runtime, runtime_o;  // run time in milliseconds

  // cout << argv[1] << endl;
  int mtx_row = 0; // atoi(argv[1]);
  int mtx_col = 0; // atoi(argv[2]);
  int blk = 4;    // atoi(argv[3]);

  ofstream datfile;
  datfile.open("mtxtrans_loop.dat");

  for(int rc = 100; rc <= 10000; rc += 100) {
    mtx_row = rc;
    mtx_col = rc;

    cout << "matrix row: " << mtx_row << endl;
    cout << "matrix col: " << mtx_col << endl;
    cout << "block size: " << blk     << endl;

    int * A;
    A = new int [mtx_row * mtx_col];

    for(int i = 0; i < mtx_row; i++) {
      for(int j = 0; j < mtx_col; j++) {
         A[mtx_col * i + j] = rand();
      }
    }

    /*
    cout << "A = " << endl;
    for(int i = 0; i < mtx_row; i++) {
      for(int j = 0; j < mtx_col; j++) {
        cout << A[mtx_col * i + j] << " ";
      }
      cout << endl;
    }
    */

    int * B;
    B = new int  [mtx_col * mtx_row];

    // timer
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < mtx_row; i++) {
      for(int j = 0; j < mtx_col; j++) {
         B[mtx_row * j + i]= A[mtx_col * i + j];
      }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    runtime = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6; // in milliseconds

    cout << "Run-time of Cache-ignorant: " << runtime << " milliseconds" << endl;

    int * AA;
    AA = new int  [mtx_row * mtx_col];

    for(int i = 0; i < mtx_row; i++) {
      for(int j = 0; j < mtx_col; j++) {
        AA[mtx_col * i + j]= rand();
      }
    }

    int * C;
    C = new int [mtx_col * mtx_row];

    clock_gettime(CLOCK_REALTIME, &start_o);
    for(int i = 0; i < mtx_row; i += blk) {
      for(int j = 0; j < mtx_col; j += blk) {
        for(int k = i; k < i + blk; k++) {
          for(int l = j; l < j + blk; l++) {
            C[mtx_row*l + k]= AA[mtx_col * k + l];
          }
        }
      }
    }
    clock_gettime(CLOCK_REALTIME, &end_o);
    runtime_o = (end_o.tv_sec - start_o.tv_sec) * 1E3 + (end_o.tv_nsec - start_o.tv_nsec) / 1E6; // in milliseconds

    cout << "Run-time of Cache-oblivious: " << runtime_o << " milliseconds" << endl;

    datfile << mtx_row * mtx_col << " ";
    datfile << runtime << " ";
    datfile << runtime_o << "\n";
    cout << endl;
  //cout << "A^T = " << endl;


    // timer
    /*
    for(int i = 0; i < mtx_col; i++) {
      for(int j = 0; j < mtx_row; j++) {
        cout << B[mtx_row * i + j] << " ";
      }
      cout << endl;
    }

    cout << endl;
    cout << "Cache-Oblivious A^T = " << endl;

    for(int i = 0; i < mtx_col ; i++) {
      for(int j = 0; j < mtx_row; j++) {
        cout << C[mtx_row * i + j] << " ";
      }
      cout << endl;
    }
    */

    /*
    for(int i = 0; i < mtx_row; i++) {
      A = new int [mtx_col];
      for(int j = 0; j < mtx_col; j++) {
        A[i][j] = j;
      }
    }
    */

    delete [] A;
    delete [] B;
    delete [] C;
    delete [] AA;
  }
  datfile.close();

  return 0;
}
