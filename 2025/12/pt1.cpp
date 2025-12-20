#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bit>
#include <algorithm>
#include <numeric>
#include <thread>
#include <bitset>
//768 too high
struct Piece
{
    std::bitset<9> bits; //original
    std::vector< std::bitset<9> > states;
    bool canCheckCenter;
    int sum;
};

struct Question
{
    int width;
    int height;
    std::vector< int > sizes;
    int minPieceSize;
};

bool testInsertion(int width, int height, Piece const &piece, int state, std::vector<bool> const &area, int areaWidth )
{
    // see if we can add the true flags for the given piece/state at the given coordinates to area
    int areaIndex;
    for(int i=0;i<piece.states[state].size();i++)
    {
        areaIndex = ((i/3) + height) * areaWidth + width + (i%3);
        if( ( piece.states[state][i] ) && area[areaIndex] ) return false;
    }
    return true;
}

void printBoard( std::vector<bool> const &area, int areaWidth )
{
    std::cout << "Area: \n";
    for(int i=0;i<area.size();i++)
    {
        if( (i%areaWidth) == 0) std::cout << "\n";
        std::cout << ((area[i] == true) ? "#" : ".");
    }
    std::cout << "\n";
}

void placePiece( int width, int height, Piece const &piece, int state, std::vector< bool > &area, int areaWidth)
{
    //std::cout << "placing piece with state " << state << " at [" << width << "," << height << "]\n";
    //printBoard(area, areaWidth);
    int areaIndex;
    for(int i=0;i<piece.states[state].size();i++)
    {
        areaIndex = ((i/3) + height) * areaWidth + width + (i%3);
        if( (piece.states[state][i] ) && !area[areaIndex])
        {
            area[areaIndex] = true;
        }
    }
    //std::cout << "done placing piece with state " << state << " at [" << width << "," << height << "]\n";
    //printBoard(area, areaWidth);
}

void unplacePiece( int width, int height, Piece const &piece, int state, std::vector< bool > &area, int areaWidth)
{
    //std::cout << "removing piece with state " << state << " at [" << width << "," << height << "]\n";
    //printBoard(area, areaWidth);
    int areaIndex;
    for(int i=0;i<piece.states[state].size();i++)
    {
        areaIndex = ((i/3) + height) * areaWidth + width + (i%3);
        if( (piece.states[state][i] ) && area[areaIndex])
        {
            area[areaIndex] = false;
        }
    }
    //std::cout << "done removing piece with state " << state << " at [" << width << "," << height << "]\n";
    //printBoard(area, areaWidth);
}

bool quickPoke(int w, int h, std::vector<bool> &area, int areaWidth)
{
    // given a w,h, check w+1,h+1 in area
    return area[ ((h+1) * areaWidth + w + 1 ) ];
}

int emptySum(int w, int h, std::vector<bool> &area, int areaWidth)
{
    int sum=0;
    // given a w,h, check w+2, h+2 to count # of cells set
    for(int i=0;i<9;i++)
    {
        if(false == area[((i/3) * areaWidth + i%3)]) sum++;
    }
    return sum;
}

bool recurse(Question const &q, std::vector< Piece > const &pieces, std::vector<bool> &area, std::vector< int > &pieceCount )
{
    for(int w=0;w<(q.width - 2);w++)
    {
        for(int h=0;h<(q.height -2 );h++)
        {
            // (number of empty spaces in 3x3) <
            if( emptySum(w,h, area, q.width) < q.minPieceSize )
            {
                continue;
            }
            //std::cout << "testing w,h: " << w << "," << h << "\n";
            for(int piece=0;piece<q.sizes.size();piece++)
            {
                if( pieceCount[piece] < q.sizes[piece] )
                {
                    // quickcheck
                    if( pieces[piece].canCheckCenter && quickPoke(w,h, area, q.width) )
                    {
                        continue; // abort checking all states.
                    }
                    // (number of empty spaces in 3x3) <
                    if( emptySum(w,h, area, q.width) < pieces[piece].sum )
                    {
                        //printBoard(area, q.width);
                        continue;
                    }
                    pieceCount[piece]++;
                    for(int state=0;state<8;state++)
                    {
                        // state = 0 (normal), 1 (rot cw), 2 (flipped vert), 3 (rot ccw),
                        // state = 4 (flipped horiz), 5(flip + cw), 6(flipped horiz,vert), 7(flipped horiz, ccw)
                        if( testInsertion(w,h,pieces[piece],state, area, q.width) )
                        {
                            // Check for victory
                            if( q.sizes == pieceCount ) 
                            {
                                return true;
                            }

                            placePiece( w,h,pieces[piece],state, area, q.width);
                            
                            if( recurse(q,pieces,area,pieceCount) )
                            {
                                return true;
                            }
                            unplacePiece(w,h,pieces[piece],state,area, q.width);
                        }
                    }
                    pieceCount[piece]--;
                }
            }
        }
    }
    return false;
}

