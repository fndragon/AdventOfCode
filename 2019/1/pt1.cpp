#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    long long pt1sum;
    long long pt2sum;
    while( std::getline(input, currentLine) )
    {
        std::cout << currentLine << "\n";
        //Fuel required to launch a given module is based on its mass. Specifically, to find the fuel required for a module, 
        /// take its mass, divide by three, round down, and subtract 2.
        long long fuel = (floor( atoll(currentLine.c_str()) / 3.0 ) - 2);
        pt1sum += fuel;
        pt2sum += fuel;
        while( fuel > 0 )
        {
            fuel = std::max< long long >(0,(floor(fuel / 3.0)-2.0));
            pt2sum += fuel;
        }

    }
    std::cout << "pt1: Sum is " << pt1sum << "\n";
    std::cout << "pt2: Sum is " << pt2sum << "\n";
    input.close();
    return 0;
}