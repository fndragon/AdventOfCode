#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

std::vector< std::vector< int > > grid;

void addLineToMap(std::string &line)
{
    std::vector< int > mazeLine;
    // Convert from ./@ to 0/1
    for(int i=0;i<line.size();i++)
    {
        mazeLine.push_back( line[i] == '@' ? 1 : 0);
    }
    grid.push_back(mazeLine);
}

bool checkCoords(int row, int col)
{
    if(( col >= 0 ) && ( col < grid[0].size()) && (row >= 0 ) && ( row < grid.size()))
    {
        if( grid[row][col] == 1)
        {
            return true;
        }
    }
    return false;
}

bool inspectGridCoords(int row, int col, int &nearby)
{
    // first check the center location:
    if(!checkCoords(row,col))
    {
        return false;
    }
    // check 8 cardinal directions:
    int numAround = 0;
    if(checkCoords(row-1,col-1)) numAround++;
    if(checkCoords(row  ,col-1)) numAround++;
    if(checkCoords(row+1,col-1)) numAround++;
    if(checkCoords(row-1,col  )) numAround++;

    if(checkCoords(row+1,col  )) numAround++;
    if(checkCoords(row-1,col+1)) numAround++;
    if(checkCoords(row  ,col+1)) numAround++;
    if(checkCoords(row+1,col+1)) numAround++;
    nearby = numAround;
    return true;
}

void inspectGrid(void)
{
    for(int row = 0; row < grid.size(); row++)
    {
        for( int col = 0; col < grid[0].size();col++)
        {
            int nearby;
            if (inspectGridCoords(row,col, nearby))
            {
                if( nearby < 4)
                {
                    sum++;
                    std::cout << 'x';
                }
                else
                {
                    std::cout << '@';
                }
            }
            else
            {
                std::cout << '.';
            }
        }
        std::cout << "\n";
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
        addLineToMap(currentLine);
    }

    // Now we have the full grid.
    inspectGrid();

    input.close();
    std::cout << " sum : " << sum << "\n";
    return 0;
}