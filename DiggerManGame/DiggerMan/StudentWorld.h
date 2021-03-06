#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "Actor.h"
#include <ctime>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class RunawayBox
{
public:
	RunawayBox(int xCoord, int yCoord, bool canWalk) :
		x(xCoord), 
		y(yCoord), 
		m_walkFlag(canWalk), 
		distanceFromStart(0)
	{
	}
	int getX(){ return x; }
	int getY(){ return y; }
	bool canWalk(){ return m_walkFlag; }
	int getDistance() { return distanceFromStart; }

	void setDistance(int dis){ distanceFromStart = dis; }
	void setWalkFlag(bool flag){ m_walkFlag = flag; }
private:
	int x;
	int y;
	bool m_walkFlag;
	int distanceFromStart;
};
struct EmptySquare
{
	EmptySquare(int xCoord, int yCoord) : x(xCoord), y(yCoord)
	{
	}
	int x;
	int y;
};
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_ticks(1)
	{
		srand(static_cast<unsigned int>(time(nullptr)));
	}
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	std::string formatGameStats() const;
	void addActor(std::shared_ptr<Actor> obj);

	bool containsSquare(int, int);
	std::vector<EmptySquare>						*getEmptySquares()
	{
		return &m_emptySquares;
	}
	std::shared_ptr<DiggerMan>						getDiggerMan()
	{
		return m_diggerMan;
	}
	std::vector<std::shared_ptr<Protester>>			getProtesters()	
	{ 
		return m_protesters; 
	}
	std::vector<std::shared_ptr<Actor>>				getActors()		
	{
		return m_actors; 
	}
	std::vector<std::vector<std::unique_ptr<Dirt>>> *getDirt()		
	{ 
		return &m_dirt; 
	}
	std::vector<std::vector<std::shared_ptr<RunawayBox>>> *getRunawayGrid()
	{
		return &m_runGrid;
	}
	const int getDiggerHealth() const										
	{
		return m_diggerMan->getHealth(); 
	}
	const int getNumSquirtsLeft() const									
	{
		return m_diggerMan->getNumSquirts(); 
	}
	const int getTicks() const											
	{
		return m_ticks; 
	}
	const int getBoulder() const
	{
		return m_boulderCount;
	}
	const int getGold() const
	{
		return m_goldCount;
	}
	const int getOil() const
	{
		return m_oilCount;
	}
	void updateRunGrid(int x, int y, int searchX, int searchY);
	void decOil()													
	{ 
		m_oilCount--;
	}
	void setMaxProt();
	void initEmptySquares();
	void initNumBould();
	void initNumGold();
	void initNumOil();
	void initProbGoodie();
	void initTicks();
	void decProtCount(){ m_protesterCount--; }
	double distance(int x1, int x2, int y1, int y2);

private:
	void fillGoodies(int, int, ActorType);
	void fillDirt();
	void initProtTicks();
	void incTicks();
	void initMaxProt();
	void initProbHard();
	void fillRunawayGrid();

	std::vector<EmptySquare>						m_emptySquares;
	std::vector<std::shared_ptr<Actor>>				m_actors;
	std::vector<std::shared_ptr<Protester>>			m_protesters;
	std::shared_ptr<DiggerMan>						m_diggerMan;
	std::vector<std::vector<std::unique_ptr<Dirt>>>	m_dirt;
	std::vector<std::vector<std::shared_ptr<RunawayBox>>> m_runGrid;
	int m_oilCount;
	int m_boulderCount;
	int m_goldCount;
	int	m_ticks;
	int m_maxProt;
	int	m_protTicks;
	int	m_probHard;
	int m_protesterCount;
	int m_probGoodie;
};
#endif // STUDENTWORLD_H_
