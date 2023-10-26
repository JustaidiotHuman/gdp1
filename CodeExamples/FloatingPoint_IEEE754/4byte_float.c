// Investigate the IEEE 754 storage format of single precision floating point numbers
// See
// https://en.wikipedia.org/wiki/Single-precision_floating-point_format

#include <stdio.h>
#include <stdlib.h>

// A data type for modelling the bitwise representation of IEEE 754
// float values with 4 byte

struct floatrep {
  char nibble1_rep[4+1];
  char nibble2_rep[4+1];
  char nibble3_rep[4+1];
  char nibble4_rep[4+1];
  char nibble5_rep[4+1];
  char nibble6_rep[4+1];
  char nibble7_rep[4+1];
  char nibble8_rep[4+1];

  char all_rep[32+1];
  char sign_rep[1+1];
  char exponent_rep[8+1];
  char mantissa_rep[23+1];

  unsigned char nibble1_val;
  unsigned char nibble2_val;
  unsigned char nibble3_val;
  unsigned char nibble4_val;
  unsigned char nibble5_val;
  unsigned char nibble6_val;
  unsigned char nibble7_val;
  unsigned char nibble8_val;

  unsigned char sign_val;
  unsigned char exponent_val;
  unsigned int  mantissa_val;
};

// ---------------------------------------------------------------------
// Convert to binary representation
// ---------------------------------------------------------------------

void make_bit_string(unsigned int ui, char* buf, int pos){
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

struct floatrep make_bitrep32(unsigned int ui){
  struct floatrep res;

  // create values by bit masking
  res.nibble1_val  = (ui & 0xf0000000)>>28;
  res.nibble2_val  = (ui & 0x0f000000)>>24;
  res.nibble3_val  = (ui & 0x00f00000)>>20;
  res.nibble4_val  = (ui & 0x000f0000)>>16;
  res.nibble5_val  = (ui & 0x0000f000)>>12;
  res.nibble6_val  = (ui & 0x00000f00)>>8;
  res.nibble7_val  = (ui & 0x000000f0)>>4;
  res.nibble8_val  = (ui & 0x0000000f);

  res.sign_val     = (ui & 0x80000000)>>31;
  res.exponent_val = (ui & 0x7F800000)>>23;
  res.mantissa_val = (ui & 0x007FFFFF);

  make_bit_string(res.nibble1_val, res.nibble1_rep, 4);
  make_bit_string(res.nibble2_val, res.nibble2_rep, 4);
  make_bit_string(res.nibble3_val, res.nibble3_rep, 4);
  make_bit_string(res.nibble4_val, res.nibble4_rep, 4);
  make_bit_string(res.nibble5_val, res.nibble5_rep, 4);
  make_bit_string(res.nibble6_val, res.nibble6_rep, 4);
  make_bit_string(res.nibble7_val, res.nibble7_rep, 4);
  make_bit_string(res.nibble8_val, res.nibble8_rep, 4);

  make_bit_string(ui, res.all_rep, 32);
  make_bit_string(res.sign_val, res.sign_rep, 1);
  make_bit_string(res.exponent_val, res.exponent_rep, 8);
  make_bit_string(res.mantissa_val, res.mantissa_rep, 23);

  return res;
}


// ---------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------

int main() {
  float f1 = 0.15625;
  //float f1 = -0.15625;
  //
  // Note: the following value cannot be stored precisely as a binary float
  // It is converted to a binary fraction, which results in -815.47113
  // upon re-convertion to a decimal number
  //float f1 = -815.4711;

  // Some pointer voodoo in order to prevent type conversion of values
  unsigned int* uip;
  uip = (unsigned int*) &f1;
  // Now, the unsigned integer variable uip stores the same 32 bit pattern
  // as the 4 byte float variable f1

  printf("\n");
  printf("Single precision float variable f1: %f\n", f1);
  printf("\n");
  printf("Hexadecimal representation in memory (msb first): %8x\n", *uip);

  // Create a string representation of the binary value
  struct floatrep f_rep = make_bitrep32(*uip);

  printf("\n");
  printf("Nibble representation:\n");
  printf("     all: %32s\n" , f_rep.all_rep);
  printf("nibble 1: %4s\n"  , f_rep.nibble1_rep);
  printf("nibble 2: %8s\n"  , f_rep.nibble2_rep);
  printf("nibble 3: %12s\n" , f_rep.nibble3_rep);
  printf("nibble 4: %16s\n" , f_rep.nibble4_rep);
  printf("nibble 5: %20s\n" , f_rep.nibble5_rep);
  printf("nibble 6: %24s\n" , f_rep.nibble6_rep);
  printf("nibble 7: %28s\n" , f_rep.nibble6_rep);
  printf("nibble 8: %32s\n" , f_rep.nibble8_rep);

  // Create a string representation of the IEEE float value
  printf("\n");
  printf("IEEE 754 float components:\n");
  printf("     all: %32s\n", f_rep.all_rep);
  printf("    sign: %1s\n", f_rep.sign_rep);
  printf("exponent: %9s\n", f_rep.exponent_rep);
  printf("mantissa: %32s\n", f_rep.mantissa_rep);

  // Recompute the value from the components
  printf("\n");
  printf("Recomputation of IEEE 754 float:\n");

  char sign_factor = (f_rep.sign_val?-1:1);
  printf("           sign_bit: %d\n", f_rep.sign_val);
  printf("        sign_factor: %d\n", sign_factor);

  char ex = (f_rep.exponent_val-127);
  printf("exponent   (biased): %d\n", f_rep.exponent_val);
  printf("exponent (unbiased): %d\n", ex);

  float exp_factor = (ex < 0 ? 1.0/(1<<(-1 * ex)) : 1<<ex);
  printf("    exponent factor: %f\n", exp_factor);

  float mantissa = 1 + (1.0 * f_rep.mantissa_val / (1<<23) );
  printf("           mantissa: %f\n", mantissa);
  
  printf("IEEE 754 float value = sign_factor * exp_factor * mantissa : %f\n",
         1.0 * sign_factor * exp_factor * mantissa);

  printf("\n");
  return 0;
}

