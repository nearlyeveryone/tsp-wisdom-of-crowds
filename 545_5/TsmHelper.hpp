#ifndef TSMHELPER_HPP
#define TSMHELPER_HPP

#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

#include "City.hpp"
#include "SalesmanRoute.hpp"

struct GraphPoint
{
	int generation;
	double distance;
};

struct RouteStatistics
{
	double min = -1;
	double max = -1;
	double average = -1;
	double stdDeviation = -1;
};

class TsmHelper
{
private:
	vector<SalesmanRoute> _population;
	mutex muteRoute;
	int _currentGeneration = 0;
	mutex muteGeneration;
	vector<GraphPoint> generationGraph;
	mutex muteGraph;
	bool jobFinished = false;
	mutex muteJob;
	void setPopulation(vector<SalesmanRoute> population);
	void setCurrentGeneration(int generation);
	void addPointToGraph(int generation, double distance);
	void clearGraphPoints();
	void setJobFinished(bool state);
	
public:
	TsmHelper();
	TsmHelper(vector<City>);
	SalesmanRoute GetBestRoute();
	vector<SalesmanRoute> GetPopulation();
	int GetCurrentGeneration();
	vector<GraphPoint> GetGenerationGraph();
	void GeneticAlgorithm(int populationSize, int generations);
	void GeneticAlgorithm2(vector<SalesmanRoute> initalPopulation, int populationSize, int generations);
	void GeneticStatistics(int sampleSize, int populationSize, int generations);
	bool IsJobFinished();
	struct RouteStatistics routeStats;
	~TsmHelper(); 

};

#endif // TSMHELPER_HPP
