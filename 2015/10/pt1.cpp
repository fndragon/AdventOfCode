#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::string permute(std::string line)
{
    std::string output;
    // walk the string and convert chars
    for(int i=0;i<line.length();i++)
    {
        char c = line[i];
        int matchIndex = i+1;
        while( matchIndex < line.length() )
        {
            char m = line[matchIndex];
            if( m != c )
            {
                matchIndex--;
                break;
            }
            matchIndex++;
        }
        if( matchIndex >= line.length() ) --matchIndex;
        int numberOfCopies = matchIndex - i + 1;
        output.push_back( '0' + numberOfCopies );
        output.push_back( c );
        i = matchIndex;
    }
    return output;
}

int main(void)
{
    std::string currentLine = "1113222113";
    for( int i = 0; i < 50;i++)
    {
        std::string result = permute(currentLine);

        std::cout << currentLine << " >> " << result << "\n";
        currentLine = result;
    }
    std::cout << currentLine.length() << "\n";
    return 0;
}