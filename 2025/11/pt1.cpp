#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>

struct Node
{
    std::string name; //unimportant
    std::vector< Node * > outputs;
};

std::vector< Node > nodes;

Node *findNode(std::string name)
{
    for( int i=0;i<nodes.size();i++)
    {
        if( nodes[i].name == name )
        {
            return &nodes[i];
        }
    }
    std::cout << "uh oh\n";
    return nullptr;
}

int countToOutput( Node *start, Node *end )
{
    if( start == end )
    {
        return 1;
    }
    int sum = 0;
    for( auto n : start->outputs )
    {
        sum += countToOutput(n, end);
    }
    return sum;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;
    std::vector< std::pair< std::string, std::vector< std::string > > > internalList;
    while( std::getline(input, currentLine) )
    {
        std::pair< std::string, std::vector< std::string > > listItem;
        std::cout << "line : " << currentLine << "\n";
        std::string line = currentLine;
        char *c = strtok(&line[0], ": ");
        listItem.first = std::string(c);
        c = strtok(nullptr, ": ");
        while( c != nullptr )
        {
            listItem.second.push_back(std::string(c));
            c = strtok(nullptr, ": ");
        }
        Node n;
        n.name = listItem.first;
        nodes.push_back(n);
        internalList.push_back(listItem);
    }
    // create output node
    Node outNode;
    outNode.name = "out";
    nodes.push_back(outNode);

    // construct graph
    for( auto list : internalList )
    {
        // find the node
        Node *n = findNode(list.first);
        for( auto s: list.second )
        {
            Node *output = findNode(s);
            n->outputs.push_back(output);
        }
    }

    // Now we have our graph.  Start at YOU
    Node *you = findNode("you");

    std::cout << countToOutput( you, findNode("out") ) << "\n";

    input.close();
    return 0;
}