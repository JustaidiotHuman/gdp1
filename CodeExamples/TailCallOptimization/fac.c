// Compile with
//
//  mkdir -p bin
//  gcc -g -Wall fac.c -o bin/fac
//
// Run the binary as a test
//
//  bin/fac 5
//  
//  > output
//  fac(5)=120
// 
// Then, run the debugger
//
//  gdb -q bin/fac -ex "disassemble fac"
//
//  (gdb) break fac
//  (gdb) run 5
//  (gdb) cont
//  (gdb) <enter>
//  (gdb) ...
//  (gdb) quit

#include <stdio.h>
#include <stdlib.h>

extern int fac(int n);

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

int fac(int n){
  int res;

  if (n == 0){
    res = 1;
  } else {
    res = n * fac(n-1);
  }
  return res;
}

