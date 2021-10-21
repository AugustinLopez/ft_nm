#include <stdio.h>

void *Letter_B = 0;
static void *Letter_b = 0;
char Letter_C;
static char Letter_c;
char Letter_D = 'D';
static char Letter_d = 'd';
extern char Letter_U;
const char Letter_R = 'R';
static const char Letter_r = 'r';

//compile with -g3 for n


static int Letter_t()
{
    return (Letter_d);
}



int Letter_T()
{
    return (Letter_t());
}


int main(){printf("Hi\n");return Letter_T();}
//W and w
