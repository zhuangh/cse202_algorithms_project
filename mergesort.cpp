#include "sort.h"

// funnel buffer structure
typedef struct funnelbuf {
  elem_t* base; // base address of fifo (do not change once allocated)
  elem_t* tail; // points to the next elem_t to go out (or equals head if fifo is empty)
  elem_t* head; // points to the next empty spot (or the last elem_t if fifo is full)
  int     size; // size of fifo (don not change once allocated)
  int     fill; // number of filled elem_t in fifo
} funnelbuf_t;

// binary sorting funnel structure
typedef struct binfunnel {
  funnelbuf_t* opbuf;
  bool exhausted;
  struct binfunnel* lcf; // left child funnel
  struct binfunnel* rcf; // right child funnel
} binfunnel_t;

inline bool is_empty(binfunnel_t* funnel) {
  return (funnel -> opbuf -> fill == 0);
}
inline bool is_full(binfunnel_t* funnel) {
  return (funnel -> opbuf -> fill == funnel -> opbuf -> size);
}
inline bool is_exhausted(binfunnel_t* funnel) {
  return (funnel -> exhausted);
}


inline elem_t* get_base(binfunnel_t* funnel) {
  return (funnel -> opbuf -> base);
}
inline elem_t* get_head(binfunnel_t* funnel) {
  return (funnel -> opbuf -> head);
}
inline elem_t* get_tail(binfunnel_t* funnel) {
  return (funnel -> opbuf -> tail);
}
inline int get_size(binfunnel_t* funnel) {
  return (funnel -> opbuf -> size);
}
inline int get_fill(binfunnel_t* funnel) {
  return (funnel -> opbuf -> fill);
}


inline void set_head(binfunnel_t* funnel, elem_t* head) {
  funnel -> opbuf -> head = head;
}
inline void set_tail(binfunnel_t* funnel, elem_t* tail) {
  funnel -> opbuf -> tail = tail;
}
inline void set_fill(binfunnel_t* funnel, int fill) {
  funnel -> opbuf -> fill = fill;
}
inline void set_exhausted(binfunnel_t* funnel) {
  funnel -> exhausted = true;
}

// for creating leaf streams
funnelbuf_t* bind_buffer(elem_t* base, elem_t* tail, elem_t* head, int size, int fill) {
  funnelbuf_t* buffer = new funnelbuf_t;
  buffer -> base = base;
  buffer -> tail = tail;
  buffer -> head = head;
  buffer -> size = size;
  buffer -> fill = fill;
  return buffer;
}

funnelbuf_t* create_buffer(int size) {
  funnelbuf_t* buffer = new funnelbuf_t;
  buffer -> base = new elem_t [size];
  buffer -> tail = buffer -> base;
  buffer -> head = buffer -> base;
  buffer -> size = size;
  buffer -> fill = 0;
  return buffer;
}

binfunnel_t* create_funnel(funnelbuf_t* opbuf, binfunnel_t* lcf, binfunnel_t* rcf) {
  binfunnel_t* funnel = new binfunnel_t;
  funnel -> opbuf = opbuf;
  funnel -> exhausted = false;
  funnel -> lcf = lcf;
  funnel -> rcf = rcf;
  return funnel;
}

// insert elem_t to head
inline void insert(binfunnel_t* funnel, elem_t elem) {
  elem_t* head = get_head(funnel);
  if( !is_full(funnel) ) {
    *head = elem;
    set_fill(funnel, get_fill(funnel) + 1);
    if( !is_full(funnel) ) {
      if( head < get_base(funnel) + get_size(funnel) - 1 ) {
        set_head(funnel, head + 1);
      } else {
        set_head(funnel, get_base(funnel));
      }
    }
  }
}

// look at tail elem_t
inline elem_t peek(binfunnel_t* funnel) {
  elem_t* tail = get_tail(funnel);
  if( !is_empty(funnel) ) {
    return *tail;
  } else {
    return 0;
  }
}

// remove tail elem_t and return its value
inline elem_t remove(binfunnel_t* funnel) {
  elem_t* tail = get_tail(funnel);
  elem_t elem = 0;
  if( !is_empty(funnel) ) {
    elem = *tail;
    set_fill(funnel, get_fill(funnel) - 1);
    if( !is_empty(funnel) ) {
      if( tail < get_base(funnel) + get_size(funnel) - 1 ) {
        set_tail(funnel, tail + 1);
      } else {
        set_tail(funnel, get_base(funnel));
      }
    }
  }
  return elem;
}

void print_opbuf(binfunnel_t* funnel) {
  elem_t* ptr;
  if( !is_empty(funnel) ) {
    ptr = get_tail(funnel);
    for(int i = 0; i < get_fill(funnel); i++){
      cout << *ptr << " ";
      if( ptr < get_base(funnel) + get_size(funnel) - 1 ) {
        ptr += 1;
      } else {
        ptr = get_base(funnel);
      }
    }
    cout << endl;
  } else {
    cout << "Output buffer empty!" << endl;
  }
}

