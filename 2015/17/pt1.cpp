#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    std::vector< int > size;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        size.push_back( atoi(currentLine.c_str()));
    }

    // Find every permutation of using or not-using a cup using a unsigned int for bitmask nonsense:
    unsigned int i, minimumBitsUsed = 512; // bogus max
    int count = 0;
    for(i=0;i<(1<<size.size());i++) // i's bits from 0 .. size are whether or not a cup is ON.
    {
        int sum=0; // reset the sum for this loop
        for (int j=0; j<size.size(); j++) // bit to test
        {
            if ((1 << j) & i)
            {
                sum += size[j]; // if the bit in i is set, add the appropriate size to the sum.
            }
        }
        if( sum == 150)
        {
            count++;
        }
    }
    std::cout << "150 is " << count << "\n";
    input.close();
    return 0;
}