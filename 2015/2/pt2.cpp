#include <iostream>
#include <fstream>
#include <sstream>

long long sum = 0;

struct dims
{
    int l;
    int w;
    int h;
};

dims parse(std::string const &line)
{
    dims output;
    std::string localLine = line;
    char *tokens = strtok(&localLine[0], "x");
    output.l = std::atoi(tokens);
    tokens = strtok(nullptr, "x");
    output.w = std::atoi(tokens);
    tokens = strtok(nullptr, "x");
    output.h = std::atoi(tokens);

    //std::cout << "line: " << line << " : " << output.l << " , " << output.w << " , " << output.h << "\n";
    return output;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        dims d = parse(currentLine);

        // calc
        //one side is longest
        // ribbon length = smallest length + smallest length + middle length + middle length + l * w * h
        int &shortest = ( d.h > d.l ? ( d.l > d.w ? d.w : d.l ) : ( d.h > d.w ? d.w : d.h ) );
        int *middle = nullptr;
        if( d.h == shortest )
        {
            middle = ( d.l > d.w ) ? &d.w : &d.l;
        }
        else
        if( d.w == shortest )
        {
            middle = (d.h > d.l) ? &d.l : &d.h;
        }
        else
        {
            middle = (d.h > d.w) ? &d.w : &d.h;
        }
        long ribbon = shortest + shortest + *middle + *middle + d.h * d.l * d.w;
        std::cout << currentLine << " : shortest: " << shortest << " mid: " << *middle << " and total ribbon " << ribbon << "\n";
        sum += ribbon;
    }//3732796 wrong 3737498 right


    input.close();
    std::cout << " sum : " << sum << "\n";
    return 0;
}