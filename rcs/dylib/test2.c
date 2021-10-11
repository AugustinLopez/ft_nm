#include <dlfcn.h>
#include <stdio.h>


int (*a_ptr)(int b);
int (*c_ptr)(int d);

int main()
{
  void* lib=dlopen("./test1",RTLD_LAZY);
  a_ptr=dlsym(lib,"a");
  c_ptr=dlsym(lib,"c");
  int d = c_ptr(6);
  int b = a_ptr(5);
  printf("b is %d d is %d\n",b,d);
  return 0;
}