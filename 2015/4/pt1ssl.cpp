#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <openssl/evp.h>
#include <openssl/md5.h>
long long sum = 0;

std::string get_md5(std::string input)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    EVP_Q_digest(NULL, "MD5", NULL, input.c_str(), input.size(), hash, NULL);
    std::stringstream ss;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}
//clang++ pt1ssl.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lcrypto -O2 -o pt1ssl
//./pt1ssl 
int main(void)
{
    //std::ifstream input;
    //input.open("input.txt");
    std::string input = "bgvyzdsv";
    bool done=false;
    int count = 0;
    std::stringstream ss;
    std::string test, result, first6;
    std::cout << "starting: ";
    while( !done )
    {
        count++;
        ss.str("");
        ss << input << count;
        test = ss.str();
        result = get_md5(test);
        first6 = result.substr(0, 6);
        done = ("000000" == first6);
        if( (count % 10000) == 0)
        {
            std::cout << "." << std::flush;
        }
    }

    std::cout << "input: " << test << " md5: " << result << "\n";
    //input.close();
    return 0;
}