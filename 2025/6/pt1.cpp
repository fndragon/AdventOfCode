#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

long long sum=0;
std::vector< std::vector < long long > > array;

void processLine(std::string &line)
{
    std::string localLine = line;
    // tokenize it
    int index=0;
    char *c = strtok(&localLine[0], " ");
    while( c != nullptr )
    {
        // handle last line
        if(( *c == '*') || (*c == '+') )
        {
            if( *c == '*')
            {
                long long localSum = 1;
                for(int i=0;i< array[index].size();i++)
                {
                    localSum *= array[index][i];
                }
                sum += localSum;
            }
            else if( *c == '+')
            {
                long long localSum = 0;
                for(int i=0;i< array[index].size();i++)
                {
                    localSum += array[index][i];
                }
                sum += localSum;
            }
        }
        long long value = std::atoll(c);
        if( array.size() <= index )
        {
            std::vector< long long > localArray = { value };
            array.push_back( localArray );
        }
        else
        {
            array[index].push_back(value);
        }
        ++index;
        c = strtok(nullptr, " " );
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