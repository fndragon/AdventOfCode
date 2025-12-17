#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

struct State
{
    long long group1Sum;
    long long group1QE;
    int group1Size;
    long long group2Sum;
    int group2Size;
    long long group3Sum;
    int group3Size;
    std::vector< int > packagesLeft; // packages left to assign
    bool operator==(const State& rhs) const 
    { 
        return ((group1Sum == rhs.group1Sum) &&
                (group1QE  == rhs.group1QE ) );
    }
};

std::vector< int > packages;
int maxSize = 0;
std::vector< State > success;

int sum( std::vector< int > &group )
{
    int groupSum = 0;
    for( int g : group )
    {
        groupSum += g;
    }
    return groupSum;
}

bool addToGroup( int indexToAdd, State state )
{
    // Add package to group
    if( state.group1Sum < maxSize )
    {
        state.group1Sum += state.packagesLeft[indexToAdd];
        state.group1Size++;
        state.group1QE *= state.packagesLeft[indexToAdd];
        ///
        if( state.group1Sum > maxSize ) return false;
        state.packagesLeft.erase(state.packagesLeft.begin() + indexToAdd);
    }
    else if( state.group2Sum < maxSize )
    {
        state.group2Sum += state.packagesLeft[indexToAdd];
        state.group2Size++;
        ///
        if( state.group2Sum > maxSize ) return false;
        state.packagesLeft.erase(state.packagesLeft.begin() + indexToAdd);
    }
    else
    {
        if( state.group1Size > state.group2Size ) return false; //invalid, group1 needs to have the fewest.
        // If we get here, A and B both are full.  We dump!
        for( auto i : state.packagesLeft )
        {
            state.group3Sum += i;
            state.group3Size++;
        }
        if( state.group1Size > state.group3Size ) return false; // also invalid.
        if( success.empty() || std::find(success.begin(), success.end(), state) == success.end())
        {
            std::cout << " - Success with size " << state.group1Size << " and QE " << state.group1QE << "\n";
            success.push_back( state );
        }
        return true;
    }

    // else loop further
    for( int i=state.packagesLeft.size()-1; i >=0; i--)
    {
        if( addToGroup( i, state) )
        {
            return false; // avoids dups
        }
    }
    return false;
}

class combination
{
    bool cont { true };
    std::vector <bool> v;

public:
    combination (size_t n, size_t r) : v (r, true)
    {
        v.resize (n, false);
    }

    bool next (std::vector <size_t> & res)
    {
        if (!cont) return false;
        for (size_t i { 0 }, j { 0 }; i < v.size(); ++i)
        if (v [i])
        {
            res [j++] = i;
        }
        cont = std::prev_permutation (std::begin (v), std::end (v));
        return true;
    }
};


int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    std::vector< int > size;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        packages.push_back( atoi(currentLine.c_str()));
    }

    // Sum packages
    int packagesum=0;
    for( int p : packages )
    {
        packagesum += p;
    }
    maxSize = packagesum / 3;
    std::cout << "Sum is " << packagesum << " and the maximum size for all 3 bins is " << maxSize << "\n";

    // Make a counting list from 0..size to add
    std::vector< int > packageIndex;
    for(int i=0;i<packages.size();i++)
    {
        packageIndex.emplace_back(i);
    }
// attempt to grab the smallest size of packages that matches the size:
    unsigned long long smallestQE = std::numeric_limits<unsigned long long>::max();
    
    size_t min = 1;
    // Get the minimum package size by adding from the back of packages
    while (std::accumulate(packages.rbegin(), packages.rbegin() + ++min, 0) <= maxSize);
    
    uint64_t lowestQE = std::numeric_limits< uint64_t >::max();
    std::vector <size_t> ind (min);
    for (size_t r=min ; true; ind.resize (++r)) 
    {
        combination comb { packages.size(), r };
        while (comb.next (ind)) 
        {
            int sum = 0; 
            uint64_t QE =1;
            for (int index : ind)
            {
                sum += packages [index];
                QE *= packages [index];
            }
            if (sum == maxSize)
            {
                if( QE < lowestQE )
                {
                    std::cout << " Size " << r << " gives " << QE << " answer\n";
                    lowestQE = QE;
                }
            }
        }
        if( lowestQE < std::numeric_limits<uint64_t>::max() ) break;
    }
    input.close();
    return 0;
}