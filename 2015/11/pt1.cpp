#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::string increment(std::string input)
{
    std::string output = input;

    for(int i=input.length()-1;i>0;i--)
    {
        // rotate i (least significant character)
        if(output[i] == 'z')
        {
            output[i] = 'a';
            continue;
        }
        else
        {
            output[i] = output[i] + 1;
            return output;
        }
    }
    // if we get here... we started with zzzzzzzzzzzz
    std::cout << "Increment error\n";
    return output;
}

bool validate(std::string input)
{
    //Passwords may not contain the letters i, o, or l, as these letters can be mistaken for other characters and are therefore confusing.
    for(int i=0;i<input.length();i++)
    {
        char c = input[i];
        if(( c == 'i') || (c == 'o') || (c == 'l'))
        return false;
    }
    // Passwords must include one increasing straight of at least three letters, 
    // like abc, bcd, cde, and so on, up to xyz. They cannot skip letters; abd doesn't count.
    bool increasingStraight = false;
    for(int i=0;i<input.length()-2;i++)
    {
        if(( input[i] + 1 == input[i+1] ) && (input[i] + 2 == input[i+2]) )
        {
            increasingStraight = true;
            break;
        }
    }
    if( !increasingStraight ) return false;

    //Passwords must contain at least two different, non-overlapping pairs of letters, like aa, bb, or zz.

    bool overlappingPairs1 = false;
    bool overlappingPairs2 = false;
    for(int i=0;i<input.length()-1;i++)
    {
        if( input[i] == input[i+1] )
        {
            if( !overlappingPairs1 )
            {
                overlappingPairs1 = true;
                i++; // to avoid overlap.
            }
            else
            {
                overlappingPairs2 = true;
                break;
            }
        }
    }
    return overlappingPairs2;
}

int main(void)
{
    std::string pt1 = "hxbxwxba";
    std::string currentLine = "hxbxxyzz";
    do
    {
        currentLine = increment(currentLine);
    } while( !validate(currentLine));
    std::cout << "Validated answer is " << currentLine << "\n";
    return 0;
}