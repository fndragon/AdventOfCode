#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>

std::vector< std::pair< int, int > > grid;

long long makeSquare( std::pair< int, int > p1, std::pair< int, int > p2)
{
    // inclusive coords
    long long diffX = abs( p1.first - p2.first ) + 1;
    long long diffY = abs( p1.second - p2.second ) + 1;
    return diffX * diffY;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        std::pair< int, int > p;
        //std::cout << "line : " << currentLine << "\n";
        std::string local = currentLine;
        char *c = strtok(&local[0], ",");
        p.first = atoi(c);
        c = strtok(nullptr, ",");
        p.second = atoi(c);
        grid.push_back(p);
    }

    //std::cout << makeSquare(std::make_pair(2,5), std::make_pair(9,7));

    long long best = 0;
    for(int i=0;i<grid.size();i++)
    {
        for(int j=i+1;j<grid.size();j++)
        {
            // get size of square
            long long localBest = makeSquare(grid[i], grid[j]);
            // store if it is larger than previous best
            if( localBest > best ) best = localBest;
        }
    }
    std::cout << "best square was " << best << " squares big\n";
    input.close();
    return 0;
}