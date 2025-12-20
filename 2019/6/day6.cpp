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

    input.close();
    return 0;
}