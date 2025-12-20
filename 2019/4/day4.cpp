#include <iostream>
#include <string>
#include <sstream>

bool testPassword(std::string &pw)
{
	if(pw.size() != 6) return false;
	int num = std::stoi(pw);

	if( num < 245182) return false;
	if( num > 790572) return false;

	bool adjacent = false;
	for(int i=1;i<pw.size();i++)
	{
		if( pw[i-1] == pw[i] )
		{
			adjacent=true;
			break;
		}
	}
	if( !adjacent) return false;

	bool decreasing=false;
	int prevDigit=0;
	for(int i=0;i<pw.size();i++)
	{
		char c = pw[i];
		int digit = std::atoi(&c);
		if( digit >= prevDigit )
		{
			prevDigit = digit;
		}
		else
		{
			decreasing=true;
			break;
		}
	}
	if( decreasing ) return false;
	return true;
}

bool testAdditional(std::string &pw)
{
	// Simple check:  If any 0..9 pair exists, we're good:
	for(int i=0;i<=9;i++)
	{
		int count=0;
		for(int j=0;j<pw.size();j++)
		{
			char c = pw[j];
			if( std::atoi(&c) == i) count++;
		}
		if(count == 2) return true;
	}
	return false;
}

int main()
{
	std::cout << "Hello\n";
	std::string pass;
	int count =0;
	int pt2Count =0;
	for(int i=245182; i< 790573;i++)
	{
		std::stringstream ss;
		ss << i;
		pass = ss.str();
		ss.str("");
		if( testPassword(pass) ) count++;
		if( testPassword(pass) && testAdditional(pass) ) pt2Count++;
	}
	std::cout << "Pt1: " << count << "\n";
	std::cout << "Pt2: " << pt2Count << "\n";
	return 0;
}