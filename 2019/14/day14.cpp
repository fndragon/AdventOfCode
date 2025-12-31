#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>
#include <vector>
#include <queue>
#include <string.h>

struct Recipe
{
    std::pair< std::string, int > name; // name, quantity
    std::vector< std::pair< std::string, int > > inputs; // name, quantity
};

std::vector< Recipe > recipes;

Recipe *findRecipe(std::string &s)
{
    for(int i=0;i<recipes.size();i++)
    {
        if( recipes[i].name.first == s ) return &recipes[i];
    }
    return nullptr;
}

std::map< std::string, int > production; // all things produced so far

// need 1 fuel:  generate 7 A
//  need 7 A:  generate 10 ore
//   - 10 ore generated, consume 10 to produce 10 A
//  - 10 A generated, consume 7 to generate 1 fuel

void generate(std::string s, int count)
{
    Recipe *r = findRecipe(s);
    if( nullptr == r ) { std::cout << "NOOO\n"; return; }
    // produce this recipe - generate inputs first
    for(int i=0;i<r->inputs.size();i++)
    {
        generate(r->inputs[i].first, r->inputs[i].second);
    }

    // now consume our specific inputs
    for(int i=0;i<r->inputs.size();i++)
    {
        production[r->inputs[i].first] -= r->inputs[i].second;
        if( production[r->inputs[i].first] < 0 ) 
        {
            std::cout << "NEGATIVE\n";
        }
    }
    // and generate our output
    production[r->name.first] += r->name.second * count;
}

int calculateOreForOneFuel(void)
{
    production.clear();
    generate("FUEL", 1);
    return production["ORE"];
}

int main(void)
{
    std::ifstream input;
    input.open("example.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        Recipe r;
        std::cout << currentLine << "\n";
        // 7 A, 1 B => 1 C for example
        std::string inputs = currentLine.substr(0, currentLine.find_first_of('=')-1);
        std::string output = currentLine.substr(currentLine.find_first_of('>')+2);
        std::cout << "input [" << inputs << "] ==> [" << output << "]\n";
        // destructively use strtok on inputs
        char *c = strtok(&inputs[0], " ,");
        int count = std::atoi(c);
        c = strtok(nullptr, " ,");
        std::string name(c);
        r.inputs.push_back(std::make_pair(name, count));
        while( c = strtok(nullptr, " ,") )
        {
            count = std::atoi(c);
            c = strtok(nullptr, " ,");
            name = c;
            r.inputs.push_back(std::make_pair(name, count));
        }

        // Process the result the same way
        c = strtok(&output[0], " ");
        count = std::atoi(c);
        c = strtok(nullptr, " ");
        name = c;
        r.name = std::make_pair(name, count);

        recipes.push_back(r);
    }
    Recipe ore;
    ore.name = std::make_pair("ORE", 1);
    recipes.push_back(ore); // ore is free.

    int result = calculateOreForOneFuel();
    std::cout << "Total Ore is " << result << "\n";

    input.close();
    return 0;
}