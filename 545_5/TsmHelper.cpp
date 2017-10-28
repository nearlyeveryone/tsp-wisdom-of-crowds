#include "TsmHelper.hpp"

using namespace std;

TsmHelper::TsmHelper(vector<City> cities)
{
	SalesmanRoute initalRoute(cities);
	vector<SalesmanRoute> initalPopulation;
	initalPopulation.push_back(initalRoute);
	_population = initalPopulation;
	addPointToGraph(0, GetBestRoute().GetTotalDistance());
}

TsmHelper::~TsmHelper()
{
}

SalesmanRoute TsmHelper::GetBestRoute()
{
	lock_guard<mutex> lockGuard(muteRoute);
	return _population[0];
}
vector<SalesmanRoute> TsmHelper::GetPopulation()
{
	lock_guard<mutex> lockGuard(muteRoute);
	return _population;
}

void TsmHelper::setPopulation(vector<SalesmanRoute> population)
{
	lock_guard<mutex> lockGuard(muteRoute);
	_population = population;
}

int TsmHelper::GetCurrentGeneration()
{
	lock_guard<mutex> lockGuard(muteGeneration);
	return _currentGeneration;
}
void TsmHelper::setCurrentGeneration(int generation)
{
	lock_guard<mutex> lockGuard(muteGeneration);
	_currentGeneration = generation;
}
void TsmHelper::clearGraphPoints()
{
	lock_guard<mutex> lockGuard(muteGeneration);
	generationGraph.clear();
}
vector<GraphPoint> TsmHelper::GetGenerationGraph()
{
	lock_guard<mutex> lockGuard(muteGraph);
	return generationGraph;
}
void TsmHelper::addPointToGraph(int generation, double distance)
{
	struct GraphPoint point;
	point.generation = generation;
	point.distance = distance;
	lock_guard<mutex> lockGuard(muteGraph);
	generationGraph.push_back(point);
}
void TsmHelper::setJobFinished(bool state)
{
	lock_guard<mutex> lockGuard(muteJob);
	jobFinished = state;
}
bool TsmHelper::IsJobFinished()
{
	lock_guard<mutex> lockGuard(muteJob);
	return jobFinished;
}

void TsmHelper::GeneticAlgorithm2(vector<SalesmanRoute> initalPopulation, int populationSize, int generations)
{
	vector<SalesmanRoute> population = initalPopulation;
	
	setCurrentGeneration(0);
	setPopulation(population);
	clearGraphPoints();
	addPointToGraph(0, GetBestRoute().GetTotalDistance());
	
	int mutationRate = (GetBestRoute().GetRoute().size()/20)+1;
	
	for(int i = 0; i < generations; i++)
	{
		vector<SalesmanRoute> children;
		
		//make offspring and add to population
		for(int j = 0; j < populationSize; j++)
		{
			int rt1 = rand() % populationSize;
			int rt2 = rand() % populationSize;
			
			SalesmanRoute offspring = population[rt1].CrossoverSplit(population[rt2]);
			
			children.push_back(offspring);
		}
		
		
		//take ~75% of the children, and ~25% of the top of the previous generation
		children.resize(populationSize-populationSize/4 + 1);
		children.insert(children.end(), population.begin(), population.begin() + populationSize/4 + 1);
		
		//sort by fitness to ensure that the top 25% is taken in the next generation
		population = children;
		
		//mutate population
		for(unsigned int j = 0; j < population.size()/20+1; j++)
		{
			population[rand() % population.size()].Mutate(mutationRate);
		}
		

		sort(population.begin(), population.end());
		
		setPopulation(population);
		setCurrentGeneration(i+1);
		addPointToGraph(i+1, population[0].GetTotalDistance());
	}
	
	setJobFinished(true);
}

void TsmHelper::GeneticAlgorithm(int populationSize, int generations)
{
	vector<City> cities = GetBestRoute().GetRoute();
	
	vector<SalesmanRoute> initalPopulation;
	for(int i = 0; i < populationSize; i++)
	{
		SalesmanRoute initalRoute;
		random_shuffle(cities.begin(), cities.end());
		initalRoute = SalesmanRoute(cities);
		initalPopulation.push_back(initalRoute);
	}
	
	GeneticAlgorithm2(initalPopulation, populationSize, generations);
}