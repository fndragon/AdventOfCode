#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>

std::vector< std::string > manifold;
std::vector< long long > value;
int numSplit = 0;
void processLine(int i)
{
    for(int j=0;j<manifold[i].size();j++)
    {
        char &c = manifold[i][j];
        switch (c)
        {
            case '^':
                // look above to see if we were triggered:
                if( manifold[i-1][j] == '|' )
                {
                    numSplit++;
                    if(j-1 >= 0) manifold[i][j-1] = '|';
                    if(j+1 <manifold[i].size()) manifold[i][j+1] = '|';
                }
                if( j-1 >= 0) value[j-1] += value[j];
                if( j+1 <value.size()) value[j+1] += value[j];
                value[j] = 0;
                break;
            case '.':
                if( i-1 >= 0) // look up
                {
                    if((manifold[i-1][j] == 'S') || (manifold[i-1][j] == '|'))
                    {
                        manifold[i][j] = '|';
                    }
                }
                // leave value untouched, it just falls.
                break;
            case '|':
                break;
            case 'S':
                value[j] = 1;
                break;
            default:
                break;
        }
    }
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    std::vector< int > size;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        manifold.push_back( currentLine.c_str());
    }
    value.resize(manifold[0].size(), 0);

    for( int i=0;i<manifold.size();i++)
    {
        processLine(i);
    }

    long long sum = 0;
    for(int i=0;i<value.size();i++)
    {
        sum += (long long) value[i];
    }

    std::cout << numSplit << "\n";
    std::cout << "number of time dimensions : " << sum << "\n";
    input.close();
    return 0;
}