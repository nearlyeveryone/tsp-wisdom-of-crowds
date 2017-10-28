#ifndef WISDOMOFCROWDSHELPER_HPP
#define WISDOMOFCROWDSHELPER_HPP

#include <vector>
#include <mutex>
#include <iostream>
#include <algorithm>
#include "City.hpp"
#include "SalesmanRoute.hpp"

struct WiseStatistics
{
	double min = -1;
	double max = -1;
	double average = -1;
	double stdDeviation = -1;
};

class WisdomOfCrowdsHelper
{
private:
	bool tallied = false;
	vector<City> _cities;
	vector<SalesmanRoute> _population;
	SalesmanRoute _wiseman;
	void setWiseman(SalesmanRoute);
	mutex muteWiseman;
	int getCityIndexById(const vector<City>& cities, int Id);
public:
	WisdomOfCrowdsHelper(vector<SalesmanRoute> population): _population(population)
	{
		_cities = population[0].GetRoute();
		_wiseman = SalesmanRoute(_cities);
	}
	WisdomOfCrowdsHelper(){};
	
	void GenerateWiseman();
	SalesmanRoute GetWiseman();
	void WisdomStatistics(int sampleSize);
	struct WiseStatistics wiseStats;
	vector<SalesmanRoute> _wisemans;
	void TallyCities();
};

#endif // WISDOMOFCROWDSHELPER_HPP
