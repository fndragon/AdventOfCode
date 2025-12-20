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
    std::string name;
    bool visited;
    std::vector< Node * > outputs;
};

std::vector< Node > nodes;
int nodeCount = 0;
void resetNodes(void)
{
    for(int i=0;i<nodes.size();i++)
    {
        nodes[i].visited = false;
    }
    nodeCount = 0;
}

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

int countToOutput( Node *start, Node *end)
{
    if( start->visited == true ) 
    {
        return 0;
    }
    if( start == end )
    {
        return 1;
    }
    int sum = 0;
    for(int i=0;i< start->outputs.size();i++)
    {
        //std::cout << "[" << start->name << "] -> ]" << start->outputs[i]->name << "[\n";
        sum += countToOutput(start->outputs[i], end);
    }
    start->visited = true; nodeCount++;
    return sum;
}

int checkPath( std::string a, std::string b)
{
    resetNodes();
    int count = countToOutput(findNode(a), findNode(b));
    std::cout << "[[ Path from \"" << a << "\" to \"" << b << "\" = " << count << "]] (" << nodeCount << ")\n";
    return count;
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
    resetNodes();
    // construct graph backwards!
    for(int j=0;j<internalList.size();j++)
    {
        // find the node
        Node *n = findNode(internalList[j].first);
        for( int i=0; i < internalList[j].second.size();i++)
        {
            //backward:
            //findNode(s)->outputs.push_back(n);
            //forward
            n->outputs.push_back(findNode(internalList[j].second[i]));
        }
    }
    
    // svr > dac > fft > out
    // or
    // svr > fft > dac > out
    // 
    // if dac > fft = 0, it's the second path.
    int paths =1;
    //paths = countToOutput(findNode("out"), findNode("fft"), true);
    //paths = checkPath("fft", "dac");
    paths *= checkPath("svr", "fft");
    paths *= checkPath("fft","dac");
    checkPath("dac","fft");
    paths *= checkPath("dac", "out");
    std::cout << " total permutations = " << paths << "\n";
    input.close();
    return 0;
}