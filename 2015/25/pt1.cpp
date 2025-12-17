#include <numbers>
#include <iostream>
#include <algorithm>

int main()
{
    // figure out row/column number
    int row = 2978;
    int column = 3083;
    // Figure out the starting column for row 2978:
    uint64_t index = 1; // topleft starts at 1

    for(int i=1;i<row;i++)
    {
        // to get from row n to row n+1 you add the row number of n:
        // 1+1 >> 2
        // 2+2 >> 4
        // 4+3 >> 7
        // 7+4 >> 11
        index += i;
    }
    std::cout << "index is " << index << "\n";
    // Now to get from that element to column, do a different loop:
    for( int i=1;i<column;i++)
    {
        // to get from column n to column n+1 you add the column of n+1;
        //  1   3   6  10  15  21
        //  2   5   9  14  20
        //  4   8  13  19
        // So, in row#3, 4 + (3+1) == 8
        //               8 + (3+2) == 13
        //              13 + (3+3) == 19
        // generic form is index += (1-based row) + i;
        index += row + i;
    }
    // iterate counting algorithm that many times
    std::cout << " Total number of times to loop: " << index - 1 << "\n";

    uint64_t prev = 20151125;
    for( int i=0;i< index - 1;i++)
    {
        prev = (prev * 252533) % 33554393;
    }
    std::cout << "Value should be " << prev << "\n";
    return 0;
}