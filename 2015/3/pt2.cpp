#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

long long sum = 0;

std::map< std::pair< int, int >, bool > locationsTravelled;

void deliver(std::pair< int, int > loc)
{
    // Check if location is set;
    if( locationsTravelled.find( loc ) == locationsTravelled.end() )
    {
        // new location
        locationsTravelled[loc] = true;
        sum++;
    }
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        // Infinite santa project:
        locationsTravelled.clear();
        std::pair< int, int > santa = { 0,0};
        std::pair< int, int > roboSanta = {0,0};
        bool santaTurn = true;
        deliver(santa); // always delivers at home
        for(int i=0;i<currentLine.length();i++)
        {
            char arrow = currentLine[i];
            switch( arrow )
            {
                default:
                    std::cout << "Unhandled\n";
                    break;
                case '^':
                    if( santaTurn ) { santa.second++; deliver(santa); }
                    else { roboSanta.second++; deliver(roboSanta); }
                    break;
                case 'v':
                    if( santaTurn ) { santa.second--; deliver(santa); }
                    else { roboSanta.second--; deliver(roboSanta); }
                    break;
                case '<':
                    if( santaTurn ) { santa.first--; deliver(santa); }
                    else { roboSanta.first--; deliver(roboSanta); }
                    break;
                case '>':
                    if( santaTurn ) { santa.first++; deliver(santa); }
                    else { roboSanta.first++; deliver(roboSanta); }
                    break;
            }
            santaTurn = !santaTurn;
        }
        // summarize outputs
        std::cout << "sum: " << sum << "\n";
        sum=0;
    }


    input.close();
    return 0;
}