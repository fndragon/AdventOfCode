#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>

std::vector< std::string > grid;
std::vector< std::string > postStepGrid;

int numSplit = 0;

int checkCell(int x, int y)
{
    if ( x < 0 ) return 0;
    if ( y < 0 ) return 0;
    if ( x >= grid[0].size()) return 0;
    if ( y >= grid.size()) return 0;
    return ( grid[y][x] == '#' ? 1 : 0);
}

int neighbors(int x, int y)
{
    int g = grid[0].size();
    int count =0;
    // look at 8 diagonally adjacent cells:
    count += checkCell(x-1,y-1);
    count += checkCell(x  ,y-1);
    count += checkCell(x+1,y-1);
    count += checkCell(x-1,y  );
//  count += checkCell(x  ,y  );
    count += checkCell(x+1,y  );
    count += checkCell(x-1,y+1);
    count += checkCell(x  ,y+1);
    count += checkCell(x+1,y+1);
    return count;
}

void processLine(int y)
{
    // game of life for this line.
    for(int x=0;x<grid[y].size();x++)
    {
        char c = grid[y][x];
        if( c == '.' )
        {
            // a light which is off turns on if exactly 3 neighbors are on
            if(neighbors(x,y) == 3)
            {
                postStepGrid[y][x] = '#';
            }
            else
            {
                postStepGrid[y][x] = '.';
            }
        }
        else
        {
            //on previously.
            int n = neighbors(x,y);
            if( n >=2 && n <= 3)
            {
                postStepGrid[y][x] = '#';
            }
            else
            {
                postStepGrid[y][x] = '.';
            }
        }
    }
}

void step(void)
{
    // set four corners every timestep now
    grid[0][0] = '#';
    grid[0].back() = '#';
    grid.back().back() = '#';
    grid.back().at(0) = '#';

    for( int i=0;i<grid.size();i++)
    {
        processLine(i);
    }
    grid = postStepGrid;

    grid[0][0] = '#';
    grid[0].back() = '#';
    grid.back().back() = '#';
    grid.back().at(0) = '#';
    // print grid
    for(int i=0;i<grid.size();i++)
    {
        std::cout << grid[i] << "\n";
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
        grid.push_back( currentLine.c_str());
        postStepGrid.push_back( currentLine.c_str());
    }
    std::cout << "start grid: \n";

    // print grid
    for(int i=0;i<grid.size();i++)
    {
        std::cout << grid[i] << "\n";
    }

    std::cout << " stepping 100 times\n";
    for(int i=0;i<100;i++)
    {
        step();
    }

    //count #'s:
    int hashcount=0;
    for(int j=0;j<grid.size();j++)
    {
        for(int i=0;i<grid[j].size();i++)
        {
            if(grid[j][i] == '#') hashcount++;
        }
    }
std::cout << "Hash count: " << hashcount << "\n";
    input.close();
    return 0;
}