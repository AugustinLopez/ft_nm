void *Letter_B = 0;
static void *Letter_b = 0;
char Letter_C;
static char Letter_c;
char Letter_D = 'D';
static char Letter_d = 'd';
extern char Letter_U;
const char Letter_R = 'R';
static const char Letter_r = 'r';

struct example {
    struct addr_t {
       int port;
    } addr;
    union {
       char a8[4];
       short a16[2];
    } in_u;
};
struct example Letter_D2 = { // start of initializer list for struct example
                     { // start of initializer list for ex.addr
                        80 // initialized struct's only member
                     }, // end of initializer list for ex.addr
                     { // start of initializer-list for ex.in_u
                        {127,0,0,1} // initializes first element of the union
                     } };

#include <string.h>

//compile with -g3 for n

static int Letter_t()
{
    return (Letter_U);
}

inline int Letter_U2()
{
    return (Letter_d);
}

int Letter_T()
{
    const char aaa;
    static const char rrr;
    static char bbb;
    static char ddd = 'a';
    memset(&bbb, &ddd, sizeof(ddd));
    return (Letter_t()+Letter_U2()+Letter_U3());
}