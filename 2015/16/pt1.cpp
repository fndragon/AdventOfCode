#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

struct Sue
{
    int id;
    int children;
    int cats;
    int samoyeds;
    int pomeranians;
    int akitas;
    int vizslas;
    int goldfish;
    int trees;
    int cars;
    int perfumes;
};

std::vector< Sue > family;
//Sue 1: goldfish: 6, trees: 9, akitas: 0

void addItemToSue(Sue &sue, std::string name, std::string number)
{
    int num = std::atoi(number.c_str());
    if( "children" == name ) { sue.children = num; } else
    if( "cats" == name ) { sue.cats = num; } else
    if( "samoyeds" == name ) { sue.samoyeds = num; } else
    if( "pomeranians" == name ) { sue.pomeranians = num; } else
    if( "akitas" == name ) { sue.akitas = num; } else
    if( "vizslas" == name ) { sue.vizslas = num; } else
    if( "goldfish" == name ) { sue.goldfish = num; } else
    if( "trees" == name ) { sue.trees = num; } else
    if( "cars" == name ) { sue.cars = num; } else
    if( "perfumes" == name ) { sue.perfumes = num; }
}

void addSue(std::string &line)
{
    Sue sue = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    
    std::string localLine = line;
    char *c = strtok(&localLine[0], ",: "); //sue
    c = strtok(nullptr, ":, "); //id
    sue.id = atoi(std::string(c).c_str());
    c = strtok(nullptr, ":, ");
    std::string itemName(c);
    c = strtok(nullptr, ":, ");
    std::string itemNum(c);
    addItemToSue( sue, itemName, itemNum);
    c = strtok(nullptr, ":, ");
    itemName= std::string(c);
    c = strtok(nullptr, ":, ");
    itemNum= std::string(c);
    addItemToSue( sue, itemName, itemNum);
    c = strtok(nullptr, ":, ");
    itemName= std::string(c);
    c = strtok(nullptr, ":, ");
    itemNum= std::string(c);
    addItemToSue( sue, itemName, itemNum);
    family.push_back(sue);
}

void checkForSue(int index)
{
    bool rightChildren = ( family[index].children == 3    ) || ( family[index].children == -1 );
    bool rightCats =     ( family[index].cats == 7        ) || ( family[index].cats == -1     );
    bool rightSamoyeds = ( family[index].samoyeds == 2    ) || ( family[index].samoyeds == -1 );
    bool rightPomeranians=(family[index].pomeranians == 3 ) || ( family[index].pomeranians == -1     );
    bool rightAkitas =   ( family[index].akitas == 0      ) || ( family[index].akitas == -1     );
    bool rightVizslas =  ( family[index].vizslas == 0     ) || ( family[index].vizslas == -1     );
    bool rightGoldfish = ( family[index].goldfish == 5    ) || ( family[index].goldfish == -1     );
    bool rightTrees =    ( family[index].trees == 3       ) || ( family[index].trees == -1     );
    bool rightCars =     ( family[index].cars == 2        ) || ( family[index].cars == -1     );
    bool rightPerfumes = ( family[index].perfumes == 1    ) || ( family[index].perfumes == -1     );
    
    if( rightChildren && rightCats && rightSamoyeds && rightPomeranians && rightAkitas &&
        rightVizslas && rightGoldfish && rightTrees && rightCars && rightPerfumes )
        {
            std::cout << "***** " << index << "\n";
        }
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
        addSue(currentLine);
    }

    for(int i=0;i< family.size();i++)
    {
        checkForSue(i);
    }
    input.close();
    return 0;
}