#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::vector< std::pair< long long, long long > > ranges;

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;
    long long sum=0;

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
                long long begin = std::atol(pointer);
                pointer = strtok(nullptr, "\n");
                long long end = std::atol(pointer);

                ranges.emplace_back(std::make_pair(begin,end));
            }
        }
        else
        {
            // sort ranges by first
            std::sort(ranges.begin(), ranges.end());
            // test
            for( int i=0;i< ranges.size();i++)
            {
                // count range
                if( ranges[i].first < 0 ) // dead range
                {
                    std::cout << "skipping " << i << "\n";
                    continue;
                }
                std::cout << "Processing Range [" << ranges[i].first << "-" << ranges[i].second << "] : adding " << 
                    ranges[i].second - ranges[i].first + 1 << " to sum\n";
                sum += ranges[i].second - ranges[i].first + 1;
                // remove range from all future ranges
                for(int j=i+1;j<ranges.size();j++)
                {
                    if( ranges[j].first < 0)
                    {
                        continue; //skip
                    }
                    // process range
                    // ex:  [1,100] is the i
                    //      [1,120] is the j
                    //       -> [101,120]
                    if( ranges[j].first <= ranges[i].second )
                    {
                        long long oldval = ranges[j].first;
                        std::cout << " " << j << " adjusting [" << ranges[j].first << "-" << ranges[j].second << "] to ";
                        ranges[j].first = ranges[i].second + 1;
                        if( ranges[j].first > ranges[j].second )
                        {
                            ranges[j].first = -1;
                        }
                        std::cout << "[" << ranges[j].first << "-" << ranges[j].second << "] (" << 
                            (ranges[j].first == -1 ? -1 : ranges[j].first - oldval) << ")\n";
                    }
                }
            }
            std::cout << " testing\n";
            break;
        }
    }

    input.close();
    std::cout << " Total sum: " << sum << "\n";
    return 0;
}