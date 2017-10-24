#ifndef SALESMANROUTE_HPP
#define SALESMANROUTE_HPP

#include <vector>
#include "City.hpp"

using namespace std;

class SalesmanRoute
{
    private:
        vector<City> _route;
    public:
		SalesmanRoute() {};
		SalesmanRoute(vector<City> cities): _route(cities) {};
        double GetTotalDistance() const;
		double GetDistanceToCity(int firstIndex, int secondIndex) const;
        void AddCityToRoute(City city);
		void InsertCityInRoute(City city, int index);
		void Mutate(int timesToMutate);
		SalesmanRoute CrossoverSplit(SalesmanRoute mate);
		vector<City> GetRoute();
		
		bool operator <(const SalesmanRoute &rhs);
};

#endif // SALESMANROUTE_HPP
