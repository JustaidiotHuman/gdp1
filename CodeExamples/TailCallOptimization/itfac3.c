// Compile with
//
//  mkdir -p bin
//  gcc -g -Wall itfac3.c -o bin/itfac3
//
// Run the binary as a test
//
//  bin/itfac3 5
//  
//  > output
//  fac(5)=120
// 
// Then, run the debugger
//
//  gdb -q bin/itfac3 -ex "disassemble itfac"
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
//  gcc -O2 -g -Wall itfac3.c -o bin/itfac3
//
//  and run the debugger again like above...
//
//  gdb -q bin/itfac3 -ex "disassemble itfac"
//
// Reading symbols from bin/itfac...fertig.
// Dump of assembler code for function itfac:
//    0x00000620 <+0>:  mov    0x4(%esp),%edx
//    0x00000624 <+4>:  mov    0x8(%esp),%eax
//    0x00000628 <+8>:  test   %edx,%edx
//    0x0000062a <+10>: je     0x638 <itfac+24>
//    0x0000062c <+12>: lea    0x0(%esi,%eiz,1),%esi
//    0x00000630 <+16>: imul   %edx,%eax
//    0x00000633 <+19>: sub    $0x1,%edx
//    0x00000636 <+22>: jne    0x630 <itfac+16>
//    0x00000638 <+24>: repz ret
// End of assembler dump.
// (gdb)
//
// This is the same code that we obtain by running
// either
//
// gcc -O2 -g -Wall itfac3.c -o bin/itfac1
// or
// gcc -O2 -g -Wall itfac3.c -o bin/itfac2
//
// The gcc compiler is able to optimize the tail-recursive code
// such that we get the same tight loop as for the optimized
// while code of itfac3.c
//
// q.e.d.

#include <stdio.h>
#include <stdlib.h>

extern int itfac(int n, int acc);
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
  return itfac(n,1); 
}

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
/*
int itfac(int n, int acc){
  if (n != 0) {
      acc = itfac(n-1, n*acc); // tail-recursive call
  }
  return acc;
}
*/

int itfac(int n, int acc){
  while (n != 0){
    acc = n*acc;
    n = n-1;
  }
  return acc;
}
