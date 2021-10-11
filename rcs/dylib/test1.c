#include <stdio.h>

int a(int b)
{
  return b+1;
}

int c(int d)
{
  return a(d)+1;
}

int main()
{
  int b = a(3);
  printf("Calling a(3) gave %d \n", b);
  int d = c(3);
  printf("Calling c(3) gave %d \n", d);
}