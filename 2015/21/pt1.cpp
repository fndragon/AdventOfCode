#include <vector>
#include <iostream>

struct Ring
{
    int cost;
    int damage;
    int armor;
};

int main(void)
{
    std::vector< std::pair< int, int > > weapons = { {4,8}, {5,10}, {6,25}, {7,40}, {8,65}, {9,90}};
    std::vector< std::pair< int, int > > armor = { {0,0}, {1,13},{2,31},{3,51},{4,71},{5,93} };
    std::vector< Ring > rings = { {0,0,0}, {0,0,0}, {25, 1, 0}, {50, 2, 0}, {100,3,0},{20,0,1},{40,0,2},{80,0,3}};
    std::vector< int > ringIndex = {0,1,2,3,4,5,6,7};
    std::vector< int > costs;
    for( auto w: weapons )
    {
        for( auto a: armor )
        {
            do
            {
                // next permutation will cycle all ring possibilities by.
                // Assume the FIRST two elements [0,1] are the currently equipped rings.
                
                int hp=100;
                int damage = w.first;
                int defense = a.first;
                int bossHP = 104;
                int bossDam = 8;
                int bossArm = 1;
                while( bossHP > 0 && hp > 0)
                {
                    bossHP -= damage - bossArm + rings[ringIndex[0]].damage + rings[ringIndex[1]].damage;
                    if( bossHP <= 0 ) break;
                    hp -= bossDam - defense - rings[ringIndex[0]].armor - rings[ringIndex[1]].armor;
                }
                if( bossHP <= 0)
                {
                    costs.push_back( w.second + a.second + rings[ringIndex[0]].cost + rings[ringIndex[1]].cost);
                }

            } while (std::next_permutation(ringIndex.begin(), ringIndex.end()));
        }
    }

    // find the lowest cost
    int cost = 1000000;
    for( auto c : costs )
    {
        cost = std::min<int>(cost, c);
    }
    std::cout << "cost is "<< cost << "\n";
    return 0;
}