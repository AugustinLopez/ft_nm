struct obj { void (*fnptr)(void); int z; };
static void fn0(void) { }
const struct obj constInitedReadonlyObject = { 0, 42 };
const struct obj readonlyObject = { &fn0, 42 };

int main()
{
    int volatile*z = (int volatile*)&readonlyObject.z;
    *z = 1000;
}