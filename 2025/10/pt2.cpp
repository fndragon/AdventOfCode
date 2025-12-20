#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>

struct Machine
{
    std::string lights; // . = off, # = on
    std::vector< std::vector < short > > buttons;
    std::vector< short > joltages;
};
std::vector< Machine > machines;

struct State
{
    int buttonPresses; // Times each button has been pressed
    std::vector< short > currentJoltage; // Voltage at each output
};

// result storage for the current solve:
std::vector< int > lowestButtonPresses; // one per thread

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

void pressButton( int tid, size_t button, State &s)
{
    // press button first, see if we fail
    s.buttonPresses++;

    // Check for too many button presses
    if( s.buttonPresses > lowestButtonPresses[tid] ) 
    {   
        // unroll 
        s.buttonPresses--;
        return;
    }

    for( int i : machines[tid].buttons[button] )
    {
        s.currentJoltage[i]++;
        if( s.currentJoltage[i] > machines[tid].joltages[i] )
        {
            // unroll so far
            s.buttonPresses--;
            for( int j : machines[tid].buttons[button] )
            {
                s.currentJoltage[j]--;
                if( j == i )
                {
                    return;
                }
            }
        }
    }

    // check for win
    bool win=true;
    for(int i=0;i< machines[tid].joltages.size();i++)
    {
        if( s.currentJoltage[i] != machines[tid].joltages[i] )
        {
            win = false;
            break;
        }
    }
    if( win )
    {
        // if we get here we have at most the same # of button presses as results.
        if( s.buttonPresses < lowestButtonPresses[tid] )
        {
            lowestButtonPresses[tid] = s.buttonPresses;
            std::cout << tid << ": new low: " << s.buttonPresses << " " << std::flush;
            //unroll
            s.buttonPresses--;
            for( int j : machines[tid].buttons[button] )
            {
                s.currentJoltage[j]--;
            }
            return;
        }
    }

    // loop over all possibilities
    for( int i=0;i< machines[tid].buttons.size();i++)
    {
        pressButton( tid, i, s);
    }

    s.buttonPresses--;
    for( int j : machines[tid].buttons[button] )
    {
        s.currentJoltage[j]--;
    }
    return;
}

void solveJoltage( int tid )
{
    Machine &m=machines[tid];
    State s;
    s.buttonPresses = 0;
    s.currentJoltage.resize(m.joltages.size(),0);
    lowestButtonPresses[tid] = std::accumulate(m.joltages.begin(), m.joltages.end(),0);
    // Recursively figure out what buttons to press
    for(int i=0;i<m.buttons.size();i++)
    {
        pressButton(tid, i, s);
    }
    std::cout << tid << ": " << lowestButtonPresses[tid] << "\n";
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
            std::vector< short > currentButton;
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
    std::vector< std::thread > threads;
    int maxThreads=7; // 7 max threads
    threads.resize(machines.size());
    lowestButtonPresses.resize(machines.size());
    for( int j=0;j<machines.size();j+= maxThreads)
    {
        for( int i=0;i<maxThreads;i++)
        {
            if( j+i < machines.size() )
                threads[i] = std::thread(solveJoltage, j+i );
        }
        for( int i=0;i<maxThreads;i++)
        {
            if( j+i < machines.size() )
                threads[i].join();
        }
    }
    
    totalButtonPresses = std::accumulate( lowestButtonPresses.begin(), lowestButtonPresses.end(), 0);
    std::cout << " Total Button Presses = " << totalButtonPresses << "\n";
    input.close();
    return 0;
}