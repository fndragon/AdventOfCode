#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>
#include <vector>

std::vector< int > intcode, restoreState;

void runPC(void)
{
    int PC = 0;
    bool running = true;
    while( running )
    {
        std::vector< bool > modes;
        // reset modes on each run
        modes.clear();
        modes.resize(4, false); // modes 0 doesn't get set
        // handle and set modes properly
        int currentOP = intcode[PC];
        if( intcode[PC] > 9 )
        {
            std::stringstream ss;
            ss << intcode[PC];
            std::string number = ss.str();
            int j = 1; // 0 doens't get set
            for(int i=number.size()-1-2; i >=0;i--) // start at position 0
            {
                if( number[i] == '1') modes[j] = true;
                ++j;
            }
            // now set the value at intcode[PC] to be the 2 digit value
            number = number.substr(number.size() - 2, 2);
            currentOP = std::stoi(number);
        }
        switch(currentOP)
        {
            case 1: // add
                std::cout << " 1] (" << intcode[PC+1] << " + " << intcode[PC+2] << ") => [" << intcode[PC+3] << "]\n";
                if(intcode[PC+3] > intcode.size() ) std::cout << "PANIK1\n";
                (modes[3] ? intcode[PC+3] : intcode[intcode[PC+3]]) = (modes[2] ? intcode[PC+2] : intcode[intcode[PC+2]]) + (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]);
                PC += 4;
                break;
            case 2: // mul
                std::cout << " 2] (" << intcode[PC+1] << " * " << intcode[PC+2] << ") => [" << intcode[PC+3] << "]\n";
                if(intcode[PC+3] > intcode.size() ) std::cout << "PANIK2\n";
                (modes[3] ? intcode[PC+3] : intcode[intcode[PC+3]]) = (modes[2] ? intcode[PC+2] : intcode[intcode[PC+2]]) * (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]);
                PC += 4;
                break;
            case 3: // Take a single integer as "input"
                {
                    if(intcode[PC+1] > intcode.size() ) std::cout << "PANIK3\n";
                    // do something
                    std::string textInput;
                    std::cout << "Requesting Input: ";
                    std::cin >> textInput;
                    int value = std::stoi(textInput);
                    (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]) = value;
                    PC += 2;
                }
                break;
            case 4: // OUTPUT the value to output
                if(intcode[PC+1] > intcode.size() ) std::cout << "PANIK4\n";
                std::cout << "Output: ";
                std::cout << (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]) << '\n';
                PC += 2;
                break;
            case 5: // Jump-if-true if(R1 != 0), PC = R2
                std::cout << " 5] JIT " << intcode[PC+1] << " ==> " << intcode[PC+2] << "\n";
                if( (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]) != 0 )
                {
                    PC = (modes[2] ? intcode[PC+2] : intcode[intcode[PC+2]]);
                }
                else
                {
                    PC +=3;
                }
                break;
            case 6: // Jump-if-false if(R1 == 0), PC = R2
                std::cout << " 6] JIF " << intcode[PC+1] << " ==> " << intcode[PC+2] << "\n";
                if( (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]) == 0 )
                {
                    PC = (modes[2] ? intcode[PC+2] : intcode[intcode[PC+2]]);
                }
                else
                {
                    PC +=3;
                }
                break;
            case 7: // < op
                std::cout << " 7] LT \n";
                if( (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]) < 
                    (modes[2] ? intcode[PC+2] : intcode[intcode[PC+2]]))
                {
                    (modes[3] ? intcode[PC+3] : intcode[intcode[PC+3]]) = 1;
                }
                else
                {
                    (modes[3] ? intcode[PC+3] : intcode[intcode[PC+3]]) = 0;
                }
                PC += 4;
                break;
            case 8: // = op
                std::cout << " 7] EQ \n";
                if( (modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]]) == 
                    (modes[2] ? intcode[PC+2] : intcode[intcode[PC+2]]))
                {
                    (modes[3] ? intcode[PC+3] : intcode[intcode[PC+3]]) = 1;
                }
                else
                {
                    (modes[3] ? intcode[PC+3] : intcode[intcode[PC+3]]) = 0;
                }
                PC += 4;
                break;
            case 99:
                running=false;
                break;
            default:
                std::cout << "Unknown operand\n";
                PC++;
                break;
        }
    }
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

    // run
    runPC();
    input.close();
    return 0;
}