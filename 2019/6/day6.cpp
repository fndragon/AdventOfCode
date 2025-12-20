#include <iostream>
#include <fstream>

struct Node
{
    std::string name;
    std::string parentName;
    Node *parent;
};

std::vector< Node > nodes;

Node *findNode(std::string const &name)
{
    for(int i=0;i<nodes.size();i++)
    {
        if( nodes[i].name == name )
        {
            return &nodes[i];
        }
    }
    return nullptr;
}

int main()
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        std::string parent = currentLine.substr(0, currentLine.find_first_of(')'));
        std::string name = currentLine.substr(currentLine.find_first_of(')')+1);
        std::cout << currentLine << "\n";
        std::cout << " parent " << parent << " :: " << name << "\n";
        Node N;
        N.name = name;
        N.parentName = parent;
        N.parent = nullptr;
        nodes.push_back(N);
    }
    // once all nodes are loaded, loop several times until we stop adding parent links:
    bool loop=true;
    while(loop)
    {
        loop = false;
        for(int i=0;i<nodes.size();i++)
        {
            if( nodes[i].parent == nullptr )
            {
                if( findNode(nodes[i].parentName) != nullptr )
                {
                    nodes[i].parent = findNode(nodes[i].parentName);
                    loop = true;
                }
            }
        }
    }

    // for each node, count the number of orbits
    int total=0;
    for( int i = 0; i < nodes.size();i++)
    {
        int localCount = 0;
        Node *node = &nodes[i];
        while( node != nullptr )
        {
            localCount++;
            node = node->parent;
        }
        std::cout << "Node: " << nodes[i].name << " orbits " << localCount << "\n";
        total += localCount;
    }
    std::cout << "Pt1 : Total is " << total << "\n";

    // Got to go between node "YOU" and "SAN"
    std::string you("YOU");
    std::string santa("SAN");
    Node *youNode = findNode(you);
    Node *santaNode = findNode(santa);
    std::vector< Node * > santaTrack;
    Node *index = santaNode;
    while(index != nullptr)
    {
        santaTrack.push_back(index);
        index = index->parent;
    }
    bool foundLink = false;
    int distance = 0;
    total = 0;
    Node *youLink = nullptr;
    index = youNode->parent;
    while( true )
    {
        if(std::find(santaTrack.begin(), santaTrack.end(), index) != santaTrack.end())
        {
            // First time we are here we are at the pivot node.
            youLink = index;
            break;
        }
        distance++; // only add to distance after a failure.
        index = index->parent;
    }

    total = distance;
    index = santaNode->parent;
    // now go from santa to youLink
    while(true )
    {
        if( index == youLink )
        {
            break;
        }
        distance++;
        index = index->parent;
    }
    std::cout << "distance is " << distance << "\n";
    input.close();
    return 0;
}