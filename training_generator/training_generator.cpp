// training_generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;


int main(int argc, const char* argv[])
{
	int unitSize = 0;
	if (argc == 2)
	{
		const char* arg = argv[1];
		cout << arg;
		//cout << arg << "\n";
		if (strcmp(arg, "-eel") == 0)
		{
			//unitSize = 100000;
			unitSize = 24000000;
			//cout << "elarge\n";
		}
		else if (strcmp(arg, "-el") == 0)
		{
			unitSize = 12000000;
			//cout << "elarge\n";
		}
		else if (strcmp(arg, "-l") == 0)
		{
			unitSize = 800000;
			//cout << "large\n";
		}
		else if (strcmp(arg, "-m") == 0)
		{
			unitSize = 120000;
			//cout << "medium\n";
		}
		else if (strcmp("-s", arg) == 0)
		{
			unitSize = 12000;
			//cout << "small\n";
		}
		else if (strcmp("-es", arg) == 0)
		{
			unitSize = 6000;
			//cout << "small\n";
		}
		else
		{
			cout << "invalid option extered. \nEciting. . .\n";
			exit(0);
		}
	}
	for (size_t i = 0; i < unitSize; i++)
	{
		cout << 1 << " ";
	}
	return 0;
}

