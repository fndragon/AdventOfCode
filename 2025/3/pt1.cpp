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

void processBank(std::string line)
{
    std::vector< size_t > digits;
    digits.resize(12);
    // pre-fill from end
    for(int i=0;i<12;i++)
    {
        // want 3,4, 5,6,7,8,9, 10,11,12,13,14
        digits[i] = i + ( line.length() - 12 );
    }
    std::cout << line << " : " << calcValue(digits, line) << "\n";
    size_t checkIndex = line.length() - 12 - 1;  //2
    while( checkIndex >= 0)
    {
        int digitToCheck = line[checkIndex] - '0';
        std::vector< size_t > originalDigits = digits;
        // Iterate through all digit positions (bubble)
        for( int i=0;i<11;i++)
        {
            std::vector< size_t > testDigits = originalDigits;
            if( digitToCheck > line[digits[i]] - '0')
            {
                //Construct test digits by shift right all digits at i to the end
                for(int j=i;j<11;j++)
                {
                    testDigits[j+1] = originalDigits[j];
                }
                testDigits[i] = checkIndex;

                long testValue = calcValue(testDigits, line );
                if( testValue > calcValue(digits, line))
                {
                    digits = testDigits;
                }
            }
        }
        --checkIndex;
    }
    sum += calcValue(digits, line);
/*
    size_t lowestLocation = line.length() - 1;
    size_t highestLocation = line.length() - 2;
    int lsd = line[lowestLocation] - '0';
    int hsd = line[highestLocation] - '0';
    int bestSum = hsd * 10 + lsd;
    int checkIndex = line.length() - 3;  // start at 3rd position
    size_t largestNextNumLocation = ( hsd > lsd ) ? highestLocation : lowestLocation;
    while( checkIndex >= 0 )
    {
        int checkValue = line[checkIndex] - '0';
        int newSum = checkValue * 10 + (line[largestNextNumLocation] - '0');
        if( newSum > bestSum )
        {
            // we have a larger sum.  Move and update
            highestLocation = checkIndex;
            lowestLocation = largestNextNumLocation;
            lsd = line[ lowestLocation ] - '0';
            hsd = line[ highestLocation ] - '0';
            largestNextNumLocation = ( hsd > lsd ) ? highestLocation : lowestLocation;
            bestSum = newSum;
        }
        --checkIndex;
    }
    sum += bestSum;
    */
}

int main(void)
{
    std::ifstream input;
    input.open("example.txt");
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