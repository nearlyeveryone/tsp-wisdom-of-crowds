#include "WisdomOfCrowdsHelper.hpp"
#include <iostream>

using namespace std;

void WisdomOfCrowdsHelper::setWiseman(SalesmanRoute route)
{
	lock_guard<mutex> lockGuard(muteWiseman);
	_wiseman = route;
}

SalesmanRoute WisdomOfCrowdsHelper::GetWiseman()
{
	lock_guard<mutex> lockGuard(muteWiseman);
	return _wiseman;
}

void WisdomOfCrowdsHelper::GenerateWiseman()
{
	//tally most frequently preceding cities for each city
	for(unsigned int i = 0; i < _population.size(); i++)
	{
		
		for(unsigned int j = 0; j < _cities.size(); j++)
		{
			int tempId = _population[i].GetRoute()[j].CityId;
			int index = getCityIndexById(_cities, tempId);
			if(index != -1)
			{
				if(j != _cities.size() -1)
				{
					_cities[index].IncrementCityCounter(_population[i].GetRoute()[j+1].CityId, _cities.size());
				} else {
					_cities[index].IncrementCityCounter(_population[i].GetRoute()[0].CityId, _cities.size());
				}
				
			}
		}
	}
	
	//start with arbitrary city
	vector<City> newRoute;
	vector<City> remainingCities = _cities;
	newRoute.push_back(remainingCities[rand() % _cities.size()]);
		
	//add the most selected next city that isnt currently in the route.
	for(unsigned int i = 0; i < _cities.size() - 1; i++)
	{
		//find the next candidate to be added
		int bestCityId;
		int bestCount = -1;
		for(unsigned int j = 0; j < _cities.size(); j++)
		{
			if(newRoute[i].CityCounters[j].count > bestCount)
			{
				//city isnt already in the route, this is the part that makes the algorithm greedy,
				//it selects the 1st,2nd...jth best city to insert
				if(getCityIndexById(newRoute, newRoute[i].CityCounters[j].cityId) == -1)
				{
					bestCityId = newRoute[i].CityCounters[j].cityId;
					bestCount = newRoute[i].CityCounters[j].count;
				}
			}
		}
		newRoute.push_back(_cities[getCityIndexById(_cities, bestCityId)]);
	}
	
	SalesmanRoute wiseRoute(newRoute);
	
	cout << "wiseman d = "<< wiseRoute.GetTotalDistance() << endl;
	setWiseman(wiseRoute);
}

int WisdomOfCrowdsHelper::getCityIndexById(const vector<City>& cities, int Id)
{
	for(unsigned int i = 0; i < cities.size(); i++)
	{
		if(cities[i].CityId == Id)
			return i;
	}
	
	return -1;
}