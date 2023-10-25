// Investigate the IEEE 754 storage format of single precision floating point numbers
// See
// https://en.wikipedia.org/wiki/Single-precision_floating-point_format

#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------
// Convert to binary interleaved by . at nibble boundaries
// ---------------------------------------------------------------------
char* make_bitrep32(unsigned int ui){
  const int bitsize = 32;
  const int arraysize = bitsize+7+1; // we have 8 nibbles -> 7 dots

  char* bitrep=NULL;
  if ((bitrep = malloc(arraysize * sizeof(char))) == NULL){
    fprintf(stderr,"Out of memory\n");
    exit(1);
  }

  int pos = arraysize-1;
  int cnt = 0;

  bitrep[pos] = '\0';
  pos--;

  do {
    if (ui % 2 == 1){
      bitrep[pos] = '1';
    } else {
      bitrep[pos] = '0';
    }
    ui = ui >> 1;
    cnt++;
    pos--;

    if (0 < cnt && cnt < bitsize && cnt%4 == 0){
      bitrep[pos] = '.';
      pos--;
    }
  } while (cnt < bitsize);
  return bitrep;
}

// ---------------------------------------------------------------------
// Convert to binary float rep interleaved by | at component boundary
// ---------------------------------------------------------------------

char* make_floatrep32(unsigned int ui){
  const int bitsize = 32;
  const int arraysize = bitsize+2+1; // we have 3 components -> 2 '|'

  char* bitrep=NULL;
  if ((bitrep = malloc(arraysize * sizeof(char))) == NULL){
    fprintf(stderr,"Out of memory\n");
    exit(1);
  }

  int pos = arraysize-1;
  int cnt = 0;

  bitrep[pos] = '\0';
  pos--;

  do {
    if (ui % 2 == 1){
      bitrep[pos] = '1';
    } else {
      bitrep[pos] = '0';
    }
    ui = ui >> 1;
    cnt++;
    pos--;

    if (cnt == 23 || cnt == 31){
      bitrep[pos] = '|';
      pos--;
    }
  } while (cnt < bitsize);
  return bitrep;
}

// ---------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------

int main() {
  float f1 = 0.15625;

  // Some pointer voodoo in order to prevent type conversion of values
  void *vp;
  unsigned int* uip;
  vp  = &f1;
  uip = vp;
  // Now, the unsigned integer variable uip stores the same 32 bit pattern
  // as the 4 byte float variable f1

  printf("Single precision float variable f1: %f\n", f1);

  printf("Hexadecimal representation in memory (msb first): %8x\n", *uip);

  // Create a string representation of the binary value with
  // 7 dots separating the nibbles 
  char* bitrep;

  bitrep = make_bitrep32(*uip);
  printf("Bitwise representation: %s\n", bitrep);
  free(bitrep);

  // Create a string representation of the IEEE float values with
  // '|' separating the components:  sign|exponent|mantissa
  bitrep = make_floatrep32(*uip);
  printf("IEEE 754 float components: %s\n", bitrep);
  free(bitrep);

  return 0;
}

