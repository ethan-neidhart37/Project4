#include "knapsack.h"

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.
{
	int n, b, j, v, c;

	fin >> n;  // read the number of objects
	fin >> b;  // read the cost limit

	numObjects = n;
	costLimit = b;

	value.resize(n);
	cost.resize(n);
	selected.resize(n);

	for (int i = 0; i < n; i++)
	{
		fin >> j >> v >> c;
		value[j] = v;
		cost[j] = c;
		unSelect(j);
	}

	totalValue = 0;
	totalCost = 0;
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor.
{
	int n = k.getNumObjects();

	value.resize(n);
	cost.resize(n);
	selected.resize(n);
	numObjects = k.getNumObjects();
	costLimit = k.getCostLimit();

	totalCost = 0;
	totalValue = 0;

	for (int i = 0; i < n; i++)
	{
		value[i] = k.getValue(i);
		cost[i] = k.getCost(i);
		if (k.isSelected(i))
			select(i);
		else
			unSelect(i);
	}
}

int knapsack::getNumObjects() const
{
	return numObjects;
}

int knapsack::getCostLimit() const
{
	return costLimit;
}

int knapsack::getValue(int i) const
// Return the value of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getValue");

	return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getCost");

	return cost[i];
}

int knapsack::getCost() const
// Return the cost of the selected objects.
{
	return totalCost;
}

int knapsack::getValue() const
// Return the value of the selected objects.
{
	return totalValue;
}

float knapsack::getPriority(int i) const
// Determine priority of an item by its "density" (value/cost)
{
	return (float)value[i] / (float)cost[i];
}

float knapsack::bound()
// Find bound on current knapsack value by solving partial knapsack problem on the empty space in the knapsack
{
	int totalCost = getCost();
	float costBound = getValue();
	vector<int> items = sort();

	for (int i = 0; i < numObjects && costBound < getCostLimit(); i++)
	{
		int item = items[i];
		if (selected[item])
			continue;

		if (cost[item] + totalCost <= costLimit)
		{
			totalCost += cost[item];
			costBound += value[item];
		}
		else
		{
			int diff = costLimit - totalCost;
			float partialValue = diff * getPriority(item);
			totalCost += diff;
			costBound += partialValue;
		}
	}
	return costBound;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
	cout << "------------------------------------------------" << endl;
	cout << "Num objects: " << k.getNumObjects() << " Cost Limit: " << k.getCostLimit() << endl;

	int totalValue = 0;
	int totalCost = 0;

	for (int i = 0; i < k.getNumObjects(); i++)
	{
		totalValue += k.getValue(i);
		totalCost += k.getCost(i);
	}

	cout << "Total value: " << totalValue << endl;
	cout << "Total cost: " << totalCost << endl << endl;

	for (int i = 0; i < k.getNumObjects(); i++)
		cout << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

	cout << endl;

	return ostr;
}

void knapsack::printSolution()
// Prints out the solution.
{
	cout << "------------------------------------------------" << endl;

	cout << "Total value: " << getValue() << endl;
	cout << "Total cost: " << getCost() << endl << endl;

	// Print out objects in the solution
	for (int i = 0; i < getNumObjects(); i++)
		if (isSelected(i))
			cout << i << "  " << getValue(i) << " " << getCost(i) << endl;

	cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;

	return ostr;
}

void knapsack::select(int i)
// Select object i.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::Select");

	if (selected[i] == false)
	{
		selected[i] = true;
		totalCost = totalCost + getCost(i);
		totalValue = totalValue + getValue(i);
	}
}

void knapsack::unSelect(int i)
// unSelect object i.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::unSelect");

	if (selected[i] == true)
	{
		selected[i] = false;
		totalCost = totalCost - getCost(i);
		totalValue = totalValue - getValue(i);
	}
}

int knapsack::partition(vector<int> &items, int left, int right, int pivot)
// Helper for quicksort
// Put every item with a higher priority than the pivot to the left
// Put every item with a lower priority than the pivot to the right
{
	for (int i = left; i < right; i++)
	{
		if (getPriority(items[i]) >= getPriority(pivot))
		{
			swap(items[i], items[left]);
			left++;
		}
	}
	return left - 1;
}

void knapsack::quicksort(vector<int> &items, int left, int right)
// Sorting items from highest to lowest priority using partition and recursion
{
	//partition(k, 0, k.getNumObjects() - 1, items);
	if (left >= right)
		return;

	int middle = left + (right - left) / 2;
	swap(items[middle], items[left]);
	int midpoint = partition(items, left + 1, right, items[left]);
	swap(items[left], items[midpoint]);
	quicksort(items, left, midpoint);
	quicksort(items, midpoint + 1, right);
}

bool knapsack::isSelected(int i) const
// Return true if object i is currently selected, and false otherwise.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getValue");

	return selected[i];
}

vector<bool> knapsack::getSelected()
// Return a vector of selected items
{
	return selected;
}

vector<int> knapsack::sort()
// Public function to return a vector of item numbers, sorted via quicksort
{
	vector<int> items(numObjects, 0);
	for (int i = 0; i < numObjects; i++)
		items[i] = i;

	quicksort(items, 0, numObjects);

	return items;
}