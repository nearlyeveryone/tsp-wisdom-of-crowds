#include "SalesmanRoute.hpp"
#include <cstdlib>
#include <algorithm>
#include <ctime>

bool SalesmanRoute::operator <(const SalesmanRoute &rhs)
{
	return this->GetTotalDistance() < rhs.GetTotalDistance();
}

double SalesmanRoute::GetTotalDistance() const
{
    double totalDistance = 0;
    for(unsigned int i = 0; i < _route.size() - 1; i++)
    {
		//visit intermediate cities
        totalDistance = totalDistance + _route[i].DistanceTo(_route[i+1]);
    }
	//return home
	totalDistance = totalDistance + _route[0].DistanceTo(_route[_route.size()-1]);
	
	
	return totalDistance;
}

double SalesmanRoute::GetDistanceToCity(int firstIndex, int secondIndex) const
{
    double totalDistance = 0;
    for(int i = firstIndex; i < secondIndex; i++)
    {
		//visit intermediate cities
        totalDistance = totalDistance + _route[i].DistanceTo(_route[i+1]);
    }
	//return home
	totalDistance = totalDistance + _route[0].DistanceTo(_route[_route.size()-1]);
	
	
	return totalDistance;
}

void SalesmanRoute::AddCityToRoute(City city)
{
    _route.push_back(city);
		
}
void SalesmanRoute::InsertCityInRoute(City city, int index)
{
    _route.insert(_route.begin()+index, city);
}

vector<City> SalesmanRoute::GetRoute()
{
	return _route;
}

void SalesmanRoute::Mutate(int timesToMutate)
{
	
	for(int i = 0; i < timesToMutate; i++)
	{
		int pos1 = rand() % _route.size();
		int pos2 = rand() % _route.size();
		
		iter_swap(_route.begin() + pos1, _route.begin() + pos2);
	}
}

SalesmanRoute SalesmanRoute::CrossoverSplit(SalesmanRoute mate)
{	
	SalesmanRoute child;
	
	//populate child with fake cities for later insertion
	for(unsigned int i = 0; i < this->_route.size(); i++)
	{
		City fakeCity(-1,-1,-1); 
		child._route.push_back(fakeCity);
	}
	
	//find shortest subsegment
	unsigned int segmentLength = (rand() % this->_route.size())/2 +this->_route.size()/20;
	unsigned int offset = (rand() % this->_route.size()) - segmentLength;
	
	unsigned int start = offset;
	unsigned int end = segmentLength + offset;
	
	//insert the cities between the start and and the end of the first parent
	for(unsigned int i = start; i < end; i++)
	{
			child._route[i] = this->_route[i];
	}
	
	//add the missing cities in the childs tour in the order that they come from the second parent
	for(unsigned int i = 0; i < mate._route.size(); i++)
	{
		bool childContainsCity = false;
		
		for(unsigned int j = 0; j < child._route.size(); j++)
		{
			if(child._route[j].CityId == mate._route[i].CityId)
				childContainsCity = true;
		}
		if(!childContainsCity)
		{
			for(unsigned int k = 0; k < child._route.size(); k++)
			{
				if(child._route[k].CityId == -1)
				{
					child._route[k] = mate._route[i];
					break;
				}
			}
		}
	}
	
	return child;
}
