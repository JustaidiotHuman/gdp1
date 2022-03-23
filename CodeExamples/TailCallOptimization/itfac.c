// The gcc compiler automatically replaces a
// tail-recursive call by a tight loop.
//
// Compile with
//
//  mkdir -p bin
//  gcc -O2 -g -Wall itfac.c -o bin/itfac
//
// or simply by
//
//  make
//
// Run the binary as a test
//
//  bin/itfac 5
//  
//  > output
//  fac(5)=120
// 
// Then, run the debugger
//
//  gdb -q bin/itfac -ex "disassemble itfac"
//
// Reading symbols from bin/itfac...fertig.
// Dump of assembler code for function itfac:
//    0x00000620 <+0>:	mov    0x4(%esp),%edx
//    0x00000624 <+4>:	mov    0x8(%esp),%eax
//    0x00000628 <+8>:	test   %edx,%edx
//    0x0000062a <+10>:	je     0x638 <itfac+24>
//    0x0000062c <+12>:	lea    0x0(%esi,%eiz,1),%esi
//    0x00000630 <+16>:	imul   %edx,%eax
//    0x00000633 <+19>:	sub    $0x1,%edx
//    0x00000636 <+22>:	jne    0x630 <itfac+16>
//    0x00000638 <+24>:	repz ret
// End of assembler dump.
// (gdb)
//
// There is no longer any recursive call in the assembler code.
// q.e.d.
//

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

int itfac(int n, int acc){
  int n_next;
  int acc_next;
  int res;

  if (n == 0) {
      res = acc;
    } else {
      n_next = n-1;
      acc_next = n*acc;

      res = itfac(n_next, acc_next); // tail-recursive call
    }

  return res;
}

/*
int itfac(int n, int acc){
  if (n != 0) {
      acc = itfac(n-1, n*acc); // tail-recursive call
  }
  return acc;
}
*/

