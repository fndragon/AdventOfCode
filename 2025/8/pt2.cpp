#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>

struct Location
{
    int x;
    int y;
    int z;
};

struct Circuit
{
    std::vector< int > junctions; // connected junctions for this circuit
};

struct Distance
{
    int junction1;
    int junction2;
    unsigned long long distance;
};

std::vector< Location > junctions;
std::vector< Circuit  > circuits;

std::vector< Distance > distances;  // map of location index to dest index + distance

struct less_than_distance
{
    inline bool operator() (const Distance& struct1, const Distance& struct2)
    {
        return (struct1.distance < struct2.distance);
    }
};

struct less_than_circuits
{
    inline bool operator() (const Circuit& struct1, const Circuit& struct2)
    {
        return (struct1.junctions.size() > struct2.junctions.size());
    }
};

unsigned long long distanceSquared(int i, int j)
{
    return (long long) (junctions[i].x - junctions[j].x) * (long long) (junctions[i].x - junctions[j].x) +
           (long long) (junctions[i].y - junctions[j].y) * (long long) (junctions[i].y - junctions[j].y) +
           (long long) (junctions[i].z - junctions[j].z) * (long long) (junctions[i].z - junctions[j].z);
}

void generateDistances(void)
{
    distances.clear();
    // 0..n
    for(int i=0;i<junctions.size();i++)
    {
        for(int j=i+1;j<junctions.size();j++)
        {
            distances.emplace_back(Distance { i,j,distanceSquared(i,j) } );
        }
    }
    // sort distances by distance
    std::sort(distances.begin(),distances.end(), less_than_distance());
    // print distances
    /*for( auto d: distances )
    {
       std::cout << "distance [" << junctions[d.junction1].x << "," <<junctions[d.junction1].y << "," <<junctions[d.junction1].z<<"] to [";
        std::cout << junctions[d.junction2].x << "," << junctions[d.junction2].y << "," << junctions[d.junction2].z << "]: ";
        std::cout << d.distance << "\n";
    }*/
}

bool getAvailableConnection(Distance &output)
{
    bool found = false;
    output.distance = std::numeric_limits<unsigned long long>::max();
    // find the shortest distance that doesn't have both endpoints in the same circuit
    for(int i=0;i<distances.size();i++)
    {
        bool valid = true;
        for(int j=0;j<circuits.size();j++)
        {
            bool foundj1 = std::find(circuits[j].junctions.begin(),circuits[j].junctions.end(), distances[i].junction1) != circuits[j].junctions.end();
            bool foundj2 = std::find(circuits[j].junctions.begin(),circuits[j].junctions.end(), distances[i].junction2) != circuits[j].junctions.end();

            if( foundj1 && foundj2 )
            {
                valid = false;
                break; // skip this
            }
        }
        
        if(valid)
        {
            // compare to current minimum distance
            if( distances[i].distance < output.distance )
            {
                output = distances[i];
                return true; // assumes distances are sorted
            }
        }
    }

    return found;
}
void printCircuits(void)
{
    for( int i=0;i<circuits.size();i++ )
    {
        if( circuits[i].junctions.empty()) continue;
        std::cout << "[" << i << "] = { ";
        for(int j=0;j<circuits[i].junctions.size();j++)
        {
            std::cout << circuits[i].junctions[j] << " ";
        }
        std::cout << "}\n";
    }
}

void connectCircuit( Distance connection )
{
    // find the circuit that contains the first
    int sourceCircuit = -1;
    for(int i=0;i<circuits.size();i++)
    {
        if(std::find(circuits[i].junctions.begin(),circuits[i].junctions.end(),connection.junction1) != circuits[i].junctions.end())
        {
            sourceCircuit = i;
        }
    }
    // find the destination circuit that contains the second
    for(int i=0;i<circuits.size();i++)
    {
        if( std::find(circuits[i].junctions.begin(),circuits[i].junctions.end(),connection.junction2) != circuits[i].junctions.end())
        {
            std::cout << "Moving " << sourceCircuit << " to " << i << "\n";
    
            // move all locations in the second circuit to the first
            circuits[sourceCircuit].junctions.insert( circuits[sourceCircuit].junctions.end(), circuits[i].junctions.begin(), circuits[i].junctions.end() );
            circuits[i].junctions.clear();
            circuits.erase(circuits.begin() + i);
            return;
        }
    }
}

bool isValidConnection( Distance &d )
{
    bool valid = true;
    for(int j=0;j<circuits.size();j++)
    {
        bool foundj1 = std::find(circuits[j].junctions.begin(),circuits[j].junctions.end(), d.junction1) != circuits[j].junctions.end();
        bool foundj2 = std::find(circuits[j].junctions.begin(),circuits[j].junctions.end(), d.junction2) != circuits[j].junctions.end();

        if( foundj1 && foundj2 )
        {
            valid = false;
            break; // skip this
        }
    }

    return valid;
}
int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    std::vector< int > size;
    bool getInput = false;
    while( std::getline(input, currentLine) )
    {
        //std::cout << "line : " << currentLine << "\n";
        Location l;
        std::string local = currentLine;
        char *c = strtok(&local[0], ",");
        l.x = atoi(c);
        c = strtok(nullptr, ",");
        l.y = atoi(c);
        c = strtok(nullptr, " \n");
        l.z = atoi(c);
        junctions.push_back(l);
        // each junction starts with its own circuit
        Circuit circuit;
        circuit.junctions.push_back(junctions.size()-1);
        circuits.push_back(circuit);
    }

    generateDistances();

    // distances are sorted.
    // process until we have 1 circuit!
    std::vector< Distance >::iterator index = distances.begin();
    while( circuits.size() > 1)
    {
        //std::cout << "step " << i << " numCircuits: " << circuits.size() << "\n";
        //printCircuits();
        // identify the shortest distance that isn't between two already connected circuits
        Distance availableConnection = *index;
        if( isValidConnection(availableConnection) )
        {
            connectCircuit(availableConnection);
        }
        if( circuits.size() > 1) ++index;
        //if (!getAvailableConnection(availableConnection) ) break;
        //connectCircuit(availableConnection);
    }
    // index has the last connection
    std::cout << index->junction1 << " and " << index->junction2 << "\n";

    std::cout << junctions[index->junction1].x << " x and " << junctions[index->junction2].x << " x\n";

    std::cout << junctions[index->junction1].x * junctions[index->junction2].x << "\n";
    
    // print/calc size of circuits:
    std::sort(circuits.begin(), circuits.end(), less_than_circuits());

    std::cout << "123: " << circuits[0].junctions.size() << " " << circuits[1].junctions.size() << " " << circuits[2].junctions.size() << "\n";
    std::cout << "top 3 multiplied are " << (circuits[0].junctions.size() * circuits[1].junctions.size() * circuits[2].junctions.size()) << "\n";

    input.close();
    return 0;
}