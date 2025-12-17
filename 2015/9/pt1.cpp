#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

struct Node
{
    std::vector< std::pair< std::string , int > > links;
    bool visited;
};

std::vector< std::pair< std::string, Node > > maps;

std::stringstream ss;

void resetMap(void)
{
    std::vector< std::pair< std::string, Node > >::iterator m = maps.begin();
    while( m != maps.end())
    {
        m->second.visited = false;
        ++m;
    }
    ss.str("");
}

Node *findCity(std::string city)
{
    std::vector< std::pair< std::string, Node> >::iterator m = maps.begin();
    while( m != maps.end())
    {
        if( m->first == city )
        {
            return &m->second;
        }
        ++m;
    }
    return nullptr;
}

void addToMap(std::string line)
{
    std::string localLine = line;
    char *c = strtok(&line[0], " ");
    std::string myCityName(c);
    c = strtok(nullptr, " ");
    c = strtok(nullptr, " ");
    std::string destination(c);
    c = strtok(nullptr, " =");
    std::string distance(c);
    int d = atoi(distance.c_str());

    // see if it exists

    if( findCity(myCityName) == nullptr)
    {
        maps.push_back(std::make_pair(myCityName, Node()));
    }
    if( findCity(destination) == nullptr)
    {
        maps.push_back(std::make_pair(destination, Node()));
    }

    // update and set
    findCity(myCityName)->links.push_back(std::make_pair(destination, d));
    findCity(destination)->links.push_back(std::make_pair(myCityName, d));
    return;
}
std::vector< int > distances;

// Visit city
int travelPath( std::vector< int > &cities )
{
    int distance =0;
    // Sum up the distances for this path from city to city
    for(int i=0;i<cities.size()-1;i++)
    {
        std::string startCity = maps[cities[i]].first;
        std::string nextCity  = maps[cities[i+1]].first;
        Node *startC = findCity(startCity);
        std::vector< std::pair< std::string , int > >::iterator link=startC->links.begin();
        while( link != startC->links.end())
        {
            if( link->first == nextCity)
            {
                distance += link->second;
                break;
            }
            ++link;
        }
    }
    return distance;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    long sum=0;
    bool setup = true;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        addToMap(currentLine);
    }

    // parse distnaces.
    int lowestDistance = 9999999;

    // Iterate by index
    int size = maps.size();
    std::vector< int > perms;
    for(int i=0;i<size;i++)
    {
        perms.push_back(i);
    }

    do {
        for (int x : perms)
        {
            std::cout << x << " -> ";
        }
        int distance = travelPath(perms);
        std::cout << " = " << distance;
        std::cout << std::endl;
        if( lowestDistance > distance )
        {
            lowestDistance = distance;
        }
    } while (std::next_permutation(perms.begin(), perms.end()));

    std::cout << "Lowest distance = " << lowestDistance << "\n";
    input.close();
    return 0;
}