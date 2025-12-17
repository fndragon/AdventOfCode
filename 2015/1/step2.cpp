#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    int currentChar = 1;
    int position = 0;
    char inputChar;
    input >> inputChar;
    while( input.good())
    {
        std::cout << "[" << sum << "]: read " << inputChar << " ==> ";
        switch( inputChar )
        {
            case '(':
                sum++;
                break;
            case ')':
                sum--;
                if( sum < 0 )
                {
                    if( position == 0)
                    {
                        position = currentChar;
                    }
                }
                break;
            default:
                std::cout << "error";
                break;
        }
        std::cout << sum << "\n";
        input >> inputChar;
        currentChar++;
    }

    input.close();
    std::cout << "Position set at " << position;
    std::cout << "final sum = " << sum << "\n";
    return 0;
}