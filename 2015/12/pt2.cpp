#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <nlohmann/json.hpp>

long long sum=0;
std::vector< long long > numbers;

void iterate( nlohmann::json::iterator it )
{
    if( it->is_number() )
    {
        numbers.push_back( *it );
    }
    else if( it->is_array())
    {
        for (nlohmann::json::iterator it2 = it->begin(); it2 != it->end(); ++it2)
        {
            iterate( it2 );
        }
    }
    else if( it->is_object())
    {
        if( it->contains("red"))
        {
            std::cout << "red detected in object.\n";
            return;
        }
        // do a quick inner loop here b/c we read in alphabetically:
        bool ok = true;
        for( nlohmann::json::iterator it2 = it->begin(); it2 != it->end(); ++it2)
        {
            if( it2->contains("red"))
            {
                ok = false;
                break;
            }
            if( "red" == *it2 )
            {
                ok = false;
                break;
            }
        }

        if( ok )
        {
            for (nlohmann::json::iterator it2 = it->begin(); it2 != it->end(); ++it2)
            {
                iterate( it2 );
            }
        }
    }
std::cout << *it << '\n';
}

int main(void)
{
    // read a JSON file
    std::ifstream f("input.txt");
    nlohmann::json data = nlohmann::json::parse(f);

    // Recursively get all arrays, objects, numbers, and strings
    
    // iterate the array
    // check toplevel object for red first
    if( data.is_object())
    {
        bool ok = true;
        for( nlohmann::json::iterator it2 = data.begin(); it2 != data.end(); ++it2)
        {
            if( it2->contains("red"))
            {
                ok = false;
                break;
            }
            if( "red" == *it2 )
            {
                ok = false;
                break;
            }
        }
        if(!ok)
        {
            std::cout << "top level object has red\n";
            return 0;
        }
    }
    for (nlohmann::json::iterator it = data.begin(); it != data.end(); ++it)
    {
        iterate( it );
    }

    for( long long a : numbers )
    {
        sum += a;
    }
    std::cout << " sum : " << sum << "\n";
    return 0;
}