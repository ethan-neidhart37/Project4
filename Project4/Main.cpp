// Project 4
// Contains code to run p4.cpp

#include <iostream>
#include "p4.h"

int main() {
	// Run p4.cpp

	int num;
	char select;
	bool loopEnd = false;

	while (loopEnd == false)
	{
		/*do
		{
			std::cout << "[1] knapsack \n[2] graph coloring \n[3] quit\n";
			std::cout << "Select which project problem to run: ";
			std::cin >> num;

			switch (num)
			{
			case 1: knapsackRun(); break;
			case 2: graphColoring(); break;
			case 3: exit(0);
			default: std::cout << "Please select correct input. [1/2/3]"; break;
			}
		} while (num != 1 && num != 2);*/
		std::cout << "Running knapsack solver..." << endl;
		knapsackRun();

		do
		{
			std::cout << "\nRun knapsack again? [y/n]: ";
			std::cin >> select;

			switch (select)
			{
			case 'y': loopEnd = false; break;
			case 'n': loopEnd = true; break;
			default: std::cout << "Please select correct input '[y/n]"; break;
			}
			std::cout << "\n";
		} while (select != 'y' && select != 'n');
	}

	std::cin.get();
	return 0;
}