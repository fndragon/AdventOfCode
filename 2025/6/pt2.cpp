#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

long long sum=0;
std::vector< std::string > array;

void processLine(std::string &line)
{
    std::string localLine = line;
    if( line[0] != '*' ) // it's solve time
    {
        array.push_back(line);
    }
    else
    {
        for(int i=0;i< line.size();i++)
        {
            // array has all numbers
            // line has all operators
            bool isPlus = line[i] == '+';
            // find the terminating column number:
            int j=0;
            for(j=i+1;j<line.size();j++)
            {
                if( line[j] != ' ')
                {
                    j--;
                    break;
                }
            }

            // j is now one past where we want to read from.
            std::vector< std::string > values;
            values.resize(j-i);
            for(int k=i;k<j;k++)
            {
                // read column of numbers and build a string
                for(int l=0;l< array.size();l++)
                {
                    if( array[l][k] != ' ')
                    {
                        values[k-i].push_back( array[l][k] );
                    }
                }
            }
            // Now do the thing to values:
            if( isPlus )
            {
                for( auto a : values )
                {
                    sum += atoi(a.c_str());
                }
            }
            else
            {
                long long localVal = 1;
                for( auto a : values )
                {
                    localVal *= atoi(a.c_str());
                }
                sum += localVal;
            }
            i = j; // move up to next.
        }
    }
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;
    
    while( std::getline(input, currentLine) )
    {
        //std::cout << currentLine << "\n";
        processLine(currentLine);
    }

    input.close();
    std::cout << " sum : " << sum << "\n";
    return 0;
}