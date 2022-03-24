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

