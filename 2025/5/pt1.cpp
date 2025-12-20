#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::vector< std::pair< long, long > > ranges;

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;
    int sum=0;

    bool setup = true;
    while( std::getline(input, currentLine) )
    {
        if( setup )
        {
            // add to ranges
            if( currentLine.empty())
            {
                setup = false;
            }
            else
            {
                std::string tempLine = currentLine;
                char *pointer = strtok((char *) tempLine.c_str(), "-");
                long begin = std::atol(pointer);
                pointer = strtok(nullptr, "\n");
                long end = std::atol(pointer);

                ranges.emplace_back(std::make_pair(begin,end));
            }
        }
        else
        {
            long test = atol(currentLine.c_str());
            // test against ranges
            bool isfresh=false;
            for( auto range : ranges )
            {
                if( test >= range.first && test <= range.second )
                {
                    isfresh = true;
                    sum++;
                    break;
                }
            }
            std::cout << test << (isfresh ? " is fresh\n" : " is not fresh\n");
        }
        std::cout << "line: " << currentLine << "\n";
    }

    input.close();
    std::cout << " Total sum: " << sum << "\n";
    return 0;
}