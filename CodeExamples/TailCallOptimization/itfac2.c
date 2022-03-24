// Compile with
//
//  mkdir -p bin
//  gcc -g -Wall itfac2.c -o bin/itfac2
//
// Run the binary as a test
//
//  bin/itfac2 5
//  
//  > output
//  fac(5)=120
// 
// Then, run the debugger
//
//  gdb -q bin/itfac2 -ex "disassemble itfac"
//
//  (gdb) break itfac
//  (gdb) run 5
//  (gdb) cont
//  (gdb) <enter>
//  (gdb) ...
//  (gdb) quit
//
//  Now compile with option -O2
//
//  gcc -O2 -g -Wall itfac2.c -o bin/itfac2
//
//  and run the debugger again like above...
//
//  gdb -q bin/itfac2 -ex "disassemble itfac"

#include <stdio.h>
#include <stdlib.h>

extern int itfac(int n, int acc);
extern int fac(int n);

// -------------------------------------------

int main(int argc, char* argv[]){
  int n;
  int res;

  if (argc != 2){
    printf("Error: usage %s nat\n", argv[0]);
    return 1;
  }

  n = atoi(argv[1]);
  res = fac(n);

  printf("fac(%i)=%i\n", n, res);
  return EXIT_SUCCESS;
}

// -------------------------------------------

int fac(int n){
  return itfac(n,1); 
}

// -------------------------------------------

/*
int itfac(int n, int acc){
  int res;

  if (n == 0) {
      res = acc;
    } else {
      res = itfac(n-1, n*acc); // tail-recursive call
    }

  return res;
}
*/

int itfac(int n, int acc){
  if (n != 0) {
      acc = itfac(n-1, n*acc); // tail-recursive call
  }
  return acc;
}

