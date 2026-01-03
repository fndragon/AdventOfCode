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
#include <queue>

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

struct Location
{
    int x;
    int y;
    int type; // 0 = empty.  1 = wall.  2 = HATCH, -1 = unknown
    std::vector< int > directions; // 1=north, 2=south, 3=west, 4=east, starting at start point
    int distance; // orthog distance from start.
    int oxygen;
};

std::vector< Location > grid;
int minX=0;
int minY=0;
int maxX=0;
int maxY=0;

Location *getXY(int x, int y)
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

Location *getTarget(int depth)
{
    // look for locations in the grid that match this:
    // 1) l.depth == depth-1
    // 2) the location has an empty neighbor
    // 3) the location is not a wall
    //
    // In that case, create a Location entry for the target and set up its direction vector
    for(int i=0;i<grid.size();i++)
    {
        if( grid[i].type != 1 )
        {
            if( grid[i].distance == (depth-1) )
            {
                // check north
                if( getXY(grid[i].x, grid[i].y+1) == nullptr )
                {
                    Location l;
                    l.x = grid[i].x;
                    l.y = grid[i].y + 1;
                    l.distance = depth;
                    l.type = -1;
                    l.directions = grid[i].directions;
                    l.directions.push_back(1); // for north.
                    grid.push_back(l);
                    return &grid.back();
                }
                // check east
                if( getXY(grid[i].x+1, grid[i].y) == nullptr )
                {
                    Location l;
                    l.x = grid[i].x + 1;
                    l.y = grid[i].y;
                    l.distance = depth;
                    l.type = -1;
                    l.directions = grid[i].directions;
                    l.directions.push_back(4); // for east.
                    grid.push_back(l);
                    return &grid.back();
                }
                // check south
                if( getXY(grid[i].x, grid[i].y - 1) == nullptr )
                {
                    Location l;
                    l.x = grid[i].x;
                    l.y = grid[i].y - 1;
                    l.distance = depth;
                    l.type = -1;
                    l.directions = grid[i].directions;
                    l.directions.push_back(2); // for south.
                    grid.push_back(l);
                    return &grid.back();
                }
                // check west
                if( getXY(grid[i].x - 1, grid[i].y) == nullptr )
                {
                    Location l;
                    l.x = grid[i].x - 1;
                    l.y = grid[i].y;
                    l.distance = depth;
                    l.type = -1;
                    l.directions = grid[i].directions;
                    l.directions.push_back(3); // for east.
                    grid.push_back(l);
                    return &grid.back();
                }
            }
        }
    }
    return nullptr;
}

// input state stuff here:
// we can be in several states
// 0)  No current operation
// 1)  going to a test site
// 2)  testing a site
// 3)  returning from a test site
int currentInputState = 0;
std::vector< int > destinationDirections;
int currentIndex = 0;
int currentMinDepth = 1;
int targetX=0,targetY=0;

void printGrid(bool oxygenMode=false)
{
    for(int y=maxY;y>=minY;y--)
    {
        for(int x=minX;x<=maxX;x++)
        {
            Location *l = getXY(x,y);
            if(( 0 == x && 0 == y ))
            {
                std::cout << "+";
            }
            else if(( targetX == x ) && ( targetY == y )) 
            {
                std::cout << "?";
            }
            else
            {
                if(nullptr == l) std::cout << " ";
                else
                {
                    if(( l->type == 0 ) && !oxygenMode)
                    {
                        std::cout << l->distance % 10;
                    }
                    if(( l->type == 0 ) && oxygenMode)
                    {
                        if( l->oxygen == -1 ) std::cout << ".";
                        else std::cout << l->oxygen % 10;
                    }
                    if( l->type == 1 ) std::cout << "#";
                    if( l->type == 2 ) std::cout << "*";
                }
            }
        }
        std::cout << "\n";
    }
}

int invert(int direction)
{
    switch(direction)
    {
        case 1: // north
            return 2;
            break;
        case 2: // south
            return 1;
            break;
        case 3: // west
            return 4;
            break;
        case 4: // east
            return 3;
            break;
        default:
        break;
    }
    return 0;
}

void resetOxygen(void)
{
    for(int i=0;i<grid.size();i++)
    {
        grid[i].oxygen = -1;
    }
}

