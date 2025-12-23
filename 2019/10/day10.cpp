#include <math.h>
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

struct Vector2
{
    float x,y;
};

float distanceSquared(Vector2 a, Vector2 b)
{
    auto dx = (a.x - b.x);
    auto dy = (a.y - b.y);
    return dx*dx + dy*dy;
}

bool containsPoint(Vector2 lineP, Vector2 lineQ, Vector2 point)
{
    auto d2PPoint = distanceSquared(lineP, point);
    auto d2QPoint = distanceSquared(lineQ, point);
    auto d2PQ = distanceSquared(lineP, lineQ);
    return d2PQ == d2PPoint + d2QPoint + 2*std::sqrt(d2PPoint*d2QPoint);
}

struct Location
{
    bool asteroid;
    int x,y;
    int count;
    float angle;
};

std::vector< Location > grid;
int width, height;

bool visible(Location const *left, Location const *right)
{
    Vector2 leftV, rightV, testV;
    // search through grid for asteroids
    for(int i=0;i<grid.size();i++)
    {
        if( grid[i].asteroid )
        {
            if ( &grid[i] != left && &grid[i] != right ) // check pointers
            {
                // Now we do a full point check
                // if the line defined between asteroids left and right contains grid[i]
                // then left and right don't see each other.
                leftV.x = left->x;
                leftV.y = left->y;
                rightV.x = right->x;
                rightV.y = right->y;
                testV.x = i % width;
                testV.y = floor( i / width);
                if( containsPoint( leftV, rightV, testV) )
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void updateArray(void)
{
    for(int i=0;i<grid.size();i++)
    {
        if( grid[i].asteroid)
        {
            for(int j=i+1;j<grid.size();j++)
            {
                if( grid[j].asteroid )
                {
                    // if I can see J, add one to each's count.
                    if( visible( &grid[i], &grid[j] ))
                    {
                        grid[i].count++;
                        grid[j].count++;
                    }
                }
            }
        }
    }
}

float calculateAngle( Location *pivot, Location *target)
{
    Vector2 a = { 0, 1 }; // Y axis
    Vector2 b = { (float)( target->x - pivot->x ), (float)( target->y - pivot->y)};
    // 2D Cross product is a scalar: a.x * b.y - a.y * b.x
    float cross_product_z = a.x * b.y - a.y * b.x;
    // Dot product is a scalar: a.x * b.x + a.y * b.y
    float dot_product = a.x * b.x + a.y * b.y;

    // atan2 uses cross product (y-component) and dot product (x-component)
    // to find the angle in the correct quadrant
    return std::atan2(cross_product_z, dot_product);
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    // self tests
    Location L1, L2;
    L1.x = 0;
    L1.y = 0;
    L2.x = -5;
    L2.y = -5;
    float angle = calculateAngle(&L1, &L2);
    //return 0;

    width=0;
    height=0;
    while( std::getline(input, currentLine) )
    {
        height++;
        width=currentLine.length();
        std::cout << "current: " << currentLine << "\n";
        for(int i=0;i<currentLine.size();i++)
        {
            Location L;
            L.asteroid = currentLine[i] == '#';
            L.x = i;
            L.y = height-1;
            L.count = 0;
            grid.emplace_back(L);
        }
    }
    // now iterate through the objects.  can be done in parallel...
    updateArray();

    // find best location by maximizing count
    int bestCount =0;
    Location *best = nullptr;
    for(int i=0;i<grid.size();i++)
    {
        if( bestCount < grid[i].count )
        {
            bestCount = grid[i].count;
            best = &grid[i];
        }
    }
    std::cout << "best count: " << bestCount << "\n";

    // 2nd pass: Mark the visible asteroids from this location
    std::vector< Location * > visibleObjs;
    for(int i=0;i<grid.size();i++)
    {
        if( grid[i].asteroid && 
            visible(&grid[i], best ) &&
            &grid[i] != best )
        {
            visibleObjs.push_back(&grid[i]);
        }
    }
    std::cout << "visible is " << visibleObjs.size() << "\n";
    // then go through and for each marked asteroid, calculate the angle from due north, to laser, to the asteroid.
    for(int i=0;i<visibleObjs.size();i++)
    {
        visibleObjs[i]->angle = calculateAngle(best, visibleObjs[i]);
    }
    // then sort hits by this angle, and report the 200th .
    std::sort(visibleObjs.begin(), visibleObjs.end(), 
        [](Location *a, Location *b)
        {
            return a->angle < b->angle;
        });
    // since the station can "see" 260 in our example, we don't worry about multiple passes.

    std::cout << "200th object (really index 199): ";
    std::cout << visibleObjs[199]->x << " , " << visibleObjs[199]->y << "\n";
    //903 too high
    //12 not right
    input.close();
    return 0;
}
