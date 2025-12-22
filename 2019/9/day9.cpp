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

std::vector< long long > intcode;
std::queue< long long > inputBuffer, outputBuffer;

long long &processMode(int PC, int mode, long long relBase)
{
    //(modes[1] ? intcode[PC+1] : intcode[intcode[PC+1]])
    if( mode == 1 ) // literal
    {
        return intcode[PC];
    }
    else if ( mode == 2 ) // relative
    {
        if( relBase + intcode[PC] > intcode.size() )
        {
            std::cout << "resizing at relative position!\n";
            intcode.resize(relBase + intcode[PC] + 1);
        }
        return intcode[relBase + intcode[PC]];
    }
    if( intcode.size() < intcode[PC] )
    {
        std::cout << "resizing during regular lookup\n";
        intcode.resize(intcode[PC]+1);
    }
    return intcode[intcode[PC]];
}

void runPC(void)
{
    int PC = 0;
    long long relativeBase = 0;
    while( true )
    {
        std::vector< int > paramMode;
        // reset modes on each run
        paramMode.clear();
        paramMode.resize(4, false); // modes 0 doesn't get set
        // handle and set modes properly
        long long currentOP = intcode[PC];
        if( intcode[PC] > 9 )
        {
            std::stringstream ss;
            ss << intcode[PC];
            std::string number = ss.str();
            int j = 1; // 0 doens't get set
            for(int i=number.size()-1-2; i >=0;i--) // start at position 0
            {
                if( number[i] == '1') paramMode[j] = 1;
                else if (number[i] == '2') paramMode[j] = 2;
                ++j;
            }
            // now set the value at intcode[PC] to be the 2 digit value
            number = number.substr(number.size() - 2, 2);
            currentOP = std::stoll(number);
        }
        switch(currentOP)
        {
            case 1: // add
                //std::cout << " 1] (" << intcode[PC+1] << " + " << intcode[PC+2] << ") => [" << intcode[PC+3] << "]\n";
                if(intcode[PC+3] > intcode.size() ) std::cout << "PANIK1\n";
                {
                    long long &destination = processMode(PC+3, paramMode[3], relativeBase);
                    long long &regA = processMode(PC+1, paramMode[1], relativeBase);
                    long long &regB = processMode(PC+2, paramMode[2], relativeBase);
                    destination = regA + regB;
                }
                PC += 4;
                break;
            case 2: // mul
                //std::cout << " 2] (" << intcode[PC+1] << " * " << intcode[PC+2] << ") => [" << intcode[PC+3] << "]\n";
                if(intcode[PC+3] > intcode.size() ) std::cout << "PANIK2\n";
                {
                    long long &destination = processMode(PC+3, paramMode[3], relativeBase);
                    long long &regA = processMode(PC+1, paramMode[1], relativeBase);
                    long long &regB = processMode(PC+2, paramMode[2], relativeBase);
                    destination = regA * regB;
                }
                PC += 4;
                break;
            case 3: // Take a single integer as "input"
                {
                    long long &destination = processMode(PC+1, paramMode[1], relativeBase);
                    //if(intcode[PC+1] > intcode.size() ) std::cout << "PANIK3\n";
                    std::string textInput;
                    int value;
                    if( inputBuffer.empty() )
                    {
                        std::cout << "Requesting Input: ";
                        std::cin >> textInput;
                        value = std::stoll(textInput);
                    }
                    else
                    {
                        value = inputBuffer.front();
                        inputBuffer.pop();
                    }
                    destination = value;
                    PC += 2;
                }
                break;
            case 4: // OUTPUT the value to output
                {
                    long long &value = processMode(PC+1, paramMode[1], relativeBase);
                    if(intcode[PC+1] > intcode.size() ) std::cout << "PANIK4\n";
                    std::cout << "Output: " << value << '\n';
                    outputBuffer.push(value);
                    PC += 2;
                }
                break;
            case 5: // Jump-if-true if(R1 != 0), PC = R2
                {
                    long long &compare = processMode(PC+1, paramMode[1], relativeBase);
                    //std::cout << " 5] JIT " << intcode[PC+1] << " ==> " << intcode[PC+2] << "\n";
                    if( compare != 0 )
                    {
                        long long &newPC = processMode(PC+2, paramMode[2], relativeBase);
                        PC = newPC;
                        if( intcode.size() <= PC )
                        {
                            intcode.resize(PC);
                            std::cout << "VERY BAD RESIZE\n";
                        }
                    }
                    else
                    {
                        PC +=3;
                    }
                }
                break;
            case 6: // Jump-if-false if(R1 == 0), PC = R2
                {
                    long long &compare = processMode(PC+1, paramMode[1], relativeBase);
                    //std::cout << " 6] JIF " << intcode[PC+1] << " ==> " << intcode[PC+2] << "\n";
                    if( compare == 0 )
                    {
                        long long &newPC = processMode(PC+2, paramMode[2], relativeBase);
                        PC = newPC;
                        if( intcode.size() <= PC )
                        {
                            intcode.resize(PC);
                            std::cout << "VERY BAD RESIZE2\n";
                        }
                    }
                    else
                    {
                        PC +=3;
                    }
                }
                break;
            case 7: // < op
                {
                    long long &destination = processMode(PC+3, paramMode[3], relativeBase);
                    long long &regA = processMode(PC+1, paramMode[1], relativeBase);
                    long long &regB = processMode(PC+2, paramMode[2], relativeBase);
                    //std::cout << " 7] LT \n";
                    if( regA < regB )
                    {
                        destination = 1;
                    }
                    else
                    {
                        destination = 0;
                    }
                }
                PC += 4;
                break;
            case 8: // = op
                {
                    long long &destination = processMode(PC+3, paramMode[3], relativeBase);
                    long long &regA = processMode(PC+1, paramMode[1], relativeBase);
                    long long &regB = processMode(PC+2, paramMode[2], relativeBase);
                    //std::cout << " 7] EQ \n";
                    if( regA == regB )
                    {
                        destination = 1;
                    }
                    else
                    {
                        destination = 0;
                    }
                }
                PC += 4;
                break;
            case 9: // relative base manip
                {
                    long long &value = processMode(PC+1, paramMode[1], relativeBase);
                    relativeBase += value;
                    PC += 2;
                }
                break;
            case 99:
                return;
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
            intcode.push_back(std::stoll(token));
        }
    }

    // run
    inputBuffer.push(2);
    intcode.resize( 50000);
    runPC();
    while( !outputBuffer.empty() )
    {
        std::cout << "OUT: " << outputBuffer.front() << "\n";
        outputBuffer.pop();
    }
    input.close();
    return 0;
}