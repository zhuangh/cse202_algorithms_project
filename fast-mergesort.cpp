#include "sort.h"

// ascending sort 0, 1, 2, ...
void merge(elem_t* array, int indexl, int indexm, int indexr) {
  int i, j, k;

  elem_t* clone;
  clone = new elem_t [indexr - indexl + 1];
  //for (i = indexl; i <= indexr; i++) clone[i - indexl] = array[i];
  //memcpy(clone, array + indexl, (indexr - indexl + 1) * sizeof(elem_t));
  std::copy(array + indexl, array + indexr + 1, clone);

  for(i = 0, j = indexm - indexl + 1, k = indexl; k <= indexr; k++) {
    if(i == indexm - indexl + 1) { array[k] = clone[j++]; continue; }
    if(j == indexr - indexl + 1) { array[k] = clone[i++]; continue; }
    array[k] = (clone[i] < clone[j]) ? clone[i++] : clone[j++];
  }

  delete [] clone;
}

void mergesort(elem_t* array, int indexl, int indexr) {
  if (indexr <= indexl) return; // sorted

  int indexm = (indexl + indexr) / 2;

#ifdef TH
  if(indexr - indexl < TH) {
    sort(array + indexl, array + indexr + 1);
  } else {
    mergesort(array, indexl,     indexm);
    mergesort(array, indexm + 1, indexr);
    merge(array, indexl, indexm, indexr);
  }
#else
  mergesort(array, indexl,     indexm);
  mergesort(array, indexm + 1, indexr);
  merge(array, indexl, indexm, indexr);
#endif
}

int main() {
  timespec start, end; // start and end of timer
  double runtime;      // run time in milliseconds

  elem_t* test_array;
  elem_t* ref_array;

  ofstream datfile;
  datfile.open("fast-mergesort.dat");

  int nsize = NMIN;

  while(nsize < NMAX) { // 2 ^ 25
    test_array = new elem_t [nsize];
    ref_array = new elem_t [nsize];

    for(int i = 0; i < nsize; i++) {
      test_array[i] = rand();
      ref_array[i] = test_array[i];
    }
    sort(ref_array, ref_array + nsize);

#ifdef PRINT_ARRAY
    cout << "Input array is: ";
    print_array(test_array, nsize);
    cout << endl;
#endif

    clock_gettime(CLOCK_REALTIME, &start);
    mergesort(test_array, 0, nsize - 1);
    clock_gettime(CLOCK_REALTIME, &end);
    runtime = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6; // in milliseconds

#ifdef PRINT_ARRAY
    cout << "Sorted array is: ";
    print_array(test_array, nsize);
    cout << endl;
#endif

#ifdef VERIFY_RESULT
    int error;
    error = 0;
    for(int i = 0; i < nsize; i++) { // simple verification
      if(test_array[i] != ref_array[i]) {
        error++;
      }
    }
#endif

    cout << "Run-time of mergesort: " << runtime << " milliseconds" << endl;

    delete [] test_array;

#ifdef VERIFY_RESULT
    if(error) {
      cout << "Error: Mergesort has " << error << " errors!" << endl;
    } else {
      cout << "Success: Mergesort has finished successfully." << endl;
    }
#endif
    datfile << nsize << " ";
    datfile << runtime << "\n";

    nsize = nsize * 2;
  }

  datfile.close();

  return 0;
}
