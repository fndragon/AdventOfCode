#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

struct Reindeer
{
    std::string name;
    long speed; //km/s
    long duration; //seconds
    long cooldown; // seconds
};

struct State
{
    long whenLastBoostWas;
    long distance;
};

std::vector< Reindeer > deer;

void addReindeer( std::string line )
{
    Reindeer myDeer;
    std::string localLine = line;
    char *c = strtok(&localLine[0], " ");
    myDeer.name = std::string(c);
    c = strtok(nullptr, " "); // can
    c = strtok(nullptr, " "); // fly
    c = strtok(nullptr, " "); // 
    myDeer.speed = atol(std::string(c).c_str());
    c = strtok(nullptr, " "); // km/s
    c = strtok(nullptr, " "); // for
    c = strtok(nullptr, " "); // 
    myDeer.duration = atol(std::string(c).c_str());
    c = strtok(nullptr, " "); // seconds
    c = strtok(nullptr, " "); // but
    c = strtok(nullptr, " "); // but
    c = strtok(nullptr, " "); // but
    c = strtok(nullptr, " "); // but
    c = strtok(nullptr, " "); // but
    c = strtok(nullptr, " "); // 
    myDeer.cooldown = atol(std::string(c).c_str());
    deer.push_back(myDeer);
}

void timestep( long timestep, std::vector< State > &state )
{
    for(int i=0;i<deer.size();i++)
    {
        // Start flying
        if(( timestep - state[i].whenLastBoostWas ) >= ( deer[i].cooldown + deer[i].duration) )
        {
            // start flying NOW
            state[i].whenLastBoostWas = timestep;
        }
        if(( timestep - state[i].whenLastBoostWas ) < deer[i].duration )
        {
            // actively flying
            state[i].distance += deer[i].speed;
        }
    }
}

void leaderboard( long timestep, std::vector< State > &state)
{
    std::cout << " --- Race Status at time " << timestep << " seconds\n";
    for(int i=0;i<deer.size();i++)
    {
        std::cout << deer[i].name << " is at distance " << state[i].distance << "\n";
    }
    std::cout << "\n";
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    long sum=0;
    bool setup = true;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        addReindeer( currentLine);
    }
    std::vector< State > gameState;
    gameState.resize(deer.size());
    for(int i=0;i<deer.size();i++)
    {
        // init gamestate
        gameState[i].whenLastBoostWas = 0;
        gameState[i].distance = 0;
    }

    int numTS = 2503;
    int interval = 100;
    int i;
    for(i=0;i<numTS;i++)
    {
        timestep(i, gameState);
        if(( i % interval ) == 0)
        {
            leaderboard(i+1, gameState );
        }
    }

    leaderboard(i+1, gameState );

    input.close();
    return 0;
}