// Investigate the IEEE 754 storage format of double precision floating point numbers
// See
// https://en.wikipedia.org/wiki/Single-precision_floating-point_format

#include <stdio.h>
#include <stdlib.h>
#include<assert.h>
#include <math.h>       /* isnormal */

// A data type for modelling the bitwise representation of IEEE 754
// double values with 8 byte

struct doublerep {
  char byte1_rep[8+1];
  char byte2_rep[8+1];
  char byte3_rep[8+1];
  char byte4_rep[8+1];
  char byte5_rep[8+1];
  char byte6_rep[8+1];
  char byte7_rep[8+1];
  char byte8_rep[8+1];

  char all_rep[64+1];
  char sign_rep[1+1];
  char exponent_rep[11+1];
  char fraction_rep[52+1];

  unsigned char byte1_val;
  unsigned char byte2_val;
  unsigned char byte3_val;
  unsigned char byte4_val;
  unsigned char byte5_val;
  unsigned char byte6_val;
  unsigned char byte7_val;
  unsigned char byte8_val;

  unsigned char sign_val;
  unsigned short exponent_val;
  unsigned long long fraction_val;
};

// ---------------------------------------------------------------------
// Convert to binary representation
// ---------------------------------------------------------------------

void make_bit_string(unsigned long long ui, char* buf, int pos){
  buf[pos] = '\0'; // termination
  pos--;
  do {
    if (ui % 2 == 1){
      buf[pos] = '1';
    } else {
      buf[pos] = '0';
    }
    ui = ui >> 1;
    pos--;
  } while (pos >= 0);
}

struct doublerep make_bitrep64(unsigned long long ui){
  struct doublerep res;

  // create values by bit masking
  res.byte1_val  = (ui & 0xff00000000000000)>>56;
  res.byte2_val  = (ui & 0x00ff000000000000)>>48;
  res.byte3_val  = (ui & 0x0000ff0000000000)>>40;
  res.byte4_val  = (ui & 0x000000ff00000000)>>32;
  res.byte5_val  = (ui & 0x00000000ff000000)>>24;
  res.byte6_val  = (ui & 0x0000000000ff0000)>>16;
  res.byte7_val  = (ui & 0x000000000000ff00)>>8;
  res.byte8_val  = (ui & 0x00000000000000ff);

  res.sign_val     = (ui & 0x8000000000000000)>>63; // check
  res.exponent_val = (ui & 0x7FF0000000000000)>>52; // check
  res.fraction_val = (ui & 0x000FFFFFFFFFFFFF);     // check

  make_bit_string(res.byte1_val, res.byte1_rep, 8);
  make_bit_string(res.byte2_val, res.byte2_rep, 8);
  make_bit_string(res.byte3_val, res.byte3_rep, 8);
  make_bit_string(res.byte4_val, res.byte4_rep, 8);
  make_bit_string(res.byte5_val, res.byte5_rep, 8);
  make_bit_string(res.byte6_val, res.byte6_rep, 8);
  make_bit_string(res.byte7_val, res.byte7_rep, 8);
  make_bit_string(res.byte8_val, res.byte8_rep, 8);

  make_bit_string(ui, res.all_rep, 64);
  make_bit_string(res.sign_val, res.sign_rep, 1);
  make_bit_string(res.exponent_val, res.exponent_rep, 11);
  make_bit_string(res.fraction_val, res.fraction_rep, 52);

  return res;
}


// ---------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------

