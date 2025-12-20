#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>

std::vector< std::pair< int, int > > grid;

struct Edge
{
    std::pair< int, int > p1;
    std::pair< int, int > p2;
};

std::vector< Edge > edges;

long long makeSquare( std::pair< int, int > p1, std::pair< int, int > p2)
{
    // inclusive coords
    long long diffX = abs( p1.first - p2.first ) + 1;
    long long diffY = abs( p1.second - p2.second ) + 1;
    return diffX * diffY;
}

bool intersect( long long minX, long long minY, long long maxX, long long maxY )
{
    long long iminX, imaxX, iminY, imaxY;
    for(auto e : edges)
    {
        iminX = std::min(e.p1.first, e.p2.first);
        imaxX = std::max(e.p1.first, e.p2.first);
        iminY = std::min(e.p1.second, e.p2.second);
        imaxY = std::max(e.p1.second, e.p2.second);
        if( ( minX < imaxX ) && (maxX > iminX) && (minY < imaxY) && (maxY > iminY))
        {
            return true;
        }
    }
    return false;
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
        
        Edge e;
        if( grid.size() > 0)
        {
            e.p1 = grid.back();
        }
        grid.push_back(p);
        if( grid.size() > 1)
        {
            e.p2 = p;
            edges.push_back(e);
        }
    }
    // connect front and back
    Edge connection;
    connection.p1 = grid.back();
    connection.p2 = grid[0];
    edges.push_back( connection);

    long long result = 0;
    for( int i=0;i < grid.size() - 1;i++)
    {
        for( int j=(i+1);j<grid.size();j++)
        {
            auto start = grid[i];
            auto end   = grid[j];

            long long minX = std::min(start.first, end.first);
            long long maxX = std::max(start.first, end.first);
            long long minY = std::min(start.second, end.second);
            long long maxY = std::max(start.second, end.second);

            if( false == intersect(minX, minY, maxX, maxY))
            {
                long long area = makeSquare(start, end);
                if( area > result )
                {
                    result = area;
                }
            }
        }
    }
    // 520940 too low
    std::cout << "area was " << result << " squares big\n";
    input.close();
    return 0;
}