#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

void processAltValue( std::string toProc )
{
    size_t midpoint = toProc.length() / 2;
    bool correct = false;
    //std::cout << "[" << toProc << "] Comparing:";
    for( size_t index = 1; index <= midpoint; index++)
    {
        size_t offset = 0;
        // check that the string is repeated
        std::string left = toProc.substr(0, index);
        bool this_check = true;
        if(toProc.length() % index)
        {
            continue;
        }
        for( int i = 1 ; i < (toProc.length() / index); i++)
        {
            // get right, compare
            std::string right = toProc.substr( index * i, index);
            //std::cout << left << " " << right;
            if( left != right )
            {
                this_check = false;
                break;
            }
        }
        // If we got through without having a failure check, we are correct.
        if( !correct) // latch on
        {
            correct = this_check;
        }
    }

    //std::cout << (correct ? " ok\n" : " XX\n" );
    if(correct)
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
    //input.open("unit.txt");
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
            processAltValue( std::to_string(i));
        }
    }

    std::cout << "\n";
    input.close();
    std::cout << "final sum = " << sum << "\n";
    return 0;
}