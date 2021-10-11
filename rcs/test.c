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
    return (Letter_t()+Letter_U2()+Letter_U3());
}