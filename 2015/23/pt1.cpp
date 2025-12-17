#include <iostream>
#include <fstream>
#include <vector>
#include <string>

typedef enum 
{
    hlf = 0,
    tpl,
    inc,
    jmp,
    jie,
    jio
} cmd;

struct Instruction
{
    cmd instruction; // instruction
    int regInput; // a=1, b=2,etc.
    int offsetInput;
};
std::vector< Instruction > code;
void addInstruction(std::string line)
{
    Instruction i;
    i.offsetInput = 0;
    i.regInput = 0;
    std::string localLine = line;
    char *c = strtok(&localLine[0], " ,");
    std::string inst(c);
    if( inst == "hlf" ) i.instruction = hlf;
    if( inst == "tpl" ) i.instruction = tpl;
    if( inst == "inc" ) i.instruction = inc;
    if( inst == "jmp" ) i.instruction = jmp;
    if( inst == "jie" ) i.instruction = jie;
    if( inst == "jio" ) i.instruction = jio;

    c = strtok(nullptr, " ,");
    switch( i.instruction )
    {
        case hlf:
            i.regInput = *c - 'a';
            break;
        case tpl:
            i.regInput = *c - 'a';
            break;
        case inc:
            i.regInput = *c - 'a';
            break;
        case jmp:
            i.offsetInput = atoi(c);
            break;
        case jie:
            i.regInput = *c - 'a';
            c = strtok(nullptr, " ,");
            i.offsetInput = atoi(c);
            break;
        case jio:
            i.regInput = *c - 'a';
            c = strtok(nullptr, " ,");
            i.offsetInput = atoi(c);
            break;
        break;
        default:
            std::cout << "error\n";
            break;  
    }
    code.push_back(i);
}

int main(void)
{
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    std::vector< int > size;
    while( std::getline(input, currentLine) )
    {
        std::cout << "line : " << currentLine << "\n";
        addInstruction(currentLine);
    }

    //execute
    int PC = 0;
    std::vector< int > regs = { 0, 0, 0, 0};
    // PART 2:
    regs[0] = 1;
    while( true ) 
    {
        switch ( code[PC].instruction )
        {
            case inc:
                regs[code[PC].regInput]++;
                PC++;
                break;
            case jmp:
                PC += code[PC].offsetInput;
                break;
            case hlf:
                regs[code[PC].regInput] /= 2;
                PC++;
                break;
            case tpl:
                regs[code[PC].regInput] *= 3;
                PC++;
                break;
            case jie:
                if((regs[code[PC].regInput] % 2) == 0) 
                {
                    PC += code[PC].offsetInput;
                }
                else
                {
                    PC++;
                }
                break;
            case jio:
                if(regs[code[PC].regInput] == 1)
                {
                    PC += code[PC].offsetInput;
                }
                else
                {
                    PC++;
                }
                break;
            default:
                break;
        }
        if( PC > code.size()) break;
        if( PC < 0 ) break;
    }

    // check regs
    std::cout << "Register B: " << regs[1] << "\n";
    input.close();
    return 0;
}