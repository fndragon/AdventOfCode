#include <iostream>
#include <string>
#include <vector>

int leastManaWithWin = 9999999;

int bossDamage = 8; //Damage: 8

void recordmana( int mana )
{
    if( mana < leastManaWithWin )
    {
        leastManaWithWin = mana;
        std::cout << "New mana record: " << leastManaWithWin << "\n";
    }
}

// round starts after any player-turn ticks.  Assumed zero in turn 0
void round(int bosshp, int playerhp, int currentmana, int spentmana, int shield, int poison, int refresh, int nextspell)
{
    if( spentmana > leastManaWithWin ) return; // we can't win now.
    // spell:
    switch( nextspell )
    {
        case 0: // magic missile
            if( currentmana < 53 ) return; // we can't do this, so bail
            currentmana -= 53;
            spentmana += 53;
            bosshp -= 4;
            break;
        case 1: // drain
            if( currentmana < 73 ) return;
            currentmana -= 73;
            spentmana += 73;
            bosshp -= 2;
            playerhp += 2;
            break;
        case 2: //shield
            if( currentmana < 113 ) return;
            if( shield > 0 ) return;
            currentmana -= 113;
            spentmana += 113;
            shield = 6;
            break;
        case 3: // poison
            if( currentmana < 173 ) return;
            if( poison > 0 ) return;
            currentmana -= 173;
            spentmana += 173;
            poison = 6;
            break;
        case 4: //refresh
            if( currentmana < 229 ) return;
            if( refresh > 0 ) return;
            currentmana -= 229;
            spentmana += 229;
            refresh = 5;
            break;
        default: 
            std::cout << "Error\n";
            break;
    }
    
    // boss turn
    int armor = 0;
    if( refresh > 0 ) { currentmana += 101; refresh--; }
    if( shield > 0 ) { armor = 7; shield--; }
    if( poison > 0 ) { bosshp -= 3; poison--; }
    if( bosshp <= 0 ) { recordmana(spentmana); return; }

    // boss attack
    playerhp -= (bossDamage - armor);
    if( playerhp <= 0 ) return;

    // player turn
    if( refresh > 0 ) { currentmana += 101; refresh--; }
    if( shield > 0 ) { armor = 7; shield--; }
    if( poison > 0 ) { bosshp -= 3; poison--; }
    if( bosshp <= 0 ) { recordmana(spentmana); return; }

    for(int i=0;i<5;i++)
    {
        round(bosshp, playerhp, currentmana, spentmana, shield, poison, refresh, i);
    }
}

int main()
{
    for(int i=0;i<5;i++)
    {
        // reset state here
        int playerHP = 50;
        int playerMana = 500;
        int bossHP = 55; //Hit Points: 55
        // store game state in the call to Round
        round(bossHP, playerHP, playerMana, 0, 0, 0, 0, i );
    }
    std::cout << " best mana with win " << leastManaWithWin << "\n";
}