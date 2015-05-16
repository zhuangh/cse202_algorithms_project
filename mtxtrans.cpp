#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>

using namespace std;

int main(int argc, char * argv[]) {

  // cout << argv[1] << endl;
  int mtx_row = atoi(argv[1]);
  int mtx_col = atoi(argv[2]);
  int blk = atoi(argv[3]);

  cout << "matrix row: " << mtx_row << endl;
  cout << "matrix col: " << mtx_col << endl;
  cout << "block size: " << blk     << endl;

  int * A;
  A = new int  [mtx_row * mtx_col];

  for(int i = 0; i < mtx_row; i++) {
    for(int j = 0; j < mtx_col; j++) {
      A[mtx_col * i + j] = j;
    }
  }

  /*
  cout << "A = " << endl;
  for(int i = 0; i < mtx_row; i++) {
    for(int j = 0; j < mtx_col; j++) {
      cout << A[mtx_col*i+j] << " ";
    }
    cout << endl;
  }
  */

  int * B;
  B = new int  [mtx_col * mtx_row];

  clock_t start, end;

  // timer
  start = clock();
    for(int i = 0; i < mtx_row; i++) {
      for(int j = 0; j < mtx_col; j++) {
        B[mtx_row * j + i]= A[mtx_col*i + j];
    }
  }
  end = clock();

  cout << " Cache ignorance " << (((double) (end - start)) / CLOCKS_PER_SEC) << "seconds" << "\n";
  // timer


  int * AA;
  AA = new int  [mtx_row * mtx_col];

  for(int i = 0; i < mtx_row; i++) {
    for(int j = 0; j < mtx_col; j++) {
      AA[mtx_col*i+j]= j;
    }
  }

  int * C;
  C = new int  [mtx_col * mtx_row];

  clock_t start_o, end_o;
  start_o = clock();

  for(int i = 0; i < mtx_row; i += blk) {
    for(int j = 0; j < mtx_col; j += blk) {
      for(int k = i; k < i + blk; k++) {
        for(int l = j; l < j + blk; l++) {
          C[mtx_row * l + k]= AA[mtx_col * k + l];
        }
      }
    }
  }

  end_o = clock();

  cout << " Cache oblivious " << (((double) (end_o - start_o)) / CLOCKS_PER_SEC) << "seconds" << "\n";

  cout << endl;
  cout << "A^T = " << endl;

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

  for(int i = 0; i < mtx_col; i++) {
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

  return 0;
}
