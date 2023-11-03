#include<stdio.h>
#include<assert.h>

void teile_mit_rest(int a, int b, int* p_q, int* p_r){
  assert(a>=0);
  assert(b>0);

  int cnt = 0;
  while(a >= b){
    a = a - b;
    cnt++;
  }

  *p_q = cnt;
  *p_r = a;
}

int main(void){
  int quotient = -1;
  int rest = -1;

  int x = 24;
  int y = 9;
  teile_mit_rest(x, y, &quotient, &rest);

  printf("Ganzzahlig teilen von %d durch %d geht %d mal Rest %d\n", x, y, quotient, rest);
  return 0;
}
