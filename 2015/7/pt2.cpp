#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

// name, value
std::map< std::string, unsigned short > values;

typedef enum {
    ASSIGN = 0,
    AND,
    OR,
    NOT,
    LSHIFT,
    RSHIFT
} wireType;

struct wireDef
{
    std::string input1;
    std::string input2;
    std::string output;
    wireType type; // 0 = assign, 1 = and, 2 = or, 3 = not, 4 = lshift, 5 = rshift
};

std::vector< wireDef > wires;

void notOp(std::string inputwire, std::string outputwire)
{
    values[outputwire] = ~ values[inputwire];
}

void assignValue( std::string inputNumber, std::string outputwire)
{
    if( inputNumber[0] >= '0' && inputNumber[0] <= '9')
    {
        values[outputwire] = (unsigned short) std::atoi(inputNumber.c_str());
    }
    else
    {
        values[outputwire] = values[inputNumber];
    }
}

void andOp( std::string input1, std::string input2, std::string outputwire)
{
    unsigned short a, b;
    if( values.find(input1) == values.end() )
    {
        a = 1;
    }
    else
    {
        a = values[input1];
    }
    b = values[input2];
    values[outputwire] = a & b;
}

void orOp( std::string input1, std::string input2, std::string outputwire)
{
    values[outputwire] = values[input1] | values[input2];
}

void lshiftOp( std::string input1, std::string shiftnum, std::string outputwire)
{
    int shiftamount = std::atoi(shiftnum.c_str());
    values[outputwire] = values[input1] << shiftamount;
}

void rshiftOp( std::string input1, std::string shiftnum, std::string outputwire)
{
    int shiftamount = std::atoi(shiftnum.c_str());
    values[outputwire] = values[input1] >> shiftamount;
}

void processLine( std::string line )
{
    wireDef localWire;
    // 123 -> x - set line
    // x AND y -> z - AND inputs to output z
    // p LSHIFT 2 -> q 
    // NOT e -> f

    // Process NOT first
    char *token = strtok((char * )line.data(), " ");
    std::string tokstr(token);
    if( tokstr == "NOT" )
    {
        token = strtok(nullptr, " ");
        localWire.input1 = token;
        token = strtok(nullptr, " -> ");
        localWire.output = token;
        localWire.type = wireType::NOT;
        //notOp(inputwire, outputwire);
    }
    else // either value or AND/SHIFTS
    {
        token = strtok(nullptr, " ");
        std::string checkStr(token);
        if( checkStr == "->")
        {
            // Simple assignment.
            token = strtok(nullptr, " ");
            localWire.input1 = tokstr;
            localWire.output = token;
            localWire.type = wireType::ASSIGN;
            //assignValue( inputwire, outputwire );
        }
        else if( checkStr == "AND" )
        {
            localWire.input1 = tokstr;
            token = strtok(nullptr, " ->");
            localWire.input2 = token;
            token = strtok(nullptr, " ->");
            localWire.output = token;
            localWire.type = wireType::AND;
            //andOp(inputwire, input2wire, outputwire );
        }
        else if( checkStr == "OR" )
        {
            localWire.input1 = tokstr;
            token = strtok(nullptr, " ->");
            localWire.input2 = token;
            token = strtok(nullptr, " ->");
            localWire.output = token;
            localWire.type = wireType::OR;
            //orOp(inputwire, input2wire, outputwire );
        }
        else if( checkStr == "LSHIFT" )
        {
            localWire.input1 = tokstr;
            token = strtok(nullptr, " ->");
            localWire.input2 = token;
            token = strtok(nullptr, " ->");
            localWire.output = token;
            localWire.type = wireType::LSHIFT;
            //lshiftOp(inputwire, input2wire, outputwire);
        }
        else if( checkStr == "RSHIFT" )
        {
            localWire.input1 = tokstr;
            token = strtok(nullptr, " ->");
            localWire.input2 = token;
            token = strtok(nullptr, " ->");
            localWire.output = token;
            localWire.type = wireType::RSHIFT;
            //rshiftOp(inputwire, input2wire, outputwire);
        }
        else
        {
            std::cout << "ERROR\n";
        }
    }
    wires.push_back( localWire );
}

void removeRules( std::string target )
{
    std::vector< wireDef >::iterator wire = wires.begin();

    while( wire != wires.end())
    {
        if( wire->output == target )
        {
            wire = wires.erase(wire);
        }
        else
        {
            ++wire;
        }
    }
}

void resolve( std::string target )
{
    if( values.find(target) != values.end() )
    {
        return; // already resolved.
    }
    // find all rules with target as an output
    std::vector< wireDef > localRules;
    for( auto wire : wires )
    {
        if ( wire.output == target )
        {
            localRules.push_back(wire);
        }
    }
    // Recursively go through those rules and resolve them!
    for( auto wire : localRules )
    {
        switch( wire.type )
        {
            case wireType::ASSIGN:
                if( atoi(wire.input1.c_str()) == 0)
                {
                    resolve(wire.input1);
                }
                assignValue(wire.input1, wire.output);
                break;
            case wireType::AND:
                // special goober case
                if( wire.input1 != "1")
                {
                    resolve(wire.input1);
                }
                resolve(wire.input2);
                andOp(wire.input1, wire.input2, wire.output);
                break;
            case wireType::NOT:
                resolve(wire.input1);
                notOp(wire.input1, wire.output);
                break;
            case wireType::OR:
                resolve(wire.input1);
                resolve(wire.input2);
                orOp(wire.input1, wire.input2, wire.output);
                break;
            case wireType::LSHIFT:
                resolve(wire.input1);
                lshiftOp(wire.input1, wire.input2, wire.output);
                break;
            case wireType::RSHIFT:
                resolve(wire.input1);
                rshiftOp(wire.input1, wire.input2, wire.output);
                break;
            default: 
                std::cerr << "NOPE\n";
                break;
        }
    }
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    bool setup = true;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        processLine( currentLine );
    }

    // resolve wires and dependencies
    resolve( "a" );
    std::cout << "A is: " << values["a"] << "\n";

    // Now, take the signal you got on wire a, override wire b to that signal, and reset the other 
    // wires (including wire a). What new signal is ultimately provided to wire a?
    short valueToStore = values["a"];
    values.clear();

    // remove rules for B recursively
    removeRules("b");

    // Add new rule for B
    std::stringstream ss;
    ss << valueToStore << " -> b";
    processLine(ss.str());
    resolve("a");

    std::cout << "Final A is " << values["a"] << "\n";

    input.close();
    return 0;
}