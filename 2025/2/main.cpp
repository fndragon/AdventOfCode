#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

void processValue(long long i, long long begin, long long end)
{
    std::stringstream ss1;
    ss1 << i;
    std::string numStr = ss1.str();
    // Only deal with even length numbers.
    size_t midpoint = numStr.length() / 2;
    std::string left = numStr.substr(0, midpoint);
    std::string right = numStr.substr(midpoint, numStr.length() - midpoint);
    if(( left == right ) )
    {
        sum += i;
    }
    //std::cout << "processing " << i << " splits to [" << left << "," << right << "]\n";
    return;
}

void processAltValue( std::string toProc )
{
    size_t midpoint = toProc.length() / 2;
    std::string left = toProc.substr(0, midpoint);
    std::string right = toProc.substr(midpoint, toProc.length() - midpoint);
    if( left == right )
    {
        sum += std::atoll(toProc.c_str());
    }
}

bool compare( std::string a, std::string b)
{
    auto Adash = a.find_first_of('-');
    auto Bdash = b.find_first_of('-');
    long long AVal = std::atoll( a.substr(0, Adash).c_str());
    long long BVal = std::atoll( b.substr(0, Bdash).c_str());
    return AVal < BVal; // Custom ascending order
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    //input.open("example.txt");
    std::string token;
    std::vector<std::string> tokens;

    // Extract tokens using getline with a comma delimiter
    while (std::getline(input, token, ','))
    {
        tokens.push_back(token);
    }
    std::string begin, end;
    // sort the tokens, check for overlap
    std::sort( tokens.begin(), tokens.end(), compare);

    for( auto n : tokens) 
    {
        auto dash = n.find_first_of('-');
        begin = n.substr(0, dash);
        end = n.substr( dash + 1, n.length() - dash - 1);
        long long beginNum = std::atoll(begin.c_str());
        long long endNum = std::atoll(end.c_str());
        for( long long i = beginNum; i <= endNum; i++)
        {
            //processValue( i, beginNum, endNum );
            processAltValue( std::to_string(i));
        }
    }

    std::cout << "\n";
    input.close();
    std::cout << "final sum = " << sum << "\n";
    return 0;
}