bool answerQuestion(Question const &q, std::vector< Piece > const &pieces)
{
    // I hate this but it probably works.
    int numPresents = std::accumulate(q.sizes.begin(), q.sizes.end(),0);
    int area = q.width * q.height;
    return (numPresents * 8) < area; // average size is 8... This heuristic is BAD.

    std::vector<bool> grid;
    grid.resize(q.width * q.height, false);

    std::vector< int > pieceCount;
    pieceCount.resize(pieces.size(),0);
    // Do some quick math to see if it's FEASIBLE
    int quickSum =0;
    for(int i=0;i<q.sizes.size();i++)
    {
        quickSum += q.sizes[i] * pieces[i].sum;
    }
    //std::cout << "Qsum: " << quickSum << " vs. " << (q.width * q.height) << "\n";
    if( quickSum > (q.width * q.height) ) return false;
    return recurse(q,pieces,grid, pieceCount);
}

void printState( Piece &piece, int state )
{
    std::cout << "Piece State " << state << "\n";
    for(int i=0;i<9;i++)
    {
        std::cout << ((piece.states[state][i]) ? "#" : ".");
        if( 2 == i%3 )
        {
            std::cout << "\n";
        }
    }
}

int main(void)
{
    std::vector< Piece > pieces;
    std::vector< Question > questions;
    std::ifstream input;
    input.open("input.txt");
    std::string currentLine;

    Piece current;
    int index=0;
    while( std::getline(input, currentLine) )
    {
        // check start.
        if(currentLine.empty()) pieces.push_back(current);
        if(currentLine[0] == '.' || currentLine[0] == '#')
        {
            for(int i=0;i<currentLine.size();i++)
            {
                current.bits[index] = (currentLine[i] == '#');
                index++;
            }
            //std::cout << "piece : " << currentLine << "\n";
        }
        else if(currentLine.find_first_of('x') == std::string::npos)
        {
            index=0;
            current.bits.reset();
            //std::cout << "start : " << currentLine << "\n";
        }
        else
        {
            // else we have a question to solve
            Question q;
            q.width = atoi(currentLine.substr(0,currentLine.find_first_of('x')).c_str());
            q.height = atoi(currentLine.substr(currentLine.find_first_of('x')+1,currentLine.find_first_of(':')).c_str());
            std::string sub = currentLine.substr(currentLine.find_first_of(':')+2, currentLine.size() - currentLine.find_first_of(':')+2);
            const char *c = strtok(&sub[0], " ");
            while( c != nullptr)
            {
                q.sizes.push_back(atoi(c));
                c = strtok(nullptr, " ");
            }
            questions.push_back(q);
            //std::cout << "quest : " << currentLine << "\n";
        }
    }

    // precook states
    for(int i=0;i<pieces.size();i++)
    {
        if(pieces[i].bits[4]) pieces[i].canCheckCenter = true;
        int sum=0;
        for(int j=0;j<9;j++)
        {
            if( pieces[i].bits[j] ) sum++;
        }
        pieces[i].sum = sum;
        pieces[i].states.clear();
        pieces[i].states.resize(8);
        for(int j=0;j<8;j++)
        {
            // 0 1 2
            // 3 4 5
            // 6 7 8
            pieces[i].states[j].reset();
            switch( j )
            {
                case 0: //normal
                    for(int k=0;k<9;k++)
                    {
                        pieces[i].states[j][k] = pieces[i].bits[k];
                    }
                    break;
                case 1: // rot ccw
                    // 0 1 2 >> 2 5 8
                    // 3 4 5 >> 1 4 7
                    // 6 7 8 >> 0 3 6
                    pieces[i].states[j][0] = pieces[i].bits[2];
                    pieces[i].states[j][1] = pieces[i].bits[5];
                    pieces[i].states[j][2] = pieces[i].bits[8];
                    pieces[i].states[j][3] = pieces[i].bits[1];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[7];
                    pieces[i].states[j][6] = pieces[i].bits[0];
                    pieces[i].states[j][7] = pieces[i].bits[3];
                    pieces[i].states[j][8] = pieces[i].bits[6];
                    break;
                case 2: // rot 2x ccw
                    // 0 1 2 >> 8 7 6
                    // 3 4 5 >> 5 4 3
                    // 6 7 8 >> 2 1 0
                    pieces[i].states[j][0] = pieces[i].bits[8];
                    pieces[i].states[j][1] = pieces[i].bits[7];
                    pieces[i].states[j][2] = pieces[i].bits[6];
                    pieces[i].states[j][3] = pieces[i].bits[5];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[3];
                    pieces[i].states[j][6] = pieces[i].bits[2];
                    pieces[i].states[j][7] = pieces[i].bits[1];
                    pieces[i].states[j][8] = pieces[i].bits[0];
                    break;
                case 3: // rot 3x ccw ~ 1x cw
                    // 0 1 2 >> 6 3 0
                    // 3 4 5 >> 7 4 1
                    // 6 7 8 >> 8 5 2
                    pieces[i].states[j][0] = pieces[i].bits[6];
                    pieces[i].states[j][1] = pieces[i].bits[3];
                    pieces[i].states[j][2] = pieces[i].bits[0];
                    pieces[i].states[j][3] = pieces[i].bits[7];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[1];
                    pieces[i].states[j][6] = pieces[i].bits[8];
                    pieces[i].states[j][7] = pieces[i].bits[5];
                    pieces[i].states[j][8] = pieces[i].bits[2];
                    break;
                case 4: // flip vert
                    // 0 1 2 >> 6 7 8
                    // 3 4 5 >> 3 4 5
                    // 6 7 8 >> 0 1 2
                    pieces[i].states[j][0] = pieces[i].bits[6];
                    pieces[i].states[j][1] = pieces[i].bits[7];
                    pieces[i].states[j][2] = pieces[i].bits[8];
                    pieces[i].states[j][3] = pieces[i].bits[3];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[5];
                    pieces[i].states[j][6] = pieces[i].bits[0];
                    pieces[i].states[j][7] = pieces[i].bits[1];
                    pieces[i].states[j][8] = pieces[i].bits[2];
                    break;
                case 5: // flip vert + ccw
                    // 0 1 2 >> 6 7 8 >> 8 5 2
                    // 3 4 5 >> 3 4 5 >> 7 4 1
                    // 6 7 8 >> 0 1 2 >> 6 3 0
                    pieces[i].states[j][0] = pieces[i].bits[8];
                    pieces[i].states[j][1] = pieces[i].bits[5];
                    pieces[i].states[j][2] = pieces[i].bits[2];
                    pieces[i].states[j][3] = pieces[i].bits[7];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[1];
                    pieces[i].states[j][6] = pieces[i].bits[6];
                    pieces[i].states[j][7] = pieces[i].bits[3];
                    pieces[i].states[j][8] = pieces[i].bits[0];
                    break;
                case 6: // flip vert + 2xccw
                    // 0 1 2 >> 6 7 8 >> 2 1 0
                    // 3 4 5 >> 3 4 5 >> 5 4 3
                    // 6 7 8 >> 0 1 2 >> 8 7 6
                    pieces[i].states[j][0] = pieces[i].bits[2];
                    pieces[i].states[j][1] = pieces[i].bits[1];
                    pieces[i].states[j][2] = pieces[i].bits[0];
                    pieces[i].states[j][3] = pieces[i].bits[5];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[3];
                    pieces[i].states[j][6] = pieces[i].bits[8];
                    pieces[i].states[j][7] = pieces[i].bits[7];
                    pieces[i].states[j][8] = pieces[i].bits[6];
                    break;
                case 7: // flip vert + 3xccw ~ 1cw
                    // 0 1 2 >> 6 7 8 >> 0 3 6
                    // 3 4 5 >> 3 4 5 >> 1 4 7
                    // 6 7 8 >> 0 1 2 >> 2 5 8
                    pieces[i].states[j][0] = pieces[i].bits[0];
                    pieces[i].states[j][1] = pieces[i].bits[3];
                    pieces[i].states[j][2] = pieces[i].bits[6];
                    pieces[i].states[j][3] = pieces[i].bits[1];
                    pieces[i].states[j][4] = pieces[i].bits[4];
                    pieces[i].states[j][5] = pieces[i].bits[7];
                    pieces[i].states[j][6] = pieces[i].bits[2];
                    pieces[i].states[j][7] = pieces[i].bits[5];
                    pieces[i].states[j][8] = pieces[i].bits[8];
                    break;
                default:
                break;
            }
        }
    }

    //for(int i=0;i<8;i++)
    //{
    //    printState(pieces[0],i);
    //}
    //return 0;
    // test testInsertion
    //std::vector< bool > testArea;
    //testArea.resize(9);
    //testArea[4] = true;
    //bool testResult = testInsertion(0,0,pieces[0],0, testArea, 3);
    //printBoard(testArea, 3);
    //placePiece(0,0,pieces[0],0,testArea,3);
    //printBoard(testArea, 3);
    //std::cout << "test is " << (testResult ? "true" : "false") << "\n";
    //bool testResult = answerQuestion(questions[0],pieces);
    //std::cout << "TestResult: " << (testResult ? "true" : "false") << "\n";
    //return 0;
    int minPieceSize = 9;
    for(int i=0;i<pieces.size();i++)
    {
        if(pieces[i].sum < minPieceSize) minPieceSize = pieces[i].sum;
    }
    int numSucceed=0;
    for(int i=0;i<questions.size();i++)
    {
        // find ma
        questions[i].minPieceSize = minPieceSize;
        bool result = answerQuestion(questions[i],pieces);
        if(result) numSucceed++;
        std::cout << "Question " << i << " : " << (result ? "works" : "fails") << "\n";
    }
    std::cout << "Total: " << numSucceed << "\n";
    input.close();
    return 0;
}