#ifndef WISDOMOFCROWDSHELPER_HPP
#define WISDOMOFCROWDSHELPER_HPP

#include <vector>
#include <mutex>
#include "City.hpp"
#include "SalesmanRoute.hpp"

class WisdomOfCrowdsHelper
{
private:
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
	}
	WisdomOfCrowdsHelper(){};
	
	void GenerateWiseman();
	SalesmanRoute GetWiseman();
};

#endif // WISDOMOFCROWDSHELPER_HPP
