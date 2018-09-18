#include <iostream>

using namespace std;

struct a{
  int x;
  char y[3];
};

struct b{
  a* array = (a*)malloc(4*sizeof(a));
};
int main(){
  b* t = (b*)malloc(sizeof(b));
  cout << sizeof(b) << endl;
  t->array[0].x = 1;
  cout << t->array[0].x << endl;
  cout << t->array[1].x << endl;
  return 0;
}