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

void printProduced(void)
{
    for(auto a : production)
    {
        std::cout << " [" << a.first << "] = " << a.second << "\n";
    }
}

int totalOreProduced = 0;

// need 1 fuel:  generate 7 A
//  need 7 A:  generate 10 ore
//   - 10 ore generated, consume 10 to produce 10 A
//  - 10 A generated, consume 7 to generate 1 fuel
void generate(std::string s, int count)
{
    Recipe *r = findRecipe(s);
    if( nullptr == r ) { std::cout << "NOOO\n"; return; }
    // produce this recipe - generate inputs first
    // find out how many total consumptions that we need to do
    // only check self
    int numberToGenerate = count - production[s];
    int timesToGenerate = (numberToGenerate + r->name.second - 1 ) / r->name.second; // at least one time.
    
    std::cout << "Recipe for " << r->name.first << " requires ";
    for(auto a : r->inputs) { std::cout << a.second << " [" << a.first << "], "; };
    std::cout << "\n";
    printProduced();

    if( numberToGenerate <= 0 ) 
    {
        // shortcut to exit.  We don't have to generate any more of OURSELVES.
        return;
    }

    // ensure we have generated enough inputs to consume
    for(int i=0;i<r->inputs.size();i++)
    {
        generate(r->inputs[i].first, r->inputs[i].second * timesToGenerate);
        // and now, before we continue on to the next dependency, consume the amount we need of ourselves!
        production[r->inputs[i].first] -= r->inputs[i].second * timesToGenerate; 
    }

    // and generate our output
    production[r->name.first] += r->name.second * timesToGenerate;
    if( r->name.first == "ORE") totalOreProduced += r->name.second * timesToGenerate;
}

void calculateOreForOneFuel(void)
{
    production.clear();
    generate("FUEL", 1);
    return;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
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
        c = strtok(nullptr, " ,");
        while( c )
        {
            count = std::atoi(c);
            c = strtok(nullptr, " ,");
            name = c;
            r.inputs.push_back(std::make_pair(name, count));
            c = strtok(nullptr, " ,");
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

    calculateOreForOneFuel();
    std::cout << "Total Ore is " << totalOreProduced << "\n";

    input.close();
    return 0;
}