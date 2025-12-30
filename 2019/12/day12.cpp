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

struct Moon
{
    int xPos;
    int yPos;
    int zPos;
    int xVel;
    int yVel;
    int zVel;
};

std::vector< Moon > moons, initialUniverse;
int stepNum=0;
long long totalEnergy;

void addMoon(std::string &s)
{
    Moon m;
    std::istringstream ss(s);
    std::string token;
    char delimiter = ',';
    // <x=-1, y=0, z=2>
    //std::cout << "token start: \n";
    while (std::getline(ss, token, delimiter))
    {
        char which = token[token.find_first_of('=') - 1];
        // clean up token for final > char:
        if( token.find_first_of('>') != std::string::npos )
        {
            token = token.substr(0, token.length()-1 );
        }
        token = token.substr(token.find_first_of('=')+1);
        int val = std::stoi(token);
        //std::cout << " -> " << which << " = " << val << "\n";
        switch(which)
        {
            case 'x':
                m.xPos = val;
            break;
            case 'y':
                m.yPos = val;
            break;
            case 'z':
                m.zPos = val;
            break;
            default:
            break;
        }
    }
    m.xVel =0;
    m.yVel =0;
    m.zVel =0;
    moons.push_back(m);
}

void printState(void)
{
    std::cout << "After " << stepNum << " steps:\n";
    for(int i=0;i<moons.size();i++)
    {
        std::cout << "pos=<x=" << moons[i].xPos << ", y=" << moons[i].yPos << ", z=" << moons[i].zPos << ">, vel=<x=";
        std::cout <<              moons[i].xVel << ", y=" << moons[i].yVel << ", z=" << moons[i].zVel << ">\n";
    }
}

void step(void)
{
    int size=moons.size();
    // this is where the magic happens.
    // to apply gravity, consider EACH PAIR of moons.
    // on each axis, the velocity changes by exactly +1 or -1 to pull the moons together.
    // if the positions on a given axis are the same, the velocity on that axis DOES NOT CHANGE.
    for(int i=0;i<size;i++)
    {
        for(int j=i+1;j<size;j++)
        {
            // compare i and j
            if( moons[i].xPos != moons[j].xPos )
            {
                moons[i].xVel += ( moons[i].xPos > moons[j].xPos ? -1 : 1);
                moons[j].xVel += ( moons[i].xPos < moons[j].xPos ? -1 : 1);
            }
            if( moons[i].yPos != moons[j].yPos )
            {
                moons[i].yVel += ( moons[i].yPos > moons[j].yPos ? -1 : 1);
                moons[j].yVel += ( moons[i].yPos < moons[j].yPos ? -1 : 1);
            }
            if( moons[i].zPos != moons[j].zPos )
            {
                moons[i].zVel += ( moons[i].zPos > moons[j].zPos ? -1 : 1);
                moons[j].zVel += ( moons[i].zPos < moons[j].zPos ? -1 : 1);
            }
        }
    }

    // apply velocity to moons!
    for(int i=0;i<size;i++)
    {
        moons[i].xPos += moons[i].xVel;
        moons[i].yPos += moons[i].yVel;
        moons[i].zPos += moons[i].zVel;
    }
    stepNum++;
}

bool compareMoons(long long timesteps, int *period)
{
    // check all four moons.  If any of them have all velocities == zero, set the appropriate period
    int i;
    int size = moons.size();
    bool repeatX=true, repeatY=true, repeatZ=true;
    for(i=0;i<size;i++)
    {
        if( moons[i].xVel != 0 ) repeatX=false;
        if( moons[i].yVel != 0 ) repeatY=false;
        if( moons[i].zVel != 0 ) repeatZ=false;
    }
    if(( repeatX ) && ( period[0] == 0)) period[0] = timesteps * 2; // * 2 because you need to go all the way around, not just 180 degrees.
    if(( repeatY ) && ( period[1] == 0)) period[1] = timesteps * 2;
    if(( repeatZ ) && ( period[2] == 0)) period[2] = timesteps * 2;

    return ((period[0] != 0) && (period[1] != 0) && (period[2] != 0) );  // if we have three times, stop.
}

long long gcd( long long m, long long n)
{
    return m ? gcd( n % m, m ) : n;
}

long long lcm(long long m, long long n)
{
    return m * n / gcd( m, n);
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        std::cout << currentLine << "\n";
        addMoon(currentLine);
    }
    initialUniverse = moons;

    int numSteps = 1000;
    for( int i=0;i< numSteps;i++)
    {
        printState;
        step();
    }
    printState();

    // calculate total energy in system
    totalEnergy = 0;
    // the total energy for a moon is its potential energy (abs of x,y,z sum)
    // times kinetic energy (abs of x,y,z sum of velocity)
    for(int i=0;i<moons.size();i++)
    {
        long long potential = abs(moons[i].xPos) + abs(moons[i].yPos) + abs(moons[i].zPos);
        long long kinetic   = abs(moons[i].xVel) + abs(moons[i].yVel) + abs(moons[i].zVel);
        totalEnergy += potential * kinetic; // add energy between moons.
    }

    std::cout << "Sum of total energy is " << totalEnergy << "\n";
    // pt2
    moons = initialUniverse; // reset moons

    // So, what we want to do is figure out the period of x,y,z
    // then figure out the least common multiple of each of those three periods.
    
    long long timesteps = 0;
    int period[3] = {0,0,0};

    do
    {
        ++timesteps;
        step();
    } while ( !compareMoons(timesteps, period) );
    std::cout << "Finished! \nPeriods at finish: " << period[0] << " " << period[1] << " " << period[2] << "\n";

    long long result = lcm(lcm(period[0], period[1]), period[2]);
    std::cout << "Final Result: " << result << "\n";
    //19858127636746 too low
    //139006893553458 too low!
    input.close();
    return 0;
}