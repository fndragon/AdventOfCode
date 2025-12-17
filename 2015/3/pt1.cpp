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
                    santa.second++;
                    deliver(santa);
                    break;
                case 'v':
                    santa.second--;
                    deliver(santa);
                    break;
                case '<':
                    santa.first--;
                    deliver(santa);
                    break;
                case '>':
                    santa.first++;
                    deliver(santa);
                    break;
            }
        }
        // summarize outputs
        std::cout << "sum: " << sum << "\n";
        sum=0;
    }


    input.close();
    return 0;
}