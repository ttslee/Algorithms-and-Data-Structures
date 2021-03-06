#include "StudentWorld.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Initializes the world
int StudentWorld::init()
{
	auto initT = [this](){
		m_ticks = 0;
	};
	initT();
	initNumBould();
	initNumGold();
	initNumOil();
	initProbGoodie();
	m_protesterCount = 0;
	int randX = 0, randY = 0;
	initProtTicks();
	initMaxProt();
	initProbHard();
	m_diggerMan = std::make_shared<DiggerMan>(this);
	fillDirt();
	fillRunawayGrid();
	initEmptySquares();
	for (int i = 0; i < 3; i++)
	{	// Fills StudentWorld with Goodies
		switch (i)
		{
		case 0:
			fillGoodies(randX, randY, boulder);
			break;
		case 1:
			fillGoodies(randX, randY, gold);
			break;
		case 2:
			fillGoodies(randX, randY, oil);
		}
	}
	m_protesters.emplace_back(make_shared<RegularProtester>(this));
	//TESTING DISTANCES BETWEEN GOODIES
	//for (size_t i = 0; i < m_actors.size(); i++)
	//{// Nested for loop checking all actors against eachother
	//	for (size_t j = 0; j < m_actors.size(); j++) 
	//	{
	//		if (i == j)
	//			continue;
	//		cout << distance(m_actors[i]->getX(), m_actors[j]->getX(), m_actors[i]->getY(), m_actors[j]->getY()) << endl; // Outputs distances to console. 
	//		assert(distance(m_actors[i]->getX(), m_actors[j]->getX(), m_actors[i]->getY(), m_actors[j]->getY() > 6));
	//	}
	//}
	return GWSTATUS_CONTINUE_GAME;
}

