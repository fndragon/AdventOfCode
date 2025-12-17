#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
long long sum = 0;

std::string get_md5(std::string input)
{
    std::ofstream outFile;
    outFile.open("out.txt");
    outFile << input;
    outFile.close();
    std::system("md5 out.txt >test.txt"); // executes the UNIX command "ls -l >test.txt"
    std::stringstream ss;
    ss << std::ifstream("test.txt").rdbuf();
    return ss.str();
}

int main(void)
{
    //std::ifstream input;
    //input.open("input.txt");
    std::string input = "bgvyzdsv";
    bool done=false;
    int count = 0;
    std::stringstream ss;
    std::string test, result, first5;
    std::cout << "starting: ";
    while( !done )
    {
        count++;
        ss.str("");
        ss << input << count;
        test = ss.str();
        result = get_md5(test);
        first5 = result.substr(16, 5);
        done = ("00000" == first5);
        if( (count % 1000) == 0)
        {
            std::cout << "." << std::flush;
        }
    }

    std::cout << "input: " << test << " md5: " << result << "\n";
    //input.close();
    return 0;
}