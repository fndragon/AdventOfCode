#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

struct Ingredient
{
    std::string name;
    long long capacity;
    long long durability;
    long long flavor;
    long long texture;
    long long calories;
};

std::vector< Ingredient > kitchen;

void addIngredient(std::string &line)
{
    Ingredient i;
    std::string localLine = line;
    char *c = strtok(&localLine[0], ":, ");
    i.name = std::string(c);
    c = strtok(nullptr, ":, ");
    c = strtok(nullptr, ":, ");
    i.capacity = atoll(std::string(c).c_str());
    c = strtok(nullptr, ":, ");
    c = strtok(nullptr, ":, ");
    i.durability = atoll(std::string(c).c_str());
    c = strtok(nullptr, ":, ");
    c = strtok(nullptr, ":, ");
    i.flavor = atoll(std::string(c).c_str());
    c = strtok(nullptr, ":, ");
    c = strtok(nullptr, ":, ");
    i.texture = atoll(std::string(c).c_str());
    c = strtok(nullptr, ":, ");
    c = strtok(nullptr, ":, ");
    i.calories = atoll(std::string(c).c_str());
    kitchen.push_back(i);
}

long long calculate( std::vector< int > &ratio )
{
    long long capacity = 0, durability = 0, flavor = 0, texture = 0;
    long long calories = 0;
    for( int i=0;i<kitchen.size();i++)
    {
        capacity += (long long) ratio[i] * kitchen[i].capacity;
        durability += (long long) ratio[i] * kitchen[i].durability;
        flavor += (long long) ratio[i] * kitchen[i].flavor;
        texture += (long long) ratio[i] * kitchen[i].texture;
        calories += (long long) ratio[i] * kitchen[i].calories;
    }
    if( capacity < 0) capacity = 0;
    if( durability < 0) durability = 0;
    if( flavor < 0 ) flavor = 0;
    if( texture < 0 ) texture = 0;
    if( calories != 500 ) return 0;
    long long result = capacity * durability * flavor * texture;
    return result;
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
        addIngredient(currentLine);
    }

    long long result =0;
    std::vector< int > ratio;
    ratio.resize(4);
    for (int i=0;i<=100;i++)
        for (int j=0;j<=100-i;j++)
            for(int k=0;k<=100-i-j;k++)
                for(int l=0;l<=100-i-j-k;l++)
                {
                    if( i + j + k + l != 100) continue;
                    ratio[0] = i;
                    ratio[1] = j;
                    ratio[2] = k;
                    ratio[3] = l;
                    result = std::max<long long>(result, calculate(ratio));
                }
    
    std::cout << "result : "<< result << "\n";
    input.close();
    return 0;
}