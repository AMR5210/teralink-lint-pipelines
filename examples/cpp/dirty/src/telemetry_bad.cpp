/** @file
 * @brief Deliberately bad code used to show the pipeline failing.
 */
#include <iostream>
#include <cstdlib>
using namespace std;

int fib(int n)
{
  if (n<2) return n;
  return fib(n-1)+fib(n-2);
}

int* makeBuffer(int size){
  int* buf = (int*) malloc(size*sizeof(int));
  return buf;
}

int sumTo(unsigned long limit)
{
    int total;
    for(unsigned long i=0;i<limit;i++) total += i;
    return total;
}

void leak() { int* p = new int(5); }

int main() {
  cout << fib(10) << endl;
  makeBuffer(4);
  return 0;
}
