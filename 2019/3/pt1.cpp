#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>

std::vector< std::string > wire1;
std::vector< std::string > wire2;

struct Point
{
    int x, y;
};

struct Edge
{
    Point p1;
    Point p2;
    int position;
};

std::vector< Edge > edges;
std::vector< Edge > edges2;

Point parse( std::string const &instruction, Point const &coord)
{
    Point result = coord;
    int mod = std::stoi(instruction.substr(1,instruction.length()-1));
    switch(instruction[0])
    {
        case 'R':
            result.x += mod;
            break;
        case 'L':
            result.x -= mod;
            break;
        case 'U':
            result.y += mod;
            break;
        case 'D':
            result.y -= mod;
            break;
        default:
        break;
    }
    return result;
}

std::vector< long long > manhattanDistances;
int shortestWalk; //step#, distance
int calcDistance( Edge &e )
{
    // get steps to go from one end of e to the other
    int maxX = std::max<int>(e.p1.x,e.p2.x);
    int minX = std::min<int>(e.p1.x,e.p2.x);
    int maxY = std::max<int>(e.p1.y,e.p2.y);
    int minY = std::min<int>(e.p1.y,e.p2.y);
    return (maxX - minX) + (maxY - minY); // abs 
}

int countToXY( int x, int y )
{
    int totalEdge1=0, totalEdge2=0;
    // get contribution from edges1
    for(int i=0;i<edges.size();i++)
    {
        // if x,y is between this edge:
        if((x <= std::max<int>(edges[i].p1.x,edges[i].p2.x)) && 
           (x >= std::min<int>(edges[i].p1.x,edges[i].p2.x)) &&
           (y <= std::max<int>(edges[i].p1.y,edges[i].p2.y)) &&
           (y >= std::min<int>(edges[i].p1.y,edges[i].p2.y)))
        {
            // add up steps from p1(important) to position
            totalEdge1 += abs(x - edges[i].p1.x);
            totalEdge1 += abs(y - edges[i].p1.y);
            break;
        }
        else
        {
            totalEdge1 += calcDistance(edges[i]);
        }
    }
    // get contribution from edges2
    for(int i=0;i<edges2.size();i++)
    {
        // if x,y is between this edge:
        if((x <= std::max<int>(edges2[i].p1.x,edges2[i].p2.x)) && 
           (x >= std::min<int>(edges2[i].p1.x,edges2[i].p2.x)) &&
           (y <= std::max<int>(edges2[i].p1.y,edges2[i].p2.y)) &&
           (y >= std::min<int>(edges2[i].p1.y,edges2[i].p2.y)))
        {
            // add up steps from p1(important) to position
            totalEdge2 += abs(x - edges2[i].p1.x);
            totalEdge2 += abs(y - edges2[i].p1.y);
            break;
        }
        else
        {
            totalEdge2 += calcDistance(edges2[i]);
        }
    }
    return totalEdge1 + totalEdge2;
}

void intersect(Edge &e1, Edge &e2, int e2Index, int stepsE1)
{
    if((e1.p1.x == e1.p2.x) && (e2.p1.x == e2.p2.x )) return; // both x components are equal, parallel
    if((e1.p1.y == e1.p2.y) && (e2.p1.y == e2.p2.y )) return; // same with y
    // Now we know one is vertical and one is horizontal.
    if(e1.p1.x == e1.p2.x)
    {
        // e1 is vertical, e2 is horizontal
        int minX = std::min<int>(e2.p1.x, e2.p2.x);
        int maxX = std::max<int>(e2.p1.x, e2.p2.x);
        int minY = std::min<int>(e1.p1.y, e1.p2.y);
        int maxY = std::max<int>(e1.p1.y, e1.p2.y);
        if(( e1.p1.x > minX ) && (e1.p1.x < maxX ) &&
           ( e2.p1.y > minY ) && (e2.p1.y < maxY ))
        {
            Point p;
            p.x = e1.p1.x; // e1 is vert
            p.y = e2.p1.y; // e2 is horiz
            manhattanDistances.push_back(p.x + p.y);

            int steps = countToXY(p.x,p.y);
            if(steps < shortestWalk) shortestWalk = steps;
            return; // we're done.
        }
    }
    else
    {
        // e2 is vertical, e1 is horizontal
        int minX = std::min<int>(e1.p1.x, e1.p2.x);
        int maxX = std::max<int>(e1.p1.x, e1.p2.x);
        int minY = std::min<int>(e2.p1.y, e2.p2.y);
        int maxY = std::max<int>(e2.p1.y, e2.p2.y);
        if(( e1.p1.y > minY ) && (e1.p1.y < maxY ) &&
           ( e2.p1.x > minX ) && (e2.p1.x < maxX ))
        {
            Point p;
            p.x = e2.p1.x; // e1 is horiz
            p.y = e1.p1.y; // e2 is vert
            manhattanDistances.push_back(p.x + p.y);
            int steps = countToXY(p.x,p.y);
            if(steps < shortestWalk) shortestWalk = steps;
            return; // we're done.
        }
    }
}


int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;
    bool wire1Done = false;
    while( std::getline(input, currentLine) )
    {
        std::cout << currentLine << "\n";
        char delimiter = ',';
        std::istringstream ss(currentLine);
        std::string token; // The std::string variable used to hold each token
        std::vector<std::string> tokens;

        // Use getline with a specific delimiter
        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        if( !wire1Done )
        {
            wire1 = tokens;
            wire1Done = true;
        }
        else
        {
            wire2 = tokens;
        }
    }

    // generate pairs of coordinates (incl. start) to compare against for wire1:
    Point start = {0,0};
    for(int i=0;i<wire1.size();i++)
    {
        Edge e;
        e.p1 = start;
        e.p2 = parse(wire1[i], e.p1);
        e.position = i;
        edges.emplace_back( e );
        start = e.p2;
    }
    start = {0,0};
    for(int i=0;i<wire2.size();i++)
    {
        Edge e;
        e.p1 = start;
        e.p2 = parse(wire2[i], e.p1);
        e.position = i;
        edges2.emplace_back( e );
        start = e.p2;
    }

    // results:
    shortestWalk = 999999999;
    Point coordinates = {0,0};
    int steps = 0;
    for(int i=0;i<wire2.size();i++)
    {
        Edge e;
        e.p1 = coordinates;
        e.p2 = parse(wire2[i],e.p1);
        // compare with every edge in wire1
        for(int j=0;j<edges.size();j++)
        {
            intersect(e, edges[j], j, steps);
        }
        coordinates = e.p2;
        // Sum up the distance travelled along wire2[i]:
        steps += calcDistance(e);
    }

    // print
    int lowest = 99999999;
    for(int i=0;i<manhattanDistances.size();i++)
    {
        std::cout << i << ": " << manhattanDistances[i] << "\n";
        if(lowest > manhattanDistances[i])
        {
            lowest = manhattanDistances[i];
        }
    }
    std::cout << "Lowest Manhattan = " << lowest << "\n";

    std::cout << "Pt2: " << shortestWalk << "\n";
    input.close();
    return 0;
}