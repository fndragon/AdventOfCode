#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>

struct Recipe
{
    std::string input;
    std::vector< std::string > output; // outputs
};

struct Element
{
    int offset; // offset in original molecule string
    Recipe *myRecipe;
    int permState; // If -1, not permuting this offset.  If 0+, which recipe output to use.
    // if -2, skip this mod anymore
};

std::vector< Element > mods;

std::string molecule;
std::vector< Recipe > recipies;

void addRecipe(std::string &line)
{
    Recipe r;
    std::string localLine = line;
    char *c = strtok(&localLine[0], " ");
    r.input = std::string(c);
    c = strtok(nullptr, " ");
    c = strtok(nullptr, " ");
    std::string outputElement(c);
    r.output.push_back(outputElement);

    // see if recipe exists
    for( Recipe &rec : recipies )
    {
        if( rec.input == r.input )
        {
            rec.output.push_back(outputElement); // we're done.
            return;
        }
    }
    recipies.push_back(r);
}

Recipe *findRecipe(std::string name)
{
    for( int i=0;i<recipies.size();i++)
    {
        if( recipies[i].input == name )
        {
            return &recipies[i];
        }
    }
    std::cout << "******\n";
    return nullptr;
}

bool isModifiableElement(std::string element)
{
    for( Recipe r : recipies )
    {
        if( r.input == element) return true;
    }
    return false;
}

void permute(void)
{
    // add 1 to the mods in order until they wrap, then continue to the next if they wrap.
    auto i = mods.begin();
    while( i != mods.end() )
    {
        i->permState++;
        if( i->permState < i->myRecipe->output.size() )
        {
            return;
        }
        // otherwise, keep permuting up the stack.
        i->permState = -1;
        ++i;
    }
}

void permute2(void)
{
    // go through all of the mods and trigger them in order, moving on once they're done.
    for(int i=0;i< mods.size();i++)
    {
        if( mods[i].permState == -2 )
        {
            continue; // skip!
        }
        mods[i].permState++;
        if( mods[i].permState < mods[i].myRecipe->output.size() )
        {
            return;
        }
        mods[i].permState = -2;
    }
}

bool isDone(void)
{
    for( auto m : mods )
    {
        if( m.permState >= 0 )
        {
            return false;
        }
    }
    return true;
}

std::string applyMods( std::string molecule )
{
    std::string output;
    for( int i=0;i<molecule.size();i++)
    {
        std::string replace { molecule[i] }; // default to original string
        for( auto m: mods )
        {
            if(( i == m.offset ) && (m.permState >= 0))
            {
                // replace
                replace = m.myRecipe->output[m.permState];
                i += m.myRecipe->input.size() - 1; // skip the g part of Hg for instance.
                break; // break out of for loop, we're done.
            }
        }
        output.append(replace);
    }
    std::cout << "molecule went from " << molecule << " to " << output << "\n";
    return output;
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    std::vector< int > size;
    bool getInput = false;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        if(currentLine.empty())
        {
            getInput=true;
            continue;
        }
        if( !getInput ) 
        { 
            addRecipe(currentLine);
        }
        else 
        {
            molecule = currentLine;
        }
    }
    // inputs are done.

    // now, for the molecule:
    // Splice it up into modifiable sections (how many permutable objects)

    for(int i=0;i<molecule.size();i++)
    {
        std::string element = molecule.substr(i,1);
        if( isModifiableElement(element) )
        {
            Element e;
            e.offset = i;
            e.myRecipe = findRecipe(element);
            e.permState = -1;
            mods.push_back(e);
        }
        else
        {
            if( i+1 < molecule.size())
            {
                element = molecule.substr(i,2);
                if( isModifiableElement(element) )
                {
                    Element e;
                    e.offset = i;
                    e.myRecipe = findRecipe(element);
                    e.permState = -1;
                    mods.push_back(e);

                    i++; // to account for the 2nd char
                }
                //else fall through and move 1 char up.
            }
        }
    }

    // figure out the number of permutations for each section
    // try all combinations of all permutations and store the string result
    long maximumCombo = 1;
    for(int i=0;i<mods.size();i++)
    {
        // for each element, find the appropriate recipe that might be used
        maximumCombo *= mods[i].myRecipe->output.size();
    }

    std::vector< std::string > resultArray;
    // HOHOHO
    permute2();
    do
    {
        //permute();
        resultArray.emplace_back(applyMods(molecule));
        permute2();
    } while (!isDone());

    // eliminate duplicates
    auto last = std::unique(resultArray.begin(), resultArray.end());
    resultArray.erase(last, resultArray.end());
    std::cout << " Result array is size " << resultArray.size() << "\n";
    input.close();
    return 0;
}