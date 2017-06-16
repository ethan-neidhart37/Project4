// File: p4.cpp
// Authors: Kevin Do, Ethan Neidhart
// Project 4: Solving knapsack using branch and bound

#include "p4.h"

using namespace std;

void branchAndBound(knapsack &k, int time)
// Branch and bound algorithm to solve the knapsack problem.
// Branching is binary: one branch to take the "next" object, one branch to skip it
// Should this run on a sorted list?
{
	clock_t startTime = clock();
	knapsack incumbent(k);
	vector<int> items = k.sort();
	stack<knapsack> nodes;
	nodes.push(k);

	cout << "Cost Bound: " << k.getCostLimit() << endl;
	cout << "Items:\n";
	for (int i = 0; i < k.getNumObjects(); i++)
	{
		cout << items[i] << "\t" << k.getValue(items[i]) << "\t" << k.getCost(items[i]) << endl;
	}
	cout << endl;

	while (!nodes.empty() && (clock() - startTime) / CLOCKS_PER_SEC < time)
	{
		knapsack current = nodes.top();
		nodes.pop();
		bool isFathomed = current.isFathomed(incumbent.getValue());

		// If node is fathomed, legal, and better than incumbent, it becomes the new incumbent
		if (isFathomed && current.getCost() <= current.getCostLimit() && current.getValue() > incumbent.getValue())
		{
			cout << "Fathomed Incumbent\n";
			cout << "----------------------------\n";
			cout << "Old Incumbent: " << incumbent.getValue() << endl;
			cout << "Value: " << current.getValue() << endl;
			cout << "Selected Items: ";
			for (int i = 0; i < current.getNumObjects(); i++)
			{
				if (current.isSelected(items[i]))
					cout << items[i] << ", ";
			}
			cout << endl;

			incumbent = current;

			cout << "New Incumbent:\n";
			cout << "Value: " << incumbent.getValue() << endl;
			cout << "Selected Items: ";
			for (int i = 0; i < incumbent.getNumObjects(); i++)
			{
				if (current.isSelected(items[i]))
					cout << items[i] << ", ";
			}
			cout << endl << endl;
		}
		// If node is fathomed and not new incumbent, ignore it
		// If node is not fathomed, branch on next item
		else if (!isFathomed)
		{
			int item = items[current.getCurrentItem()];
			knapsack yes(current);
			knapsack no(current);

			cout << "Unfathomed Node\n";
			cout << "----------------------------\n";
			cout << "Incumbent: " << incumbent.getValue() << endl;
			cout << "Current Item: " << item << ", bound: " << current.getBound() << ", value: " << current.getValue() << endl;
			cout << "Selected Items: ";
			for (int i = 0; i < current.getNumObjects(); i++)
			{
				if (current.isSelected(items[i]))
					cout << items[i] << ", ";
			}
			cout << endl;

			// NOTE: I don't like those copy constructors. Pretty sure they're costing us O(n) each, every single iteration...
			// Is it necessary to do that? I'd like to avoid all that copying if possible...
			// But I think we need it so that we actually have separate objects. Maybe we only have to copy one of them...
			// But that really doesn't solve the problem. Thoughts?

			yes.select(item);
			yes.nextItem();
			no.nextItem();

			// Error checking...
			if (yes.getCurrentItem() != no.getCurrentItem() || yes.getCurrentItem() - current.getCurrentItem() != 1)
			{
				cout << "Error on " << current.getCurrentItem() << ":\n";
				cout << "Yes = " << yes.getCurrentItem() << ", No = " << no.getCurrentItem() << endl;
			}

			if (!yes.isSelected(item) || no.isSelected(item))
			{
				cout << "Error on " << current.getCurrentItem() << ":\n";
				cout << "Yes = " << yes.isSelected(item) << ", No = " << no.isSelected(item) << endl;
			}

			if (yes.bound(yes.getCurrentItem()) >= no.bound(no.getCurrentItem()))
			{
				nodes.push(no);
				nodes.push(yes);
			}
			else
			{
				nodes.push(yes);
				nodes.push(no);
			}

			cout << "Child Node: yes\n";
			cout << "Incumbent: " << incumbent.getValue() << endl;
			cout << "Current Item: " << items[yes.getCurrentItem() - 1] << ", bound: " << yes.getBound() << ", value: " << yes.getValue() << endl;
			cout << "Selected Items: ";
			for (int i = 0; i < yes.getNumObjects(); i++)
			{
				if (yes.isSelected(items[i]))
					cout << items[i] << ", ";
			}
			cout << endl;

			cout << "Child Node: no\n";
			cout << "Incumbent: " << incumbent.getValue() << endl;
			cout << "Current Item: " << items[no.getCurrentItem() - 1] << ", bound: " << no.getBound() << ", value: " << no.getValue() << endl;
			cout << "Selected Items: ";
			for (int i = 0; i < no.getNumObjects(); i++)
			{
				if (no.isSelected(items[i]))
					cout << items[i] << ", ";
			}
			cout << endl << endl;

		}
	}

	k = incumbent;
}

