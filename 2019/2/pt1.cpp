#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>

std::vector< int > intcode, restoreState;

void runPC(void)
{
    int PC = 0;
    bool running = true;
    while( running )
    {
        switch(intcode[PC])
        {
            case 1: // add
                if(intcode[PC+3] > intcode.size() ) std::cout << "PANIK\n";
                intcode[intcode[PC+3]] = intcode[intcode[PC+2]] + intcode[intcode[PC+1]];
                PC += 4;
                break;
            case 2: // mul
                if(intcode[PC+3] > intcode.size() ) std::cout << "PANIK\n";
                intcode[intcode[PC+3]] = intcode[intcode[PC+2]] * intcode[intcode[PC+1]];
                PC += 4;
                break;
            case 99:
                running=false;
                break;
            default:
                break;
        }
    }
}

void resetPC(void)
{

}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        std::cout << currentLine << "\n";
        char delimiter = ',';
        std::istringstream ss(currentLine);
        std::string token; // The std::string variable used to hold each token
        std::vector<std::string> tokens;

        // Use getline with a specific delimiter
        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
            intcode.push_back(std::stoi(token));
        }
    }
    // snapshot
    restoreState = intcode;

    // Once you have a working computer, the first step is to restore the gravity assist program (your puzzle input) to 
    // the "1202 program alarm" state it had just before the last computer caught fire. To do this, before running the program, 
    //replace position 1 with the value 12 and replace position 2 with the value 2. 
    intcode[1] = 12;
    intcode[2] = 2;

    // run
    runPC();
    std::cout << "Pt1: INTCODE Pos [0]: " << intcode[0] << "\n";
    intcode = restoreState;
    intcode[1] = 12;
    intcode[2] = 2;
    runPC();
    std::cout << "Pt2 example " << 100 * intcode[1] + intcode[2] << "\n";
    // pt2
    // Each of the two input values will be between 0 and 99, inclusive.
    for(int i=0;i<99;i++)
    {
        for(int j=0;j<99;j++)
        {
            intcode = restoreState;
            intcode[1] = i;
            intcode[2] = j;
            runPC();
            if( intcode[0] == 19690720)
            {
                std::cout << "PT2: Word: " << i << " Verb: " << j << "\n";
                std::cout << "PT2: 100*N*V: " << 100 * i + j << "\n";
                i=100;
                j=100;
            }
        }
    }
    //265500 too high
    input.close();
    return 0;
}