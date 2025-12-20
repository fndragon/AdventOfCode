#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>

struct Machine
{
    std::string lights; // . = off, # = on
    std::vector< std::vector < int > > buttons;
    std::vector< int > joltages;
};
std::vector< Machine > machines;

int solve(Machine m)
{
    // press each button a maximum of once.  This is because they xor and cancel out.
    std::vector< int > permutation;
    for( int i=1;i<= m.buttons.size();i++) // number of buttons to press (+1)
    {
        permutation.clear();
        permutation.resize(m.buttons.size(),0);
        // set up the perm vector
        for( int j=0;j<i;j++)
        {
            // set the first j elements to 1 to indicate how many buttons to press [ 1, 0, 0 ] means press 1 button out of 3
            permutation[j] = 1;
        }
        std::sort( permutation.begin(), permutation.end()); // must be sorted for permutation to work.
        do 
        {
            // press all buttons marked with 1 and compare to target
            std::string lights;
            lights.resize(m.lights.size(), '.');
            for(int j=0;j<permutation.size();j++)
            {
                if( permutation[j] == 1)
                {
                    // activate all lights for this button
                    for(auto b : m.buttons[j])
                    {
                        if( lights[b] == '.' )
                        {
                            lights[b] = '#';
                        }
                        else
                        {
                            lights[b] = '.';
                        }
                    }
                }
            }

            if( lights == m.lights )
            {
                return i;
            }
        } while (std::next_permutation(permutation.begin(), permutation.end()));
    }
    std::cout << "Oops\n";
    return -1;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        //[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
        Machine m;
        std::cout << "line : " << currentLine << "\n";
        // substrings
        size_t lightBegin = currentLine.find_first_of('[');
        size_t lightEnd = currentLine.find_first_of(']');
        m.lights = currentLine.substr(lightBegin + 1, lightEnd - lightBegin - 1);

        // joltages
        size_t joltBegin = currentLine.find_first_of('{');
        size_t joltEnd   = currentLine.find_first_of('}');
        std::string joltages = currentLine.substr(joltBegin + 1, joltEnd - joltBegin - 1);
        char *c = strtok(&joltages[0], ",");
        while( c != nullptr )
        {
            m.joltages.push_back( atoi(c));
            c = strtok(nullptr, ",");
        }

        std::string buttonStr = currentLine.substr( lightEnd + 2, joltBegin - lightEnd - 3);

        size_t start = 0;
        while( true )
        {        
            std::vector< int > currentButton;
            size_t buttonStart = buttonStr.find_first_of('(', start);
            size_t buttonEnd = buttonStr.find_first_of(')', start);
            if( buttonStart == std::string::npos )
            {
                break;
            }
            start = buttonEnd + 1;
            std::string buttonSubStr = buttonStr.substr(buttonStart + 1, buttonEnd - buttonStart - 1);
            c = strtok(&buttonSubStr[0], " ," );
            while( c != nullptr )
            {
                currentButton.push_back(atoi(c));
                c = strtok(nullptr, " ,");
            }
            m.buttons.push_back(currentButton);
        }
        machines.push_back(m);
    }

    // process machines
    int totalButtonPresses = 0;
    for( Machine m : machines )
    {
        int presses = solve(m);
        totalButtonPresses += presses;
    }
    std::cout << " Total Button Presses = " << totalButtonPresses << "\n";
    input.close();
    return 0;
}