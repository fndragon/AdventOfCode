#include <stdio.h>
#include <iostream>
#include <fstream>

int count = 0;

void turnDialRight( int const rotation, int &value )
{
    // rotate right
    if( value + rotation >= 100) count++;
    value = (value + rotation) % 100;
    return;
}

void turnDialLeft( int const rotation, int &value )
{
    // rotate left
    if( value != 0)
    {
        if( value - rotation <= 0 )
        {
            count++;
        }
    }

    value = (value - rotation) % 100;
    if( value < 0 ) value += 100;
    return;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string line;
    int value = 50; // (0..99)
    while( std::getline(input, line) )
    {
        // process
        if( line.empty() ) break;
        std::string sub = line.substr( 1, line.length());
        int rotation = std::atoi ( sub.c_str() );
        while( rotation >= 100 )
        {
            count++;
            rotation -= 100;
        }

        if( line[0] == 'R' )
        {
            turnDialRight(rotation, value);
        }
        if( line[0] == 'L' )
        {
            turnDialLeft(rotation, value);
        }
    }

    input.close();

    std::cout << "final count = " << count << "\n";
    return 0;
}