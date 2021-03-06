// main.cpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.
#include "InputReader.hpp"
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include <fstream>
#include <sstream>
#include <stack>
#include <cmath>
#include <iomanip>
#include "TripReader.hpp"
double time(const RoadSegment & r)
{
	double time = r.miles/r.milesPerHour;
	return time*60.0;
}

std::string convertToMinutesAndSeconds(double time)
{
	std::stringstream ss;
	std::string out = "";
	int iTime = static_cast<int>(time);
	if(iTime >= 60)
	{
		int hr = iTime / 60;
		time -= hr*60;
		iTime -= hr*60;
		double sec = time - iTime;
		int isec = std::round((sec*60)*100);
		ss << hr << " hrs " << iTime << " minutes " << (isec/100.0) << " seconds";
		std::getline(ss, out);
		return out;
	}
	else if(iTime >= 1)
	{
		double sec = time - iTime;
		int isec = std::round((sec*60)*100);
		ss << iTime << " minutes " << (isec/100.0) << " seconds";
		std::getline(ss, out);
		return out;
	}
	else
	{
		ss << std::fixed << std::setprecision(2) << (std::round((time*60)*100)/100) << " seconds";
		std::getline(ss, out);
		return out;
	}
}

std::stack<int> analyzeMap(const RoadMap & r, const std::map<int, int>& shortestMap, int from, int to)
{
	std::stack<int> ret;
	ret.push(to);
	while(ret.top() != from)
	{
		ret.push(shortestMap.find(ret.top())->second);
	}
	return ret;
}

void output(const RoadMap & r, std::stack<int> st, TripMetric c, int from, int to, double& len)
{
	std::ios oldState(nullptr);
	oldState.copyfmt(std::cout);
	if(c == TripMetric::Distance)
		std::cout << "Shortest distance from " << r.vertexInfo(from) << " to " << r.vertexInfo(to) << ":" << std::endl;
	else
		std::cout << "Shortest driving time from " << r.vertexInfo(from) << " to " << r.vertexInfo(to) << ":" << std::endl;

	std::cout << "\tBegin at " <<  r.vertexInfo(st.top()) << std::endl;
	while(st.size() != 1)
	{
		std::cout.copyfmt(oldState);
		int f = st.top();
		st.pop();
		std::cout << "\tContinue to " << r.vertexInfo(st.top()) << " ";
		RoadSegment seg = r.edgeInfo(f, st.top());
		switch(c)
		{
		case TripMetric::Distance:
			std::cout << "(" << seg.miles << " miles)" << std::endl;
			len += seg.miles;
			break;
		case TripMetric::Time:
			std::cout << "(" << seg.miles << " miles @ " << seg.milesPerHour << "mph = " << convertToMinutesAndSeconds(time(seg)) << ")" << std::endl;
			len += time(seg);
			break;
		}
	}
	if(c == TripMetric::Distance)
		std::cout << "Total distance: " << len << " miles" << std::endl << std::endl;
	else
		std::cout << "Total time: " << convertToMinutesAndSeconds(len) << std::endl << std::endl;
}

int main()
{
	RoadMap r_map;
	InputReader input{std::cin};
	RoadMapReader reader;
	TripReader tripReader;
	// RoadMapWriter writer;
	r_map = reader.readRoadMap(input);
	// writer.writeRoadMap(std::cout, r_map);
	if(!r_map.isStronglyConnected())
	{
		std::cout << "Disconnected Map" << std::endl;
		return 0;
	}
	std::vector<Trip> trips = tripReader.readTrips(input); 
	try
	{
		for (Trip & t: trips)
		{
			std::map<int, int> shortestMap;
			std::stack<int> myStack;
			double len = 0.0;
			switch(t.metric)
			{
			case TripMetric::Distance:
				shortestMap = r_map.findShortestPaths(t.startVertex, 
					[](const RoadSegment & e){
						return e.miles;
				});
				myStack = analyzeMap(r_map, shortestMap, t.startVertex, t.endVertex);
				output(r_map, myStack, t.metric, t.startVertex, t.endVertex, len);
				break;
			case TripMetric::Time:
				shortestMap = r_map.findShortestPaths(t.startVertex, 
					[](const RoadSegment & e){
						return time(e);
				});
				myStack = analyzeMap(r_map, shortestMap, t.startVertex, t.endVertex);
				output(r_map, myStack, t.metric, t.startVertex, t.endVertex, len);
				break;
			}
		}
	}
	catch(DigraphException & e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "Different Error" << std::endl;
	}
    return 0;
}

