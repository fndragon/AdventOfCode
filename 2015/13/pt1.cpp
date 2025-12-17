#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

struct Node
{
    std::vector< long long > neighbors; // index , happiness of neighbor
};

std::vector< std::pair< std::string, Node > > table;

std::stringstream ss;

void resetMap(void)
{
    ss.str("");
}

Node *findPerson(std::string name)
{
    std::vector< std::pair< std::string, Node> >::iterator m = table.begin();
    while( m != table.end())
    {
        if( m->first == name )
        {
            return &m->second;
        }
        ++m;
    }
    return nullptr;
}

int getPersonIndex(std::string name)
{
    for(int i=0;i < table.size();i++)
    {
        if( table[i].first == name)
        {
            return i;
        }
    }
    std::cout << "error!\n";
    return -1; // 
}

void addToTable(std::string line)
{
    std::string localLine = line;
    char *c = strtok(&line[0], " ");
    std::string person(c);
    c = strtok(nullptr, " "); // would
    c = strtok(nullptr, " ");
    std::string gainLose(c); // pos-neg
    c = strtok(nullptr, " ");
    std::string happiness(c); // happiness units
    c = strtok(nullptr, " "); // "happiness"
    c = strtok(nullptr, " "); // "units"
    c = strtok(nullptr, " "); // "by"
    c = strtok(nullptr, " "); // "sitting"
    c = strtok(nullptr, " "); // "next"
    c = strtok(nullptr, " "); // to
    c = strtok(nullptr, " "); // Neighbor.
    std::string neighborWithPeriod(c);
    std::string neighbor = neighborWithPeriod.substr(0, neighborWithPeriod.length() - 1);

    long long happinessValue = atoll(happiness.c_str());
    if( gainLose == "lose" )
    {
        happinessValue *= -1;
    }

    // see if it exists
    if( findPerson(person) == nullptr)
    {
        table.push_back(std::make_pair(person, Node()));
    }
    if( findPerson(neighbor) == nullptr)
    {
        table.push_back(std::make_pair(neighbor, Node()));
    }

    // update and set
    findPerson(person)->neighbors.resize( table.size()); // resize if necessray.
    findPerson(person)->neighbors[getPersonIndex(neighbor)] = happinessValue;
    //findPerson(destination)->links.push_back(std::make_pair(myCityName, d));
    return;
}
std::vector< int > distances;

long long calculateHappiness(std::vector< int > pos)
{
    long long happy = 0;

    // Check the happiness values for each neighbor (+/- index in pos)
    for( int i=0;i< pos.size();i++)
    {
        std::string currentPlayer = table[i].first;
        int leftIndex = i-1;
        if( leftIndex < 0 ) leftIndex = pos.size() - 1;
        int rightIndex = i+1;
        if( rightIndex >= pos.size() ) rightIndex = 0;
//        std::string leftNeighbor = table[pos[leftIndex]].first;
//        std::string rightNeighbor = table[pos[rightIndex]].first;
        Node *current = &table[pos[i]].second;
        // add happiness from currentplayer first
        happy += current->neighbors[pos[leftIndex]];
        happy += current->neighbors[pos[rightIndex]];
        //// do neighbors
        //happy += left->neighbors[i];
        //happy += right->neighbors[i];
//        std::cout << currentPlayer << " would gain " << current->neighbors[pos[leftIndex]] << " happiness units by sitting next to ";
//        std::cout << leftNeighbor << ".\n";
//
//        std::cout << currentPlayer << " would gain " << current->neighbors[pos[rightIndex]] << " happiness units by sitting next to ";
//        std::cout << rightNeighbor << ".\n";

    }
    return happy;
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
        addToTable(currentLine);
    }

    // parse distnaces.
    int happy = -99999999;

    // Iterate by index
    int size = table.size();
    std::vector< int > perms;
    for(int i=0;i<size;i++)
    {
        perms.push_back(i);
    }

    do {
        int localHappy = calculateHappiness(perms);
        std::cout << perms[0];
        for (int x : perms)
        {
            if( x == perms[0]) continue;
            std::cout << " -> " << x;
        }
        std::cout << " : " << localHappy;
        if( happy < localHappy )
        {
            happy = localHappy;
        }
        std::cout << " (new max: " << happy << ")\n";
    } while (std::next_permutation(perms.begin(), perms.end()));

    std::cout << "highD distance = " << happy << "\n";
    input.close();
    return 0;
}