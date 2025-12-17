#include <iostream>

int attemptDeliver( int elfNum, int houseNum )
{
    if (houseNum % elfNum == 0)
    {
        if( houseNum <= elfNum * 50 )
        {
            return 11 * elfNum;
        }
    }
    return 0;
}

int main()
{
    std::cout << "test\n";
    int houseNum = 1;
    while(true)
    {
        long sum = 0;
        for(int i=1;i<=houseNum;i++)
        {
            sum += attemptDeliver(i, houseNum);
        }
        if( sum > 29000000)
        {
            std::cout << "housenum is " << houseNum << "\n";
            break;
        }
        //std::cout << "[" << houseNum << "]: gets " << sum << " presents\n";
        houseNum++;
    }
    return 0;
}