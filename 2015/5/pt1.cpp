#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

long long nice = 0;

bool check1(std::string &str)
{
    int count=0;
    // at least 3 vowels
    for( int i=0;i<str.length();i++)
    {
        if((str[i] == 'a') ||
           (str[i] == 'e') ||
           (str[i] == 'i') ||
           (str[i] == 'o') ||
           (str[i] == 'u') )
        {
            count++;
            if(count >= 3)
            {
                return true;
            }
        }
    }
    return false;
}

bool check2(std::string &str)
{
    for(int i=0;i<( str.length() - 1) ;i++)
    {
        if( str[i] == str[i+1] )
        {
            return true;
        }
    }
    return false;
}

bool check3(std::string &str)
{
    for(int i=0;i<( str.length() - 1) ;i++)
    {
        std::string sub = str.substr(i, 2);
        if(( sub == "ab" ) || ( sub == "cd" ) || ( sub == "pq" ) || (sub == "xy"))
        {
            return false;
        }
    }
    return true;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    while( std::getline(input, currentLine) )
    {
        if( check1(currentLine) && check2(currentLine) && check3(currentLine))
        {
            nice++;
        }
    }

    std::cout << "nice: " << nice << "\n";
    //input.close();
    return 0;
}