void greedyKnapsack(knapsack &k)
// Greedy algorithm to solve knapsack problem by grabbing highest priority items that will fit
{
	int limit = k.getCostLimit();
	int cost = 0;
	vector<int> items = k.sort();

	// The first item in this list now contains the item number of the highest priority knapsack item

	for (int i = 0; i < k.getNumObjects(); i++)
	{
		int item = items[i];

		if (cost + k.getCost(item) <= limit)
		{
			k.select(item);
			cost += k.getCost(item);
		}

		if (cost == limit)
			return;
	}
}

void knapsackOutput(knapsack & k)
// Writes the results of the algorithm to an output file
{
	string fileName = "knapsack/output/knapsack" + to_string(k.getNumObjects()) + ".output";

	ofstream myfile;
	myfile.open(fileName);

	myfile << "Number of Objects: " << k.getNumObjects() << endl;
	myfile << "Total value: " << k.getValue() << endl;
	myfile << "Total cost: " << k.getCost() << endl;

	// Print out objects in the solution
	for (int i = 0; i < k.getNumObjects(); i++)
	{
		if (k.isSelected(i))
		{
			// Some unnecessary formatting to make output file look nicer
			if (i < 10)
			{
				myfile << i << "   " << k.getValue(i) << " " << k.getCost(i) << endl;
			}
			else
			{
				myfile << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;
			}
		}
	}
	myfile << endl;
	myfile.close();
}

void knapsackRun()
// Runs an algorithm to solve the knapsack problem
{
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	// fileName = "knapsack/input/knapsack16.input";

	cout << "Enter filename" << endl;
	cin >> fileName;
	string filePath = "knapsack/input/" + fileName + ".input";

	fin.open(filePath.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		knapsack k(fin);

		//exhaustiveKnapsack(k, 600);
		//greedyKnapsack(k);
		branchAndBound(k, 600);

		// Write solution to output file
		knapsackOutput(k);

		cout << endl << "Best solution" << endl;
		k.printSolution();
	}

	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}

	fin.close();

	// loop input all files

	/*
	vector<string> s;
	s.push_back("knapsack/input/knapsack8.input");
	s.push_back("knapsack/input/knapsack12.input");
	s.push_back("knapsack/input/knapsack16.input");
	s.push_back("knapsack/input/knapsack20.input");
	s.push_back("knapsack/input/knapsack28.input");
	s.push_back("knapsack/input/knapsack32.input");
	s.push_back("knapsack/input/knapsack48.input");
	s.push_back("knapsack/input/knapsack64.input");
	s.push_back("knapsack/input/knapsack128.input");
	s.push_back("knapsack/input/knapsack256.input");
	s.push_back("knapsack/input/knapsack512.input");
	s.push_back("knapsack/input/knapsack1024.input");

	for (int i = 0; i < s.size(); i++)
	{
		//cout << "Enter filename" << endl;
		//cin >> fileName;
		fileName = s[i];
		fin.open(fileName.c_str());
		if (!fin)
		{
			cerr << "Cannot open " << fileName << endl;
			exit(1);
		}

		try
		{
			cout << "Reading knapsack instance for " << s[i] << endl;

			knapsack k(fin);

			//exhaustiveKnapsack(k, 600);
			greedyKnapsack(k);

			// Write solution to output file
			knapsackOutput(k);

			cout << endl << "Best solution" << endl;
			k.printSolution();

			// Pause to view results
			system("PAUSE");
		}

		catch (indexRangeError &ex)
		{
			cout << ex.what() << endl; exit(1);
		}
		catch (rangeError &ex)
		{
			cout << ex.what() << endl; exit(1);
		}

		fin.close();
	}
	*/
}
