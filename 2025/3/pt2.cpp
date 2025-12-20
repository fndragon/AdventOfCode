#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

long calcValue(std::vector< size_t > &digits, std::string &line )
{
    long localSum = 0;
    for( int i = 0; i < digits.size();i++)
    {
        // want 10^ location * value
        // so at the very end, 10^0 * 1 = 1
        // 
        int tenpow = digits.size() - 1 - i;
        localSum += pow(10, tenpow) * (line[digits[i]] - '0');
    }
    return localSum;
}

int value(char c)
{
    return (int) (c - '0');
}

void processBank(std::string line)
{
    std::vector< size_t > digits;
    digits.resize(12);
    int currentLeftMostIndex = 0;
    for(int i=0;i<12;i++)
    {
        // for each most significant digit, find the largest value possible, from left to right.
        int maximumValidIndex = line.size() - (12 - i);
        int currentValue = 0;
        for(int j=currentLeftMostIndex;j<=maximumValidIndex;j++)
        {
            int val = value(line[j]);
            if( val > currentValue )
            {
                currentLeftMostIndex = j;
                currentValue = val;
            }
        }
        // now store this digit
        digits[i] = currentLeftMostIndex;
        currentLeftMostIndex++;
    }
    std::cout << "line \"" << line << "\" has the largest joltage of " << calcValue(digits, line) << "\n";
    sum += calcValue(digits, line);
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        //std::cout << currentLine << "\n";
        processBank(currentLine);
    }

    input.close();
    std::cout << " sum : " << sum << "\n";
    return 0;
}