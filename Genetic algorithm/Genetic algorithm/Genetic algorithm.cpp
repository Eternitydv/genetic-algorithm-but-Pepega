#include "stdafx.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <time.h>
#include <vector>

const double e = M_E;

class Point
{
private:
	double x;
	double y;
	double z;
	double fitness;
public:

	double getZ()
	{
		z = pow(e, (x + y))*(x*x + 2 * y*y);
		//z = y * sqrt(x) - 2 * y*y + 14 * y - x; //only max here
		//z = 3 * x * x + x * y + 2 * y * y - x - 4 * y;
		//z = sin(x)*cos(y);
		return z;
	}
	void setCoordinates(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	double getX()
	{
		return x;
	}
	double getY()
	{
		return y;
	}
	void countFitness()
	{
		fitness = this->getZ();
	}
	double getFitness()
	{
		return fitness;
	}
	void mutate()
	{
		x += (double)((rand() % 11) - 5) / 100;
		y += (double)((rand() % 11) - 5) / 100;
	}
	Point crossover(Point p2, int k)
	{
		Point child;
		if (k % 2 == 0)
			child.setCoordinates(x, p2.getX());
		else
			child.setCoordinates(p2.getY(), y);
		return child;
	}

};

typedef std::vector<Point> PopulationVector;

PopulationVector createPopulation(int n)
{
	PopulationVector population;
	Point p;
	double x, y;
	for (int i = 0; i < n; i++)
	{
		x = (double)((rand() % 11) - 2);
		y = (double)((rand() % 11) - 1.5);
		p.setCoordinates(x, y);
		population.push_back(p);
	}
	return population;
}

void sortPopulationAscending(PopulationVector &p, int n)
{
	Point temp;
	int k = 0;
	int j = 0;
	for (j = 0; j < n - 1; j++)
	{
		for (k = j + 1; k < n; k++)
		{
			if (p[j].getFitness() < p[k].getFitness())
			{
				temp = p[j];
				p[j] = p[k];
				p[k] = temp;
			}
		}
	}
}

void sortPopulationDescending(PopulationVector &p, int n)
{
	Point temp;
	int k = 0;
	int j = 0;
	for (j = 0; j < n - 1; j++)
	{
		for (k = j + 1; k < n; k++)
		{
			if (p[j].getFitness() > p[k].getFitness())
			{
				temp = p[j];
				p[j] = p[k];
				p[k] = temp;
			}
		}
	}
}

void printPopulation(PopulationVector population, int PopulationNumber)
{
	int i = 0;
	for (i = 0; i < PopulationNumber; i++)
	{
		std::cout << "point" << i << " x: " << population[i].getX() << " y: " << population[i].getY() << " z: " << population[i].getZ() << " fitness: " << population[i].getFitness() << std::endl;
	}
}

PopulationVector evolution(PopulationVector &population, int IterationNumber, int PopulationNumber, int cutoff, bool min, bool max)
{
	
	int i = 0;
	int j = 0;
	double sumOfFi = 0;
	Point temp;
	PopulationVector nextPopulation;
	for (i = 0; i < IterationNumber; i++);
	{
		for (j = 0; j < PopulationNumber; j++)
		{
			population[j].countFitness();
		}
		if(min)
			sortPopulationDescending(population, PopulationNumber);
		if (max)
			sortPopulationAscending(population, PopulationNumber);

		population.erase(population.end() - cutoff, population.end());
		
		for (j = 0; j < PopulationNumber; j++)
		{
			int parent1 = (int)(rand() % population.size());
			int parent2 = (int)(rand() % population.size());
			temp = population[parent1].crossover(population[parent2], j);
			if (j%4 == 0)
				temp.mutate();
			nextPopulation.push_back(temp);
		}
		population = nextPopulation;
		nextPopulation.clear();
	}
	return population;
}

int main()
{
	srand(time(0));

	int populationNumber = 50;
	int iterationNumber = 3000;
	int cutoff = 10;
	PopulationVector findmin = createPopulation(populationNumber);
	PopulationVector findmax = createPopulation(populationNumber);
	evolution(findmax, iterationNumber, populationNumber, cutoff, 0, 1);
	evolution(findmin, iterationNumber, populationNumber, cutoff, 1, 0);
	for (int j = 0; j < populationNumber; j++)
	{
		findmin[j].countFitness();
		findmax[j].countFitness();
	}
	sortPopulationDescending(findmin, findmax.size());
	sortPopulationAscending(findmax, findmax.size());

	printPopulation(findmax, findmax.size());
	std::cout << "\n";
	for (int i = 0; i < 40; i++)
	{
		std::cout << "-";
	}
	std::cout << std::endl;
	std::cout << "best guess for a minimum: x: " << round(findmin[0].getX()) << " y: " << round(findmin[0].getY()) << " z: " << round(findmin[0].getZ()) << " fitness: " << findmin[0].getFitness() << std::endl;
	std::cout << "best guess for a maximum: x: " << round(findmax[0].getX()) << " y: " << round(findmax[0].getY()) << " z: " << round(findmax[0].getZ()) << " fitness: " << findmax[0].getFitness() << std::endl;
	system("Pause");
	return 0;
}
