#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <nlohmann/json.hpp>

long long sum=0;
std::vector< long long > numbers;

void iterate( nlohmann::json::iterator begin, nlohmann::json::iterator end)
{
    for (nlohmann::json::iterator it = begin; it != end; ++it)
    {
        if( it->is_number() )
        {
            numbers.push_back( *it );
        }
        else if( it->is_array())
        {
            iterate( it->begin(), it->end());
        }
        else if( it->is_object())
        {
            iterate( it->begin(), it->end());
        }
    std::cout << *it << '\n';
    }
}

int main(void)
{
    // read a JSON file
    std::ifstream f("input.txt");
    nlohmann::json data = nlohmann::json::parse(f);

    // Recursively get all arrays, objects, numbers, and strings
    
    // iterate the array
    iterate( data.begin(), data.end());
    for( long long a : numbers )
    {
        sum += a;
    }
    std::cout << " sum : " << sum << "\n";
    return 0;
}