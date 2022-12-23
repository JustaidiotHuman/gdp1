#include <stdio.h>
#include <stdlib.h>

void first(){
  int a;
  a = 0x11223344;
  printf("a=%x\n",a);
}

void second(){
  int b;
  // Was passiert, wenn man die Zuweisung erst nach dem
  // zweiten printf() ausführt?
  // Was wird hier ausgegeben?
  printf("%ld\n", sizeof(b));
  printf("b=%x\n",b);
  b = 0x55667788;
  printf("b=%x\n",b);
}

int main () {
  first();
  second();
  return EXIT_SUCCESS;
}
