#include <stdexcept>

void func( int x )
{
    char* pleak = new char[1024]; // might be lost => memory leak
    std::string s( "hello world" ); // will be properly destructed

    if ( x ) throw std::runtime_error( "boom" );

    delete [] pleak; // will only get here if x == 0. if x!=0, throw exception
}

int main()
{
    try
    {
        func( 10 );
    }
    catch ( const std::exception& e )
    {
        return 1;
    }

    return 0;
}