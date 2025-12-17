#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::string convert(std::string line)
{
    std::string output;
    output.push_back('\"');

    // walk the string and convert chars
    for(int i=0;i<line.length();i++)
    {
        char c = line[i];
        switch( c )
        {
            default:
                output.push_back(c);
                break;
            case '\\':
                output.push_back('\\');
                output.push_back('\\');
                break;
            case '\"':
                output.push_back('\\');
                output.push_back('\"');
                break;
        }
    }
    output.push_back('\"');
    return output;
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
        std::string converted = convert(currentLine);

        std::cout << "line : " << currentLine << " >> " << converted << "\n";
        sum+= converted.size() - currentLine.size();
    }
    std::cout << "Sum: " << sum << "\n";
    input.close();
    return 0;
}