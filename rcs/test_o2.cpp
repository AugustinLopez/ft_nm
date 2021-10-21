//https://stackoverflow.com/questions/19376458/what-does-eh-mean-in-nm-output
bool extenrnal_variable;

int f(...)
{
    if (extenrnal_variable)
        throw 0;

    return 42;
}

int g()
{
    return f(1, 2, 3);
}
