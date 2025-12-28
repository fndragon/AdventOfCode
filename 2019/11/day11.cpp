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

// paintint specific i/o:
struct Panel
{
    int x;
    int y;
    int color; // 0 is black, 1 is white
};

std::vector< Panel > grid;
int currentX = 0;
int currentY = 0;
int currentDirection = 0; // 0 = north, 1 = east, 2 = south, 3 = west
bool readColor = true; // state of reading color vs. reading direction change.

Panel *findPanel(int x, int y)
{
    for(int i=0;i<grid.size();i++)
    {
        if( grid[i].x == x && grid[i].y == y)
        {
            return &grid[i];
        }
    }
    return nullptr;
}

void printGrid(std::vector< Panel > &g)
{
    int maxX=-990,maxY=-990,minX=990,minY=990;
    for(int i=0;i<grid.size();i++)
    {
        maxX = std::max(grid[i].x,maxX);
        maxY = std::max(grid[i].y,maxY);
        minX = std::min(grid[i].x,minX);
        minY = std::min(grid[i].y,minY);
    }
    // loop over y,x to print out the grid, line by line:
    for(int y=maxY;y>=minY;y--)
    {
        for(int x=minX;x<=maxX;x++)
        {
            if(nullptr == findPanel(x,y)) std::cout << "."; // default is black
            else
            {
                std::cout << ((findPanel(x,y)->color == 0 ) ? "." : "X" );
            }
        }
        std::cout << "\n";
    }
}

// never writes to the grid
long long getInput(void)
{
    std::string textInput;
    int value;
    if( inputBuffer.empty() )
    {
        //std::cout << "Requesting Input: ";
        //std::cin >> textInput;
        //value = std::stoll(textInput);

        //The program uses input instructions to access the robot's camera: 
        //provide 0 if the robot is over a black panel or 1 if the robot is over a white panel.
        if( nullptr == findPanel(currentX, currentY) )
        {
            // by default all panels are black
            return 0;
        }
        else
        {
            return findPanel(currentX,currentY)->color;
        }
    }
    else
    {
        value = inputBuffer.front();
        inputBuffer.pop();
    }
    return value;
}

// writes to the grid depending on mode.
void writeOutput(long long &value)
{
    //std::cout << "Output: " << value << '\n';
    outputBuffer.push(value);

    if( readColor )
    {
        //First, it will output a value indicating the color to paint the panel the robot is over: 0 means to paint the panel black, and 1 means to paint the panel white.
        if( nullptr == findPanel(currentX,currentY))
        {
            Panel p;
            p.x = currentX;
            p.y = currentY;
            p.color = value;
            grid.push_back(p);
        }
        else
        {
            findPanel(currentX,currentY)->color = value;
        }
        readColor = false;
    }
    else
    {
        // Second, it will output a value indicating the direction the robot should turn: 0 means it should turn left 90 degrees, and 1 means it should turn right 90 degrees.
        if( value == 0 )
        {
            currentDirection--;
            if( currentDirection < 0 ) currentDirection = 3;
        }
        else
        {
            currentDirection++;
            if( currentDirection > 3 ) currentDirection = 0;
        }
        // After the robot turns, it should always move forward exactly one panel. The robot starts facing up.
        switch(currentDirection)
        {
            case 0:  // N
                currentY++;
                break;
            case 1: // 
                currentX++;
                break;
            case 2: // S
                currentY--;
                break;
            case 3: // W
                currentX--;
                break;
            default:
                std::cout << "Abort!\n";
                break;
        }
        readColor = true;
    }
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
                    destination = getInput();
                    PC += 2;
                }
                break;
            case 4: // OUTPUT the value to output
                {
                    long long &value = processMode(PC+1, paramMode[1], relativeBase);
                    if(intcode[PC+1] > intcode.size() ) std::cout << "PANIK4\n";
                    writeOutput(value);
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
    intcode.resize( 50000); // to start.
    // pt2:
    //it looks like the robot was expecting to start on a white panel, not a black one
    Panel p;
    p.x=0;
    p.y=0;
    p.color=1;
    grid.push_back(p);
    runPC();
    //while( !outputBuffer.empty() )
    //{
    //    std::cout << "OUT: " << outputBuffer.front() << "\n";
    //    outputBuffer.pop();
    //}

    std::cout << "Total panels touched " << grid.size() << "\n";

    // print the grid to the console:
    printGrid(grid);
    input.close();
    return 0;
}