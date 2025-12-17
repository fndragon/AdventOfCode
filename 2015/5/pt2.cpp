#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

long long nice = 0;

bool check1(std::string &str)
{
    //It contains a pair of any two letters that appears at least twice in the string without overlapping, 
    //like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps).
    for( int i=0;i<(str.length()-3);i++)
    {
        std::string sub = str.substr(i,2);
        for(int j=(i+2);j<(str.length()-1);j++)
        {
            std::string compare = str.substr(j,2);
            if( sub == compare)
            {
                return true;
            }
        }
    }
    return false;
}

bool check2(std::string &str)
{
    //It contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa.
    for(int i=0;i<( str.length() - 2) ;i++)
    {
        if( str[i] == str[i+2] )
        {
            return true;
        }
    }
    return false;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        std::cout << currentLine << " : ";
        bool c1 = check1(currentLine);
        bool c2 = check2(currentLine);
        if( c1 && c2 )
        {
            nice++;
            std::cout << "nice   ";
        }
        else
        {
            std::cout << "naughty";
        }
        std::cout << " (" << (c1 ? '.' : 'x' ) << " , " << (c2 ? '.' : 'x') << ")\n";
    }

    std::cout << "nice: " << nice << "\n";
    //input.close();
    return 0;
}