// 1 tick of the game takes place per move() call
// calls the doSomething() of every actor, protestor, and DiggerMan
int StudentWorld::move()
{
	
	if (!m_diggerMan->isDead())
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		incTicks();
		setGameStatText(formatGameStats());
		if (m_oilCount == 0)
		{
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
		if (m_ticks % m_protTicks == 0 && m_protesterCount < m_maxProt)
		{
			int regOrhard = rand() % 100;
			if (regOrhard < m_probHard)
				m_protesters.emplace_back(make_shared<HardcoreProtester>(this));
			else
				m_protesters.emplace_back(make_shared<RegularProtester>(this));
			m_protesterCount++;
		}
		if (rand() % m_probGoodie == 1)
		{
			if ((rand() % 5 + 1) < 4)
			{
				int spawn = rand() % m_emptySquares.size();
				addActor(make_shared<Water>(this, m_emptySquares[spawn].x, m_emptySquares[spawn].y));
			}
			else
				addActor(make_shared<Sonar>(this, 0, 60));
		}
		m_diggerMan->doSomething();
		for (auto it = m_actors.begin(); it != m_actors.end();)
		{
			if ((*it)->isDead())
				it = m_actors.erase(it);
			else
			{
				(*it)->doSomething();
				it++;
			}
		}
		for (auto it = m_protesters.begin(); it != m_protesters.end();)
		{
			if ((*it)->isDead())
				it = m_protesters.erase(it);
			else
			{
				(*it)->doSomething();
				it++;
			}
		}
		getDiggerMan()->setSonarFlag(false);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(t2 - t1).count();

		std::cout << duration << endl;
		return GWSTATUS_CONTINUE_GAME;
	}
	else
	{
		decLives();
		if (getLives() == 0)
			playSound(SOUND_PLAYER_DIE);
		else
			playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	
}

// clears all vectors in StudentWorld
void StudentWorld::cleanUp()
{
	m_actors.clear();
	m_protesters.clear();
	for (auto &a : m_dirt)
	{
		a.clear();
	}
	m_dirt.clear();
	for (auto & r : m_runGrid)
	{
		r.clear();
	}
	m_runGrid.clear();
	m_emptySquares.clear();
}
// Formats and updates game stats
std::string StudentWorld::formatGameStats() const 
{
	ostringstream os;

	os << left << "Lvl: " << setw(2) << getLevel() << " Hlth: " << setw(3) << m_diggerMan->getHealth() << "% Wtr: "
		<< setw(2) << (m_diggerMan->getNumSquirts() * 10) << " Gld: " << setw(2) << m_diggerMan->getGold()
		<< " Sonar: " << setw(2) << m_diggerMan->getSonarCount() << " Oil Left: " << setw(2) << getOil()
		<< " Scr: " << right <<  setfill('0') << setw(6) << getScore();

	return os.str();
}

// Add an actor to m_actors
void StudentWorld::addActor(std::shared_ptr<Actor> obj)
{
	m_actors.emplace_back(obj);
}

// Fills the grid with goodies depending on argument gdy passed. 
void StudentWorld::fillGoodies(int randX, int randY, ActorType gdy)
{
	switch (gdy)
	{
	case boulder:
	{
		for (int j = 0; j < m_boulderCount; j++)
		{
			if (j == 0)
			{
				do
				{
					randX = rand() % 60;
					randY = ((rand() % 37) + 20);
				} while (randX >= 27 && randX <= 33 && randY > 0);
				m_actors.emplace_back(std::make_shared<Boulder>(this, randX, randY));
			}
			else
			{
				do
				{
					randX = rand() % 60;
					randY = ((rand() % 37) + 20);
					if ((randX >= 27 && randX <= 33) && randY > 0)
						continue;
					size_t b = 0;
					do
					{
						if (distance(randX, m_actors[b]->getX(), randY, m_actors[b]->getY()) > 6)
						{
							b++;
							continue;
						}
						break;
					} while (b < m_actors.size());
					if (b == m_actors.size())
					{
						m_actors.emplace_back(std::make_shared<Boulder>(this, randX, randY));
						break;
					}
				} while (true);
			}
		}
		for (auto e : m_actors)
			std::cout << "BOULDER X: " << e->getX() << "  BOULDER Y: " << e->getY() << endl;
		break;
	}
	case gold:
	case oil:
	{
		int count = 0;
		if (gdy == gold)
			count = m_goldCount;
		else
			count = m_oilCount;
		for (int j = 0; j < count; j++)
		{
			do
			{
				randX = rand() % 60;
				randY = rand() % 57;
				if ((randX >= 27 && randX <= 33) && randY > 0)
					continue;
				size_t b = 0;
				do
				{
					if (distance(randX, m_actors[b]->getX(), randY, m_actors[b]->getY()) > 6)
					{
						b++;
						continue;
					}
					break;
				} while (b < m_actors.size());
				if (b == m_actors.size()) {
					if (gdy == gold)
						m_actors.emplace_back(std::make_shared<Nugget>(this, randX, randY));
					else
						m_actors.emplace_back(std::make_shared<Oil>(this, randX, randY));
					break;
				}
			} while (true);
		}
		break;
	}
	}
}
//Initializes the dirt grid with a mineshaft
void StudentWorld::fillDirt() {
	for (int i = 0; i < 60; i++)
	{
		m_dirt.push_back(std::vector<std::unique_ptr<Dirt>>());
		for (int j = 0; j < 64; j++) 
		{
			if ((i >= 4) && (j >= 30 && j <= 33))
			{
				m_dirt[i].emplace_back(std::make_unique<Dirt>(this, j, i, false, true));
				continue;
			}
			m_dirt[i].emplace_back(std::make_unique<Dirt>(this, j, i));
		}
	}
}
//initializes the runaway grid with correct m_walkFlag values
//This is a parallel grid that is never seen
void StudentWorld::fillRunawayGrid()
{
	for (int i = 0; i < 61; i++)
	{
		m_runGrid.push_back(std::vector<std::shared_ptr<RunawayBox>>());
		for (int j = 0; j < 61; j++)
		{
			if (i == 60)
				m_runGrid[i].emplace_back(std::make_shared<RunawayBox>(j, i, true));
			else if (i >= 4 && j == 30)
				m_runGrid[i].emplace_back(std::make_shared<RunawayBox>(j, i, true));
			else
				m_runGrid[i].emplace_back(std::make_shared<RunawayBox>(j, i, false));
		}
	}
}
//Updates the runaway grid for the current protester that is leaving
//The run grid will change every time a protester dies
void StudentWorld::updateRunGrid(int x, int y, int searchX, int searchY)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	queue<std::shared_ptr<RunawayBox>> run_q;
	run_q.push(m_runGrid[searchY][searchX]);
	while (!run_q.empty())
	{
		std::shared_ptr<RunawayBox> front = run_q.front();
		run_q.pop();
		// up
		if (front->getY() < 60 && m_runGrid[front->getY() + 1][front->getX()]->canWalk())
		{
			if (m_runGrid[front->getY() + 1][front->getX()]->getDistance() >= front->getDistance() || m_runGrid[front->getY() + 1][front->getX()]->getDistance() == 0)
			{
				m_runGrid[front->getY() + 1][front->getX()]->setDistance(front->getDistance() + 1);
				run_q.push(m_runGrid[front->getY() + 1][front->getX()]);
			}
			if (front->getX() == x && front->getY() + 1 == y)
				break;
		}
		// left
		if (front->getX() > 0 && m_runGrid[front->getY()][front->getX() - 1]->canWalk())
		{
			if (m_runGrid[front->getY()][front->getX() - 1]->getDistance() >= front->getDistance() || m_runGrid[front->getY()][front->getX() - 1]->getDistance() == 0)
			{
				m_runGrid[front->getY()][front->getX() - 1]->setDistance(front->getDistance() + 1);
				run_q.push(m_runGrid[front->getY()][front->getX() - 1]);
			}
			if (front->getX() - 1 == x && front->getY() == y)
				break;
		}
		// right
		if (front->getX() < 60 && m_runGrid[front->getY()][front->getX() + 1]->canWalk())
		{
			if (m_runGrid[front->getY()][front->getX() + 1]->getDistance() >= front->getDistance() || m_runGrid[front->getY()][front->getX() + 1]->getDistance() == 0)
			{
				m_runGrid[front->getY()][front->getX() + 1]->setDistance(front->getDistance() + 1);
				run_q.push(m_runGrid[front->getY()][front->getX() + 1]);
			}
			if (front->getX() + 1 == x && front->getY() == y)
				break;
		}
		// down
		if (front->getY() > 0 && m_runGrid[front->getY() - 1][front->getX()]->canWalk())
		{
			if (m_runGrid[front->getY() - 1][front->getX()]->getDistance() >= front->getDistance() || m_runGrid[front->getY() - 1][front->getX()]->getDistance() == 0)
			{
				m_runGrid[front->getY() - 1][front->getX()]->setDistance(front->getDistance() + 1);
				run_q.push(m_runGrid[front->getY() - 1][front->getX()]);
			}
			if (front->getX() == x && front->getY() - 1 == y)
				break;
		}
	}
	m_runGrid[searchY][searchX]->setDistance(0); 
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	std::cout << duration;
}
bool StudentWorld::containsSquare(int xCoord, int yCoord)
{
	for (auto s : m_emptySquares)
	{
		if (s.x == xCoord && s.y == yCoord)
			return true;
	}
	return false;
}
void StudentWorld::initEmptySquares()
{
	for (int i = 4; i <= 56; i++)
	{
		m_emptySquares.emplace_back(30, i);
	}
}

void StudentWorld::initProtTicks()
{
	m_protTicks = max(25, (200 - static_cast<int>(getLevel())));
}
void StudentWorld::initMaxProt()
{
	m_maxProt = min(15, static_cast<int>(2 + getLevel() * 1.5));
}
void StudentWorld::initProbHard()
{
	m_probHard = min(90, static_cast<int>(getLevel()) * 10 + 30);
}
void StudentWorld::initNumBould()
{
	m_boulderCount = min(static_cast<int>(getLevel()) / 2 + 2, 7);
}
void StudentWorld::initNumGold()
{
	m_goldCount = max(5 - static_cast<int>(getLevel()) / 2, 2);
}
void StudentWorld::initNumOil()
{
	m_oilCount = min(2 + static_cast<int>(getLevel()), 18);
}
void StudentWorld::initProbGoodie()
{
	m_probGoodie = getLevel() * 25 + 300;
}
void StudentWorld::incTicks() 
{
	m_ticks++;
}
void StudentWorld::initTicks()
{
	m_ticks = 0;
}
double StudentWorld::distance(int x1, int x2, int y1, int y2)
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
