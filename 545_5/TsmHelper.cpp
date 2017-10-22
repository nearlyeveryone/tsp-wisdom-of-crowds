#include "TsmHelper.hpp"

using namespace std;

TsmHelper::TsmHelper(vector<City> cities)
{
	_bestRoute = SalesmanRoute(cities);
	addPointToGraph(0, GetBestRoute().GetTotalDistance());
}

TsmHelper::~TsmHelper()
{
}

SalesmanRoute TsmHelper::GetBestRoute()
{
	lock_guard<mutex> lockGuard(muteRoute);
	return _bestRoute;
}
void TsmHelper::setBestRoute(SalesmanRoute route)
{
	lock_guard<mutex> lockGuard(muteRoute);
	_bestRoute = route;
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

void TsmHelper::GeneticStatistics(int sampleSize, int populationSize, int generations, bool reverseCrossover, bool adaptiveMutator)
{
	vector<SalesmanRoute> routes;
	struct RouteStatistics routeStatistics;
	
	//collect data
	for(int i = 0; i < sampleSize; i++)
	{
		GeneticAlgorithm(populationSize, generations, reverseCrossover, adaptiveMutator);
		if(routeStatistics.max == -1 || GetBestRoute().GetTotalDistance() > routeStatistics.max)
		{
			routeStatistics.max = GetBestRoute().GetTotalDistance();
		}
		if(routeStatistics.min == -1 || GetBestRoute().GetTotalDistance() < routeStatistics.min)
		{
			routeStatistics.min = GetBestRoute().GetTotalDistance();
		}
		routes.push_back(GetBestRoute());
	}
	
	//calculate std deviation and average
	double sum = 0.0, mean, standardDeviation = 0.0;
	for(unsigned int i = 0; i < routes.size(); i++)
	{
		sum += routes[i].GetTotalDistance();
	}
	
	mean = sum/routes.size();
	//set average
	routeStatistics.average = mean;
	
	for(unsigned int i = 0; i < routes.size(); i++)
	{
		standardDeviation += pow(routes[i].GetTotalDistance() - mean, 2);
	}
	//set standard deviation
	routeStatistics.stdDeviation = sqrt(standardDeviation/routes.size());
	
	routeStats = routeStatistics;
}

void TsmHelper::GeneticAlgorithm2(vector<SalesmanRoute> initalPopulation, int populationSize, int generations, bool reverseCrossover, bool adaptiveMutator)
{
	vector<SalesmanRoute> population = initalPopulation;
	
	srand(time(NULL));
	
	setCurrentGeneration(0);
	setBestRoute(population[0]);
	clearGraphPoints();
	addPointToGraph(0, GetBestRoute().GetTotalDistance());
	
	int mutationRate = (GetBestRoute().GetRoute().size()/20)+1;
	SalesmanRoute overallBestRoute;
	double previousDistance = -1;
	double generationsSinceChange = 0;
	
	for(int i = 0; i < generations; i++)
	{
		vector<SalesmanRoute> children;
		
		previousDistance = population[0].GetTotalDistance();
		//make offspring and add to population
		for(int j = 0; j < populationSize; j++)
		{
			int rt1 = rand() % populationSize;
			int rt2 = rand() % populationSize;
			
			SalesmanRoute offspring = population[rt1].CrossoverSplit(population[rt2], reverseCrossover);
			
			children.push_back(offspring);
		}

		//take ~75% of the children, and ~25% of the top of the previous generation
		children.resize(populationSize-populationSize/4 + 1);
		children.insert(children.end(), population.begin(), population.begin() + populationSize/4 + 1);
		
		//sort by fitness to ensure that the top 25% is taken in the next generation
		population = children;
		
		if(adaptiveMutator)
		{
			if(previousDistance == population[0].GetTotalDistance())
			{
				generationsSinceChange++;
			}
			else if(generationsSinceChange > 0)
			{
				generationsSinceChange = 0;
			}
			if(generationsSinceChange >= 20)
			{
				//nuke population
				for(unsigned int j = 5; j < population.size(); j++)
				{
					population[j].Mutate(mutationRate*generationsSinceChange);
				}
			}
		}
		else {
			for(unsigned int j = 0; j < population.size()/20+1; j++)
			{
				population[rand() % population.size()].Mutate(mutationRate);
			}
		}
		
		sort(population.begin(), population.end());
		
		if(GetBestRoute().GetTotalDistance() > population[0].GetTotalDistance())
		{
			setBestRoute(population[0]);
		}
		setCurrentGeneration(i+1);
		addPointToGraph(i+1, population[0].GetTotalDistance());
	}
	
	setJobFinished(true);
}

void TsmHelper::GeneticAlgorithm(int populationSize, int generations, bool reverseCrossover, bool adaptiveMutator)
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
	
	GeneticAlgorithm2(initalPopulation, populationSize, generations, reverseCrossover, adaptiveMutator);
}