#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::vector< int > grid;

int getCoord(int x, int y)
{
    return (x + 1000 * y);
}

void setToggle(int sx, int sy, int ex, int ey)
{
    for(int i=sx;i<=ex;i++)
    {
        for(int j=sy;j<=ey;j++)
        {
            int coord = getCoord(i,j);
            grid[coord] = grid[coord] + 2;
        }
    }
}

void setOn(int sx, int sy, int ex, int ey)
{
    for(int i=sx;i<=ex;i++)
    {
        for(int j=sy;j<=ey;j++)
        {
            int coord = getCoord(i,j);
            grid[coord] = grid[coord]+1;
        }
    }
}

void setOff(int sx, int sy, int ex, int ey)
{
    for(int i=sx;i<=ex;i++)
    {
        for(int j=sy;j<=ey;j++)
        {
            int coord = getCoord(i,j);
            grid[coord] = std::max<int>(grid[coord] - 1, 0);
        }
    }
}

int main(void)
{
    grid.resize(1000000, 0); // 1000x1000

    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        std::string modLine = currentLine;
        // parse this line:
        char *tokens = strtok(&modLine[0], " ");
        bool isToggle = false;
        bool isOn = false;
        bool isOff = false;
        if( strcmp(tokens, "toggle") != 0 )
        {
            // get on/off
            tokens = strtok(nullptr, " ");
            if(strcmp(tokens, "on") == 0)
            {
                isOn = true;
            }
            else
            {
                isOff = true;
            }
        }
        else
        {
            isToggle = true;
        }
        // next up is the start coord
        int startX, startY, endX, endY;
        tokens = strtok(nullptr, ",");
        startX = atoi(tokens);
        tokens = strtok(nullptr, " throug");
        startY = atoi(tokens);
        tokens = strtok(nullptr, "throug ,");
        endX = atoi(tokens);
        tokens = strtok(nullptr, "\n");
        endY = atoi(tokens);

        std::cout << currentLine << " :: " << (isToggle ? "tog" : "") << (isOff ? "off" : "" ) << (isOn ? "on" : "");
        std::cout << " (" << startX << "," << startY << ")->(" << endX << "," << endY << ")\n";

        // set 'em
        if( isToggle ) setToggle(startX, startY, endX, endY);
        if( isOff    ) setOff   (startX, startY, endX, endY);
        if( isOn     ) setOn    (startX, startY, endX, endY);
    }

    int sum = 0;
    for(int i=0;i<grid.size();i++)
    {
        sum += grid[i];
    }
    std::cout << "sum is " << sum << "\n";
    input.close();
    return 0;
}