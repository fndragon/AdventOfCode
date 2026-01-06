#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>
#include <vector>
#include <queue>
#include <string.h>

int currentNum(int iteration, int position)
{
    if (position < iteration)
    {
        return 0; //left of diagonal
    }
    
    int location = position - iteration;
    int index = location % (4 * (iteration + 1));
    int actualPos = (int) floor(index / (iteration + 1));
    if (actualPos == 0) return 1;
    if (actualPos == 2) return -1;
    return 0;
}

std::vector< std::vector< int > > generateArray(std::string s)
{
    std::vector< std::vector< int > > result;
    std::vector< int > base = { 1, 0, -1, 0 };

    size_t size = s.size();
    for(int i=0;i<size;i++)
    {
        int baseOffset = 0;
        if( i > 0 ) baseOffset = 1;
        std::vector< int > generatedRow;
        // generate a std::vector<int> for this element (left most first)
        for(int j=0;j<size;j++)
        {
            generatedRow.push_back(currentNum(i,j));
        }
        result.push_back(generatedRow);
    }

    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            std::cout << "  " << result[i][j];
        }
        std::cout << "\n";
    }
    return result;
};

std::string calcPhase(std::string input, std::vector<std::vector<int> > &pattern)
{
    std::string result;
    result.resize(input.size());
    for(int i=0;i<input.size();i++)
    {
        int sum=0;
        for(int j=i;j<input.size();j++)
        {
            sum += (input[j] - '0') * pattern[i][j];
        }
        result[i] = (abs(sum) % 10) + '0';
    }
    return result;
}

void convert(std::string &input, std::vector<uint8_t>& out)
{
  int tmp;
  size_t offset = 0;
  for (int i = 0; i < 7; i++) {
    tmp = input[i];
    tmp -= '0';
    offset = offset * 10 + tmp;
  }

  out.resize(input.size());
  for(int i=0;i<input.size();i++)
  {
    out[i] = (uint8_t) input[i];
  }

  const size_t n = out.size();
  const size_t repeats = 10000 - offset / n;

  out.resize(n * repeats);
  for (size_t i = 1; i < repeats; i++)
  {
    std::memcpy(out.data() + i * n, out.data(), n * sizeof(uint8_t));
  }
  out.erase(out.begin(), out.begin() + offset % n);

  return;
}

//
void apply_pattern(const std::vector<uint8_t> & in, std::vector<uint8_t> & out)
{
  const int n = static_cast<int>(in.size());
  int sum = 0;
  for (int i = n - 1; i >= 0; --i)  // Here's the "trick".  Go from back to front and always just sum up
  {
    sum += in[i];
    out[i] = sum % 10;
  }
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine, pt1Line;

    while( std::getline(input, currentLine) )
    {
        std::cout << currentLine << "\n";
        pt1Line = currentLine; // only one line input
    }

    std::string test1= "12345678";
    std::vector< std::vector< int > > test1Mult = generateArray(test1);
    std::string test1Phase1 = calcPhase(test1, test1Mult);
    std::cout << "test1 after 1 phase: " << test1Phase1 << '\n';
    test1Phase1=calcPhase(test1Phase1, test1Mult);
    std::cout << "test1 after 2 phase: " << test1Phase1 << '\n';
    test1Phase1=calcPhase(test1Phase1, test1Mult);
    std::cout << "test1 after 3 phase: " << test1Phase1 << '\n';
    test1Phase1=calcPhase(test1Phase1, test1Mult);
    std::cout << "test1 after 4 phase: " << test1Phase1 << '\n';
    
    std::string test2="80871224585914546619083218645595";
    std::vector< std::vector< int > > test2Mult = generateArray(test2);
    std::string test2Phased = test2;
    for(int i=0;i<100;i++)
    {
        test2Phased = calcPhase(test2Phased, test2Mult);
    }
    std::cout << "test2: " << test2 << " becomes " << test2Phased <<"\n";

    // pt1:
    std::vector< std::vector< int > > pt1Mult = generateArray(pt1Line);
    std::string testStr = pt1Line;
    for(int i=0;i<100;i++)
    {
        testStr = calcPhase(testStr, pt1Mult);
    }
    std::cout << "pt1: " << pt1Line << " becomes " << testStr <<"\n";

    // pt2:  Input is 10000x input text.
    // The first seven digits of your initial input signal also represent the message offset.
    // We don't have enough memory to store input + all of the -1/0/+1 integers...
    std::vector<uint8_t> data[2];
    convert(pt1Line, data[0]);
    data[1].resize(data[0].size());
    std::vector<uint8_t> &start = data[0], &next = data[1];
    int numPhases = 100;
    for(int i=0;i<numPhases;i++)
    {
        apply_pattern(start, next);
        std::vector<uint8_t> &backup = start;
        start = next;
        next = backup;
    }

    size_t finalDst = ((numPhases - 1) ^ 1) & 1; // 99 xor'd with 1 masked for the lsb.  If 99 is odd, this means 1 xor 1 = 0
    std::string digits;
    digits.resize(8);
    for (size_t i = 0; i < 8; i++)
    {
        digits[i] = data[finalDst][i] + '0';
    }
    std::cout << " pt2: result is " << digits << "\n";

    input.close();
    return 0;
}