// create sorting tree
binfunnel_t* create_funnel_tree(elem_t* array, int n, int bufsize) {
  binfunnel_t* parent;

  // create leaf funnels as input streams
  if(n == TH) {
    funnelbuf_t* stream;
    stream = bind_buffer(array,          // base
                         array,          // tail
                         array + TH - 1, // head
                         TH,             // size
                         TH);            // fill
  //stream = create_buffer(TH);
  //std::copy(array, array + TH, stream -> base);
    parent = create_funnel(stream, NULL, NULL);
  //set_fill(parent, TH);
  //set_head(parent, array + TH);
  } else {
    binfunnel_t* lcf = create_funnel_tree(array,           n / 2, bufsize / 2);
    binfunnel_t* rcf = create_funnel_tree(array + (n / 2), n / 2, bufsize / 2);
    parent = create_funnel(create_buffer(bufsize), lcf, rcf);
  }
  return parent;
}

void bind_funnel(binfunnel_t* funnel, elem_t* base, elem_t* tail, elem_t* head, int size, int fill) {
  funnel -> opbuf -> base = base;
  funnel -> opbuf -> tail = tail;
  funnel -> opbuf -> head = head;
  funnel -> opbuf -> size = size;
  funnel -> opbuf -> fill = fill;
}

void delete_funnel_tree(binfunnel_t* root) {
  if( (root -> lcf == NULL) && (root -> rcf == NULL) ) {
    delete (root -> opbuf);
  } else {
    delete_funnel_tree(root -> lcf);
    delete_funnel_tree(root -> rcf);
    delete [] (root -> opbuf -> base);
    delete (root -> opbuf);
    delete root;
  }
}

void lazy_fill(binfunnel_t* funnel) {
  if( (funnel -> lcf == NULL) && (funnel -> lcf == NULL) ) { // leaf funnel
    set_exhausted(funnel);
  } else {
    while( !is_full(funnel) ) {
      if( is_empty(funnel -> lcf) && (!is_exhausted(funnel -> lcf)) ) {
        lazy_fill(funnel -> lcf);
      }
      if( is_empty(funnel -> rcf) && (!is_exhausted(funnel -> rcf)) ) {
        lazy_fill(funnel -> rcf);
      }
      if( is_exhausted(funnel -> lcf) && is_exhausted(funnel -> rcf) ) {
        set_exhausted(funnel);
        break;
      } else if( is_exhausted(funnel -> lcf) ) {
        insert(funnel, remove(funnel -> rcf));
      } else if( is_exhausted(funnel -> rcf) ) {
        insert(funnel, remove(funnel -> lcf));
      } else {
        if(peek(funnel -> lcf) <= peek(funnel -> rcf)) {
          insert(funnel, remove(funnel -> lcf));
        } else {
          insert(funnel, remove(funnel -> rcf));
        }
      }
    }
    //print_opbuf(funnel);
  }
}

int main() {
  timespec start, end; // start and end of timer
  double runtime;      // run time in milliseconds

  elem_t* test_array;
  elem_t* ref_array;

  ofstream datfile;
  datfile.open("mergesort.dat");

  int nsize = NMIN;

  while(nsize < NMAX) { // 2 ^ 25
    test_array = new elem_t [nsize];
    ref_array = new elem_t [nsize];

    for(int i = 0; i < nsize; i++) {
      //test_array[i] = nsize - 1 - i; //==>
      test_array[i] = rand();
      ref_array[i] = test_array[i];
    }
    sort(ref_array, ref_array + nsize);

    // base cases defined by threshold TH
    elem_t* input;
    input = test_array;
    for(int i = 0; i < (nsize / TH); i++) {
      sort(input, input + TH);
      input += TH;
    }

#ifdef PRINT_ARRAY
    cout << "Input array is:  ";
    print_array(test_array, nsize);
    cout << endl;
#endif

    binfunnel_t* lcf = create_funnel_tree(test_array,           nsize / 2, nsize / 2);
    binfunnel_t* rcf = create_funnel_tree(test_array + (nsize / 2), nsize / 2, nsize / 2);
    binfunnel_t* funnelt = create_funnel(create_buffer(nsize), lcf, rcf);

    clock_gettime(CLOCK_REALTIME, &start);

#ifdef STD_SORT
    sort(test_array, test_array + nsize);
#else

    lazy_fill(funnelt);

#endif

    std::copy(get_tail(funnelt), get_tail(funnelt) + nsize, test_array);

    clock_gettime(CLOCK_REALTIME, &end);
    runtime = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6; // in milliseconds

    delete_funnel_tree(funnelt);

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

    cout << "Run-time of funnelsort: " << runtime << " milliseconds" << endl;

    delete [] test_array;

#ifdef VERIFY_RESULT
    if(error) {
      cout << "Error: Funnelsort has " << error << " errors!" << endl;
    } else {
      cout << "Success: Funnelsort has finished successfully." << endl;
    }
#endif

    datfile << nsize << " ";
    datfile << runtime << "\n";

    nsize = nsize * 2;
  }

  datfile.close();

  return 0;
}
