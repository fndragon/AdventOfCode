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

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;
    int height = 6;
    int width = 25;
    std::vector< int > grid;
    while( std::getline(input, currentLine) )
    {
        // only one line.

        for(int i=0;i<currentLine.size();i++)
        {
            int va = currentLine[i] - '0';
            grid.emplace_back( va );
        }
    }

    for(int i=0;i<grid.size();i++)
    {
        std::cout << grid[i] << ",";
    }
    std::cout << '\n';

    int pixelsPerImage = width * height;
    int num0digits = 99999;
    int oneTimesTwo = 0;
    for(int i=0;i<grid.size() / pixelsPerImage;i++)
    {
        // process "image" layer
        int bins[3] = {0,0,0};
        for(int j=(i * pixelsPerImage);j<((i+1)*pixelsPerImage);j++)
        {
            bins[grid[j]]++;
        }
        std::cout << "Processing layer " << i << " : [" << bins[0] << "," <<bins[1]<<","<<bins[2]<<"]\n";
        if( bins[0] < num0digits )
        {
            num0digits = bins[0];
            oneTimesTwo = bins[1] * bins[2];
        }
    }
    //1593 too high
    std::cout << "Pt1: " << oneTimesTwo << "\n";

    std::vector< int > output;
    output.resize(pixelsPerImage);
    // now process transparency and generate an image:
    for(int i=0;i<pixelsPerImage;i++)
    {
        int pixVal = 2; // assume transparent to start
        // for each pixel go from the bottom up
        for(int j=0;j<grid.size() / pixelsPerImage;j++)
        {
            int index = j * pixelsPerImage + i;
            if( grid[index] == 2 ) continue;
            if(( grid[index] == 1 ) && (pixVal == 2))
            {
                pixVal = 1; // white
            }
            else if(( grid[index] == 0) && (pixVal == 2))
            {
                pixVal = 0; // black
            }
        }
        output[i] = pixVal;
    }

    for(int h=0;h<height;h++)
    {
        for(int w=0;w<width;w++)
        {
            int index = h * width + w;
            std::cout << ((output[index]==0) ? "." : ((output[index]==1) ? "W" : "."));
        }
        std::cout << "\n";
    }

    input.close();
    return 0;
}