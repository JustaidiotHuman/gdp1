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
//    0x0000000000000720 <+0>:	test   %edi,%edi
//    0x0000000000000722 <+2>:	mov    %esi,%eax
//    0x0000000000000724 <+4>:	je     0x738 <itfac+24>
//    0x0000000000000726 <+6>:	nopw   %cs:0x0(%rax,%rax,1)
//    0x0000000000000730 <+16>:	imul   %edi,%eax
//    0x0000000000000733 <+19>:	sub    $0x1,%edi
//    0x0000000000000736 <+22>:	jne    0x730 <itfac+16>
//    0x0000000000000738 <+24>:	repz retq
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

/*
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

*/

int itfac(int n, int acc){
  if (n != 0) {
      acc = itfac(n-1, n*acc); // tail-recursive call
  }
  return acc;
}

