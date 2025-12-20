#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

std::vector< std::vector< int > > grid;
std::vector< std::vector< int > > grid2;

void addLineToMap(std::string &line)
{
    std::vector< int > mazeLine;
    // Convert from ./@ to 0/1
    for(int i=0;i<line.size();i++)
    {
        mazeLine.push_back( line[i] == '@' ? 1 : 0);
    }
    grid.push_back(mazeLine);
    grid2.push_back(mazeLine); // construct identical
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

int inspectGrid(void)
{
    int localsum = 0;
    for(int row = 0; row < grid.size(); row++)
    {
        for( int col = 0; col < grid[0].size();col++)
        {
            int nearby;
            if (inspectGridCoords(row,col, nearby))
            {
                if( nearby < 4)
                {
                    localsum++;
                    sum++;
                    grid2[row][col] = 0;
                    //std::cout << 'x';
                }
                else
                {
                    grid2[row][col] = 1; 
                    //std::cout << '@';
                }
            }
            else
            {
                grid2[row][col] = 0;
                //std::cout << '.';
            }
        }
        //std::cout << "\n";
    }
    if( localsum > 0 )
    {
        // copy grid2 to grid
        grid = grid2;
    }
    return localsum;
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
    while( 0 != inspectGrid() )
    {
        std::cout << "doing another round...\n";
    }

    input.close();
    std::cout << " sum : " << sum << "\n";
    return 0;
}