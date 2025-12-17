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

    std::cout << "line: " << line << " : " << output.l << " , " << output.w << " , " << output.h << "\n";
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
        //2*l*w + 2*w*h + 2*h*l + 1(smallest side)
        long side1 = d.l * d.w;
        long side2 = d.w * d.h;
        long side3 = d.h * d.l;
        long smallestSide = std::min< long >( std::min< long >(side1, side2), side3);
        long total = 2 * side1 + 2 * side2 + 2 * side3 + smallestSide;
        sum += total;
        std::cout << "line : " << currentLine << " has " << total << " square feet\n";
    }


    input.close();
    std::cout << " sum : " << sum << "\n";
    return 0;
}