bool fillOxygen(int timestep)
{
    bool anythingNeededOxygen = false;
    for(int i=0;i<grid.size();i++)
    {
        if( grid[i].type == 1 ) continue; // This is a wall, skip.
        if( grid[i].oxygen >= 0 ) continue; // We already calculated this grid point's oxygen
        // for timestep n, check each grid's adjacent objects to see if they have been filled with oxygen at n-1:
        Location *neighbors[4] = { getXY(grid[i].x, grid[i].y+1), getXY(grid[i].x, grid[i].y-1),
                                   getXY(grid[i].x+1, grid[i].y), getXY(grid[i].x-1, grid[i].y) };
        for(int j=0;j<4;j++)
        {
            if( neighbors[j] == nullptr ) 
            {
                std::cout << "nope\n";
            }

            if(// ( nullptr != neighbors[j] ) &&  // we are at a valid neighbor position (should never be hit, tbh)
               (neighbors[j]->oxygen == timestep-1) &&  // If the neighbor's oxygen is n-1
               ( neighbors[j]->type != 1 ) ) // and the neighbor doesn't represent a wall.
            {
                anythingNeededOxygen=true;
                grid[i].oxygen = timestep;
                continue; // skip rest of checks for this grid object.
            }
        }
    }
    return anythingNeededOxygen;
}
int oxygenValveX=0;
int oxygenValveY=0;
// never writes to the grid
long long getInput(void)
{
    std::string textInput;
    int value = 0;
    if( inputBuffer.empty() )
    {
        if( currentInputState == 0 )
        {
            //std::cout << "\n getting new target\n";
            // figure out where to go:
            Location *l = getTarget(currentMinDepth);
            if( nullptr == l ) // need to increase depth search
            {
                currentMinDepth++;
                l = getTarget(currentMinDepth);
                if( nullptr == l )
                {
                    std::cout << "Evaluating oxygen release\n";
                    // There are no more map elements to explore.
                    // Figure out part2 (oxygen):
                    // You can't directly use the distance so instead we'll just tick time in minutes
                    // to fill the area with oxygen:
                    resetOxygen();
                    Location *valve = getXY(oxygenValveX,oxygenValveY);
                    valve->oxygen = 0;
                    printGrid(true);
                    int timesteps = 1;
                    while(true)
                    {
                        if( !fillOxygen(timesteps) )
                        {
                            break;
                        }
                        std::cout << "TS: " << timesteps <<"\n";
                        printGrid(true);
                        timesteps++;
                    }
                    std::cout << "Timesteps needed to run was " << timesteps - 1<< "\n";
                    exit(0);
                }
            }
            // Now set state up for this target:
            destinationDirections = l->directions;
            currentIndex=0;
            currentInputState = 1;
            //std::cout << "  target: " << l->x << ", " << l->y << "\n";
            targetX = l->x;
            targetY = l->y;
            minX = std::min(minX, l->x);
            minY = std::min(minY, l->y);
            maxX = std::max(maxX, l->x);
            maxY = std::max(maxY, l->y);
        }

        if( currentInputState == 1 )
        {
            // going to a target
            //std::cout << " moving\n";
            value = destinationDirections[currentIndex];
            currentIndex++;
            if( currentIndex == destinationDirections.size() )
            {
                currentInputState = 2; // testing!  - will rely on output to tell us where to go
            }
        }
        else if( currentInputState == 2 )
        {
            std::cout << "*** uh oh ***\n";
            exit(1);
        }
        else if( currentInputState == 3 )
        {
            //std::cout << " returning\n";
            value = invert( destinationDirections[currentIndex] );
            currentIndex--;
            if( currentIndex < 0 )
            {
                currentInputState = 0;
            }
        }
    }
    else
    {
        value = inputBuffer.front();
        inputBuffer.pop();
    }
    //std::cout << "Input provided " << value << "\n";
    return value;
}

// writes to the grid depending on mode.
void writeOutput(long long &value)
{
    //std::cout << "Output Received: " << value << '\n';
    if( currentInputState != 2 )
    {
        if( value == 0 ) 
        {
            std::cout << "ERRORATOUTPUT\n";
        }
        return;  //ignoring the output
    }
    currentInputState = 3;

    // (return to start)
    Location *l = getXY(targetX, targetY);
    if( value == 1 ) //success
    {
        l->type = 0;
    }
    if( value == 0 )
    {
        l->type = 1;  // We bounced! Remove the last element from the direction vector
        destinationDirections.pop_back();
        if( destinationDirections.empty() )
        {
            currentInputState = 0; // if we bounce from the start, reset.
        }
    }
    if( value == 2 )
    {
        std::cout << "FOUND target.\n";
        std::cout << "Distance is " << l->distance << "\n";
        l->type = 2;
        l->oxygen = 0; // timestep zero
        oxygenValveX = l->x;
        oxygenValveY = l->y;
        printGrid();
        std::cout << "finishing map\n";
    }
    // Start at the back of destinationDirections
    currentIndex = destinationDirections.size() - 1;
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
    Location l;
    l.x=0;
    l.y=0;
    l.type=0;
    l.distance=0;
    l.directions.clear();
    grid.push_back(l); // push back 0,0 origin

    resetOxygen();
    runPC();
    
    input.close();
    return 0;
}