#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::string convert(std::string line)
{
    std::string output;
    // walk the string and convert chars
    for(int i=0;i<line.length();i++)
    {
        if(line[i] == '\\')
        {
            // check next char:
            if( line[i+1] == '\"')
            {
                output.push_back('\"');
                i++;
            }
            else if( line[i+1] == '\\')
            {
                output.push_back('\\');
                i++;
            }
            else if( line[i+1] == 'x')
            {
                // get id, convert to char
                std::string ascii = line.substr(i+2,2);
                output.push_back((char) std::atoi(ascii.c_str()));
                i+=3;
            }
        }
        else
        {
            if( line[i] != '\"')
            {
                output.push_back( line[i]);
            }
        }

    }
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
        sum+= currentLine.size() - converted.size();
    }
    std::cout << "Sum: " << sum << "\n";
    input.close();
    return 0;
}