#include <stdio.h>
int main() {
  int age;
  char* name;

  age = 23;
  name = "Daniel Rami";

  printf("My Name is %c. Ich bin  %d Jahre alt\n", *(name + 1),age);

  return 0;
}
