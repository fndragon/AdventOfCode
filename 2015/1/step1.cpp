#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    
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
                break;
            default:
                std::cout << "error";
                break;
        }
        std::cout << sum << "\n";
        input >> inputChar;
    }

    input.close();
    std::cout << "final sum = " << sum << "\n";
    return 0;
}