int main() {
  // Check our assumptions about the storage format
  assert(sizeof(double) == 8);
  assert(sizeof(unsigned long long) == 8);
  assert(sizeof(unsigned long long*) == 8);

  // Some example values
  //double float_val = 0.15625;
  //double float_val = -0.15625;
  //double float_val = -815.4711;
  double float_val = -345686.406250;
  //double float_val = -345686.40;
  //double float_val = -34568.64;

  // Some pointer voodoo in order to prevent type conversion of values
  // Note:
  // The plain assignment
  //  unsigned long long ui = float_val;
  // enforces a type conversion
  unsigned long long* uip;
  uip = (unsigned long long*) &float_val;
  // Now, the unsigned integer variable uip stores the same 64 bit pattern
  // as the 8 byte double variable float_val

  // For explicit manipulation of the stored float value (for testing)
  // ---> Activate the declarations for manipulated values
  //unsigned long long man_sign;
  //unsigned long long man_exp ;
  //unsigned long long man_frac;

  // Patterns for manipulation:
  // man_sign = 0x8000000000000000;
  // man_exp  = 0x7FF0000000000000;
  // man_frac = 0x000FFFFFFFFFFFFF;

  // Case: signed zero
  //man_sign = 0x8000000000000000;
  //man_sign = 0x0000000000000000;
  //man_exp  = 0x0000000000000000;
  //man_frac = 0x0000000000000000;

  // Case: subnormal
  //man_sign = 0x8000000000000000;
  //man_sign = 0x0000000000000000;
  //man_exp  = 0x0000000000000000;
  //man_frac = 0x0000000000000001;
  //man_frac = 0x000FFFFFFFFFFFFF;
  
  // Case: infinite
  //man_sign = 0x8000000000000000;
  //man_sign = 0x0000000000000000;
  //man_exp  = 0x7FF0000000000000;
  //man_frac = 0x0000000000000000;

  // Case: not a number
  //man_sign = 0x8000000000000000;
  //man_exp  = 0x7FF0000000000000;
  //man_frac = 0x000FFFFFFFFFFFFF;

  // ---> Activate the assignment for manipulated values
  //*uip = man_sign | man_exp | man_frac;

  printf("\n");
  if (isnormal(float_val)){
    printf("Double precision float variable float_val (normal): %.20E\n", float_val);
  } else {
    printf("Double precision float variable float_val (non-normal): %.1000E\n", float_val);
  }
  printf("\n");
  printf("Hexadecimal representation in memory (msb first): %16llx\n", *uip);

  // Create a string representation of the binary value
  struct doublerep float_rep = make_bitrep64(*uip);

  printf("\n");
  printf("byte representation:\n");
  printf("   all: %64s\n" , float_rep.all_rep);
  printf("byte 1: %8s\n"  , float_rep.byte1_rep);
  printf("byte 2: %16s\n" , float_rep.byte2_rep);
  printf("byte 3: %24s\n" , float_rep.byte3_rep);
  printf("byte 4: %32s\n" , float_rep.byte4_rep);
  printf("byte 5: %40s\n" , float_rep.byte5_rep);
  printf("byte 6: %48s\n" , float_rep.byte6_rep);
  printf("byte 7: %56s\n" , float_rep.byte6_rep);
  printf("byte 8: %64s\n" , float_rep.byte8_rep);

  // Create a string representation of the IEEE 754 double value
  printf("\n");
  printf("IEEE 754 double components:\n");
  printf("     all: %64s\n", float_rep.all_rep);
  printf("    sign: %1s\n" , float_rep.sign_rep);
  printf("exponent: %12s\n", float_rep.exponent_rep);
  printf("fraction: %64s\n", float_rep.fraction_rep);

  // Recompute the value from the components
  printf("\n");
  printf("Re-computing the IEEE 754 double from its components:\n");

  char sign_factor = (float_rep.sign_val?-1:1);
  printf("           sign_bit: %d\n", float_rep.sign_val);
  printf("        sign_factor: %d\n", sign_factor);

  // Interpretation of the exponent bit pattern
  if (float_rep.exponent_val == 0){
    if (float_rep.fraction_val == 0){
      // Signed zero
      printf("Recomputed value (signed zero): %c0.0\n", sign_factor<0?'-':'+');
    } else {
      // Subnormal number
      short int ex = (1-1023);
      printf("  exponent (biased): %d\n", float_rep.exponent_val);
      printf("  exponent  (-1022): %d\n", ex);

      double significand = 1.0 * float_rep.fraction_val;
      short int ex_cnt = -1 * ex + 52;
      printf("ex_cnt: %d\n", ex_cnt);
      while (ex_cnt > 0) { significand /= 2.0; ex_cnt--; }

      printf(" unshifted fraction: %f\n", 1.0*float_rep.fraction_val);
      printf("Recomputed value (subnormal): %.1000E\n", sign_factor * significand);
    }
  } else if (float_rep.exponent_val == 0x7ff) {
    if (float_rep.fraction_val == 0){
      // Infinite
      printf("Recomputed value (infinite): %cINF\n", sign_factor<0?'-':'+');
    } else {
      // NaN (not a number)
      printf("Recomputed value (not a number): %cNAN\n", sign_factor<0?'-':'+');
    }
  } else {
    short int ex = (float_rep.exponent_val-1023);
    printf("  exponent (biased): %d\n", float_rep.exponent_val);
    printf("  exponent  (-1023): %d\n", ex);

    double exp_factor = (ex < 0 ? 1.0/(1ULL<<(-1 * ex)) : 1ULL<<ex);
    printf("    exponent factor: %f\n", exp_factor);

    double significand = 1 + (1.0 * float_rep.fraction_val / (1ULL<<52) );
    printf(" unshifted fraction: %f\n", 1.0*float_rep.fraction_val);
    printf("        significand: %f\n", significand);

    printf("Recomputed value:\n");
    printf("sign_factor * exp_factor * significand: %.20E\n",
        1.0 * sign_factor * exp_factor * significand);
  }
  printf("\n");
  return 0;
}

