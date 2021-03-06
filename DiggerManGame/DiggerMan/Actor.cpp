#include "StudentWorld.h" // Actor.h is included in StudentWorld.h

double Actor::distanceFromActor(std::shared_ptr<Actor> a)
{
	return sqrt(pow(getX() - a->getX(), 2) + pow(getY() - a->getY(), 2));
}
// If there is dirt within the 4x4 box of the actor calling clearDirt() 
// the dirt will be set to dead and invisible
bool Actor::clearDirt()
{
	bool flag = false;
	int i = 0;
	if (getY() < 60)
	{
		std::vector<std::vector<std::unique_ptr<Dirt>>>* dirt = getWorld()->getDirt();
		for (int i = getY(); i < getY() + SPRITE_HEIGHT; i++)
		{
			if (i >= 60)
				break;
			for (int j = getX(); j < getX() + SPRITE_WIDTH; j++)
			{
				if (!(*dirt)[i][j]->isDead())
				{
					(*dirt)[i][j]->setDead(true);
					(*dirt)[i][j]->setVisible(false);
					flag = true;
				}
			}
		}
	}
	return flag;
}

// Returns true if there is dirt within the 4x4 grid of the actor with grid lcation (getX(), getY())
bool Actor::isDirt()
{
	bool flag = false;
	int i = 0;
	if (getY() < 60)
	{
		std::vector<std::vector<std::unique_ptr<Dirt>>>* dirt = getWorld()->getDirt();
		for (int i = getY(); i < getY() + SPRITE_HEIGHT; i++)
		{
			if (i >= 60)
				break;
			for (int j = getX(); j < getX() + SPRITE_WIDTH; j++)
			{
				if (j > 63 || j < 0)
					break;
				if (!(*dirt)[i][j]->isDead())
				{
					flag = true;
				}
			}
		}
	}
	return flag;
}

// Checks if an actor of type ActorType is to the left, right, up or down of calling object
bool Actor::isTypeActorInFront(Direction dir, ActorType type)
{
	switch (type)
	{
	case boulder:
		for (auto & a : getWorld()->getActors())
		{
			if (!dynamic_cast<Boulder*>(a.get()))
				continue;
			if (!a->isDead() && (a->getY() - getY() <= 4 && a->getY() - getY() >= -4) && (a->getX() - getX() <= 4 && a->getX() - getX() >= -4))
			{
				switch (dir)
				{
				case up:
					return isAbove(a);
					break;
				case down:
					return isBelow(a);
					break;
				case left:
					return isLeft(a);
					break;
				case right:
					return isRight(a);
					break;
				}
			}
		}
		break;
	case digger:
		switch (dir)
		{
		case up:
			return isAbove(getDigger());
			break;
		case down:
			return isBelow(getDigger());
			break;
		case left:
			return isLeft(getDigger());
			break;
		case right:
			return isRight(getDigger());
			break;
		default:
			return false;
		}
		break;
	case protester:
		for (auto & p : getWorld()->getProtesters())
		{
			if (p->isDead())
				continue;
			if (distanceFromActor(p) < 4)
			{
				p->decHealth();
				p->decHealth();
				p->setStunned(true);
				getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
				return true;
			}
		}
		break;
	case dirt:
		switch (dir)
		{
		case up:
			if (getY() < 56)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!(*getWorld()->getDirt())[getY() + 4][getX() + i]->isDead())
						return true;
				}
			}
			break;
		case down:
			if (getY() > 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!(*getWorld()->getDirt())[getY() - 1][getX() + i]->isDead())
						return true;
				}
			}
			break;
		case left:
			if (getX() > 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (getY() + i > 59)
						return false;
					if (!(*getWorld()->getDirt())[getY() + i][getX() - 1]->isDead())
						return true;
				}
			}
			break;
		case right:
			if (getX() < 59)
				for (int i = 0; i < 4; i++)
				{
					if (getY() + i > 59)
						return false;
					if (!(*getWorld()->getDirt())[getY() + i][getX() + 4]->isDead())
						return true;
				}
			break;
		}
		return false;
		break;
	}
	return false;
}

// returns true if actor a is above this
template<typename T>
bool Actor::isAbove(T& a)
{
	return (a->getY() - getY() == SPRITE_HEIGHT && ((getX() >= a->getX() && getX() - a->getX() < SPRITE_WIDTH) || (a->getX() >= getX() && a->getX() - getX() < SPRITE_WIDTH)));
}

// returns true if actor a is below this
template<typename T>
bool Actor::isBelow(T& a)
{
	return (getY() - a->getY() == SPRITE_HEIGHT && ((getX() >= a->getX() && getX() - a->getX() < SPRITE_WIDTH) || (a->getX() >= getX() && a->getX() - getX() < SPRITE_WIDTH)));
	
}

// returns true if actor a is left of this
template<typename T>
bool Actor::isLeft(T& a)
{
	return (getX() - a->getX() == SPRITE_WIDTH && ((getY() >= a->getY() && getY() - a->getY() < SPRITE_HEIGHT) || (a->getY() >= getY() && a->getY() - getY() < SPRITE_HEIGHT)));
}

// returns true if actor a is right of this
template<typename T>
bool Actor::isRight(T& a)
{
	return (a->getX() - getX() == SPRITE_WIDTH && ((getY() >= a->getY() && getY() - a->getY() < SPRITE_HEIGHT) || (a->getY() >= getY() && a->getY() - getY() < SPRITE_HEIGHT)));
}

// returns Diggerman from studentWorld without having to call getWorld->getDiggerMan()
std::shared_ptr<DiggerMan> Actor::getDigger()
{
	return m_student_world->getDiggerMan();
}

void Squirt::doSomething()
{
	if (isDead())
		return;
	if (isDirt())
	{
		setVisible(false);
		setDead(true);
		return;
	}
	if (m_distance < 4)
	{
		switch (getDirection())
		{
		case up:
			if (getY() < 60)
				moveTo(getX(), getY() + 1);
			break;
		case down:
			if (getY() > 0)
				moveTo(getX(), getY() - 1);
			break;
		case left:
			if (getX() > 0)
				moveTo(getX() - 1, getY());
			break;
		case right:
			if (getX() < 60)
				moveTo(getX() + 1, getY());
			break;
		}
		if (!m_shotFlag){
			setShotFlag(true);
		}
		incDistance();
	}
	else
	{
		setVisible(false);
		setDead(true);
	}
	if (isTypeActorInFront(up, protester))
	{
		setVisible(false);
		setDead(true);
	}
}

//*************BOULDER******************
void Boulder::doSomething()
{
	if (isDead())
		return;
	if (!isFalling() && !isDirtBelow())
	{
		setFalling(true);
	}
	if (isFalling())
	{
		if (delay())
		{
			fall();
			if (!m_soundCheck)
			{
				getWorld()->playSound(SOUND_FALLING_ROCK);
				m_soundCheck = true;
			}
		}
	}

}

// checks for dirt below this boulder
bool Boulder::isDirtBelow()
{
	if (m_belowFlag)
	{
		auto count = 0;
		std::vector<std::vector<std::unique_ptr<Dirt>>>* dirt = getWorld()->getDirt();
		for (int x = getX(); x < getX() + SPRITE_WIDTH; x++)
		{
			if (count > 0)
				break;
			if (!(*dirt)[getY() - 1][x]->isDead())
				count++;
		}
		if (count == 0)
		{
			setBelowFlag(false);
		}
	}
	if (m_Falling)
	{
		auto count = 0;
		std::vector<std::vector<std::unique_ptr<Dirt>>>* dirt = getWorld()->getDirt();
		for (int x = getX(); x < getX() + SPRITE_WIDTH; x++)
		{
			if (getY() - 1 == 0)
			{
				setDead(true);
				setVisible(false);
				return true;
			}
			if ((*dirt)[getY() - 1][x]->isDead())
				count++;
		}
		if (count == 4){}
		else
			setBelowFlag(true);
	}
	return m_belowFlag;
}

// delays this boulders fall once all dirt
// has been removed from beneath this boulder
bool Boulder::delay()
{
	if (m_delayTicks == 0)
		m_delayTicks = getWorld()->getTicks();
	if (getWorld()->getTicks() - m_delayTicks < 20)
		return false;
	else
		return true;
}

// called from Boulder::doSomething(), checks for 
// digger or protester to kill them and continues
// to move down until reaching the bottom or hitting dirt
void Boulder::fall()
{
	if (isBelow(getDigger()))
		getDigger()->setDead(true);
	for (auto p : getWorld()->getProtesters())
	{
		if (isBelow(p))
		{
			p->setLeaveState(true);
			getWorld()->increaseScore(500);
		}
	}
	if (getY() > 0)
		moveTo(getX(), getY() - 1);
	if (isDirtBelow() || getX() == 0)
	{
		setDead(true);
		setVisible(false);
	}
	(*getWorld()->getRunawayGrid())[getY()][getX()]->setWalkFlag(true);
}

//**************DIGGERMAN****************
void DiggerMan::doSomething()
{
	if (isDead())
		return;
	if (getWorld()->getTicks() % 8 == 0) // Timer to control sounds played. without this the sound is cluttered
		m_digFlag = true;
	diggerAction();
	if (!isDead())
	{
		if (clearDirt())
		{
			if (m_digFlag)
				getWorld()->playSound(SOUND_DIG);
			m_digFlag = false;
		}
	}
}
bool DiggerMan::isBoulder(Direction dir)
{
	return isTypeActorInFront(dir, boulder);
}

void DiggerMan::dropGold()
{
	switch (getDirection())
	{
	case up:
		getWorld()->addActor(std::make_shared<Nugget>(getWorld(), getX(), getY() + 4, true, false, true));
		break;
	case down:
		getWorld()->addActor(std::make_shared<Nugget>(getWorld(), getX(), getY() - 4, true, false, true));
		break;
	case left:
		getWorld()->addActor(std::make_shared<Nugget>(getWorld(), getX() - 4, getY(), true, false, true));
		break;
	case right:
		getWorld()->addActor(std::make_shared<Nugget>(getWorld(), getX() + 4, getY(), true, false, true));
		break;
	}
}
void DiggerMan::setHealth(int damage)
{
	m_health -= damage;
	if (m_health <= 0)
		setDead(true);
}

// moves 1 square in getDirection(), updates the EmptySquaregrid
// and RunawayGrid for use by waterpool creation and Protester leave()
void DiggerMan::diggerAction()
{
	int key;
	if (getWorld()->getKey(key) == true)
	{
		switch (key)
		{
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			if (!isBoulder(up))
			{
				if (getY() < 60)
					moveTo(getX(), getY() + 1);
				if (!getWorld()->containsSquare(getX(), getY()))
					getWorld()->getEmptySquares()->emplace_back(getX(), getY());
				(*getWorld()->getRunawayGrid())[getY()][getX()]->setWalkFlag(true);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			if (!isBoulder(down))
			{
				if (getY() > 0)
					moveTo(getX(), getY() - 1);
				if (!getWorld()->containsSquare(getX(), getY()))
					getWorld()->getEmptySquares()->emplace_back(getX(), getY());
				(*getWorld()->getRunawayGrid())[getY()][getX()]->setWalkFlag(true);
			}
			break;
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
				setDirection(left);
			if (!isBoulder(left))
			{
				if (getX() > 0)
					moveTo(getX() - 1, getY());
				if (!getWorld()->containsSquare(getX(), getY()))
					getWorld()->getEmptySquares()->emplace_back(getX(), getY());
				(*getWorld()->getRunawayGrid())[getY()][getX()]->setWalkFlag(true);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
				setDirection(right);
			if (!isBoulder(right))
			{
				if (getX() < 60)
					moveTo(getX() + 1, getY());
				if (!getWorld()->containsSquare(getX(), getY()))
					getWorld()->getEmptySquares()->emplace_back(getX(), getY());
				(*getWorld()->getRunawayGrid())[getY()][getX()]->setWalkFlag(true);
			}
			break;
		case KEY_PRESS_TAB:
			if (getGold() > 0)
			{
				dropGold();
				m_gold--;
			}
			break;
		case 'Z':
		case 'z':
			if (getSonarCount() > 0)
			{
				setSonarFlag(true);
				decSonar();
			}
			break;
		case KEY_PRESS_SPACE:
			shoot();
			break;
		case KEY_PRESS_ESCAPE:
			setDead(true);
			break;
		}
	}
}
void DiggerMan::shoot()
{
	if (m_numSquirts > 0)
	{
		switch (getDirection())
		{
		case up:
			if (getY() > 60)
				return;
			m_gun = std::make_unique<Squirt>(getWorld(), getX(), getY() + 4, up);
			break;
		case down:
			if (getY() < 4)
				return;
			m_gun = std::make_unique<Squirt>(getWorld(), getX(), getY() - 4, down);
			break;
		case left:
			if (getX() < 4)
				return;
			m_gun = std::make_unique<Squirt>(getWorld(), getX() - 4, getY(), left);
			break;
		case right:
			if (getX() > 59)
				return;
			m_gun = std::make_unique<Squirt>(getWorld(), getX() + 4, getY(), right);
			break;
		}
		getWorld()->playSound(SOUND_PLAYER_SQUIRT);
		getWorld()->addActor(std::move(m_gun));
		decSquirts();
	}
}

//**************PROTESTERS****************
void RegularProtester::doSomething()
{
	if (isDead())
		return;
	if (getLeaveState())
	{
		leave();
		return;
	}
	if (getHealth() <= 0)
	{
		setLeaveState(true);
	}
	if (isStunned())
	{
		stunned();
		return;
	}
	if (getDigger()->getX() == getX())
	{
		if (getDigger()->getY() > getY())
		{
			if (checkIfClear(up))
			{
				setDirection(up);
			}

		}
		else
		{
			if (checkIfClear(down))
			{
				setDirection(down);
				setSquaresWalked(1);
			}

		}
	}
	else if (getDigger()->getY() == getY())
	{
		if (getDigger()->getX() < getX())
		{
			if (checkIfClear(left))
			{
				setDirection(left);
				setSquaresWalked(1);
			}

		}
		else
		{
			if (checkIfClear(right))
			{
				setDirection(right);
				setSquaresWalked(1);
			}

		}
	}

	if (!isWaiting())
	{
		if (getTickCount() % (getWaitDuration() + 1) == 0)
		{
			incTickCount();
			setWaitState(true);
			return;
		}
		if (getSquaresWalked() % (getMaxSquares() + 1) == 0)
		{
			setSquaresWalked(1);
			protesterAction(findNewDirection());
		}
		else
			protesterAction(getDirection());
		incTickCount();
	}
	else
	{
		if (getTickCount() % getWaitDuration() == 0)
			setWaitState(false);
		incTickCount();
	}
}
void HardcoreProtester::doSomething()
{
 	if (isDead())
		return;
	if (getLeaveState())
	{
		leave();
		return;
	}
	if (getHealth() <= 0)
	{
		setLeaveState(true);
	}
	if (isStunned())
	{
		stunned();
		return;
	}
	/*if (isHunting())
	{
		hunt();
		return;
	}
	if ((getDigger()->getX() == getX() && distanceFromActor(getDigger()) < m_cell_distance) || (getDigger()->getY() == getY() && distanceFromActor(getDigger())))
	{
		hunt();
	}*/
	if (getDigger()->getX() == getX())
	{
		if (getDigger()->getY() > getY())
		{
			if (checkIfClear(up))
			{
				setDirection(up);
			}

		}
		else
		{
			if (checkIfClear(down))
			{
				setDirection(down);
				setSquaresWalked(1);
			}

		}
	}
	else if (getDigger()->getY() == getY())
	{
		if (getDigger()->getX() < getX())
		{
			if (checkIfClear(left))
			{
				setDirection(left);
				setSquaresWalked(1);
			}

		}
		else
		{
			if (checkIfClear(right))
			{
				setDirection(right);
				setSquaresWalked(1);
			}

		}
	}

	if (!isWaiting())
	{
		if (getTickCount() % (getWaitDuration() + 1) == 0)
		{
			incTickCount();
			setWaitState(true);
			return;
		}
		if (getSquaresWalked() % (getMaxSquares() + 1) == 0)
		{
			setSquaresWalked(1);
			protesterAction(findNewDirection());
		}
		else
			protesterAction(getDirection());
		incTickCount();
	}
	else
	{
		if (getTickCount() % getWaitDuration() == 0)
			setWaitState(false);
		incTickCount();
	}
}
//void Protester::doSomething()
//{
//	if (isDead())
//		return;
//	if (getLeaveState())
//	{
//		leave();
//		return;
//	}
//	if (getHealth() <= 0)
//	{
//		setLeaveState(true);
//	}
//	if (isStunned())
//	{
//		stunned();
//		return;
//	}
//	if (getDigger()->getX() == getX())
//	{
//		if (getDigger()->getY() > getY())
//		{
//			if (checkIfClear(up))
//			{
//				setDirection(up);
//			}
//			
//		}
//		else
//		{
//			if (checkIfClear(down))
//			{
//				setDirection(down);
//				setSquaresWalked(1);
//			}
//			
//		}
//	}
//	else if (getDigger()->getY() == getY())
//	{
//		if (getDigger()->getX() < getX())
//		{
//			if (checkIfClear(left))
//			{
//				setDirection(left);
//				setSquaresWalked(1);
//			}
//				
//		}
//		else
//		{
//			if (checkIfClear(right))
//			{
//				setDirection(right);
//				setSquaresWalked(1);
//			}
//				
//		}
//	}
//
//	if (!isWaiting())
//	{
//		if (getTickCount() % (getWaitDuration() + 1) == 0)
//		{
//			incTickCount();
//			setWaitState(true);
//			return;
//		}
//		if (getSquaresWalked() % (getMaxSquares() + 1) == 0)
//		{
//			setSquaresWalked(1);
//			protesterAction(findNewDirection());
//		}
//		else
//			protesterAction(getDirection());
//		incTickCount();
//	}
//	else
//	{
//		if (getTickCount() % getWaitDuration() == 0)
//			setWaitState(false);
//		incTickCount();
//	}
//}
auto Protester::adjWalk()->bool
{
	bool l = (!isTypeActorInFront(left, dirt) && !isTypeActorInFront(left, boulder));
	bool r = (!isTypeActorInFront(right, dirt) && !isTypeActorInFront(right, boulder));
	bool u = (!isTypeActorInFront(up, dirt) && !isTypeActorInFront(up, boulder));
	bool d = (!isTypeActorInFront(down, dirt) && !isTypeActorInFront(down, boulder));
	switch (getDirection())
	{
	case up:
	case down:
		if (getX() > 0 && getX() < 60)
		{
			if (l && r)
			{
				if (rand() % 2 == 0)
				{
					setDirection(right);
					moveTo(getX() + 1, getY());
					setAdjSquare(0);

				}
				else
				{
					setDirection(left);
					moveTo(getX() - 1, getY());
					setAdjSquare(0);
				}
				initWalkDistance();
				return true;
			}
		}
		if (getX() > 0)
		{
			if (l)
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
				setAdjSquare(0);
				initWalkDistance();
				return true;
			}
		}
		if (getX() < 60)
		{
			if (r)
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
				setAdjSquare(0);
				initWalkDistance();
				return true;
			}
		}
		break;
	case right:
	case left:
		if (getY() > 0 && getY() < 60)
		{
			if (u && d)
			{
				if (rand() % 2 == 0)
				{
					setDirection(up);
					moveTo(getX(), getY() + 1);
					setAdjSquare(0);
				}
				else
				{
					setDirection(down);
					moveTo(getX(), getY() - 1);
					setAdjSquare(0);
				}
				initWalkDistance();
				return true;
			}
		}
		if (getY() < 60)
		{
			if (u)
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
				setAdjSquare(0);
				initWalkDistance();
				return true;
			}
		}
		if (getY() > 0)
		{
			if (d)
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
				setAdjSquare(0);
				initWalkDistance();
				return true;
			}
		}
		break;

	}
	return false;
}

void Protester::protesterAction(Direction dir)
{
	bool adj = false;
	if (getAdjSquare() >= 200)
	{
		adj = adjWalk();
	}
	if (!adj)
	{
		switch (dir)
		{
		case up:
			if (getY() < 60)
			{
				if (getDirection() != up)
					setDirection(up);
				if (!isTypeActorInFront(up, dirt) && !isTypeActorInFront(up, boulder))
				{
					moveTo(getX(), getY() + 1);
				}
			}
			break;
		case down:
			if (getY() > 0)
			{
				if (getDirection() != down)
					setDirection(down);
				if (!isTypeActorInFront(down, dirt) && !isTypeActorInFront(down, boulder))
				{
					moveTo(getX(), getY() - 1);
				}
			}
			break;
		case left:
			if (getX() > 0)
			{
				if (getDirection() != left)
					setDirection(left);
				if (!isTypeActorInFront(left, dirt) && !isTypeActorInFront(left, boulder))
				{
					moveTo(getX() - 1, getY());
				}
			}
			break;
		case right:
			if (getX() < 60)
			{
				if (getDirection() != right)
					setDirection(right);
				if (!isTypeActorInFront(right, dirt) && !isTypeActorInFront(right, boulder))
				{
					moveTo(getX() + 1, getY());
				}
			}
			break;
		}
	}
	incSquaresWalked();
	incAdjSquare();
	if (isTypeActorInFront(getDirection(), digger))
	{
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		getDigger()->setHealth(20);
		setStunned(true);
		return;
	}
}

// randomely selects a new direction and updates the number
// of steps to be taken before chosing a new direction again
GraphObject::Direction Protester::findNewDirection()
{
	Direction d = getDirection();
	bool canWalk = false;
	while (!canWalk)
	{
		switch (rand() % 4 + 1)
		{
		case up:
			if (getY() < 60)
			{
				if (!isTypeActorInFront(up, dirt) && !isTypeActorInFront(up, boulder))
				{
					d = up;
					canWalk = true;
				}
			}
			break;
		case down:
			if (getY() > 0)
			{
				if (!isTypeActorInFront(down, dirt) && !isTypeActorInFront(down, boulder))
				{
					d = down;
					canWalk = true;
				}
			}
			break;
		case left:
			if (getX() > 0)
			{
				if (!isTypeActorInFront(left, dirt) && !isTypeActorInFront(left, boulder))
				{
					d = left;
					canWalk = true;
				}
			}
			break;
		case right:
			if (getX() < 60)
			{
				if (!isTypeActorInFront(right, dirt) && !isTypeActorInFront(right, boulder))
				{
					d = right;
					canWalk = true;
				}
			}
			break;
		}
	}
	initWalkDistance();
	return d;
}

// Checks if the diggerman can be reached without hitting and boulders or dirt
auto Protester::checkIfClear(Direction dir)->bool
{
	int currentY = getY();
	int currentX = getX();
	auto actors = getWorld()->getActors();
	auto dirt = getWorld()->getDirt();
	bool flag = true;
	switch (dir)
	{
	case up:
		if (getY() < 56)
		{
			for (int k = 0; k < getWorld()->getBoulder(); k++)
			{
				if (!actors[k]->isDead() && actors[k]->getX() == getX() && actors[k]->getY() < getDigger()->getY() && actors[k]->getY() > getY())
					return false;
			}
			for (int d = getY() + 4; d < getDigger()->getY(); d++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (!(*dirt)[d][getX() + j]->isDead())
						flag = false;
				}
			}
		}
		break;
	case down:
		if (getY() > 0)
		{
			for (int k = 0; k < getWorld()->getBoulder(); k++)
			{
				if (!actors[k]->isDead() && actors[k]->getX() == getX() && actors[k]->getY() > getDigger()->getY() && actors[k]->getY() < getY())
					return false;
			}
			for (int d = getY() - 1; d > getDigger()->getY(); d--)
			{
				for (int j = 0; j < 4; j++)
				{
					if (!(*dirt)[d][getX() + j]->isDead())
						flag = false;
				}
			}
		}
		break;
	case left:
		if (getX() > 0 && getY() < 56)
		{
			for (int k = 0; k < getWorld()->getBoulder(); k++)
			{
				if (!actors[k]->isDead() && actors[k]->getY() == getY() && actors[k]->getX() > getDigger()->getX() && actors[k]->getX() < getX())
					return false;
			}
			for (int d = getX() - 1; d > getDigger()->getX(); d--)
			{
				for (int j = 0; j < 4; j++)
				{
					if (!(*dirt)[getY() + j][d]->isDead())
						flag = false;
				}
			}
		}
		break;
	case right:
		if (getX() < 59 && getY() < 56)
		{
			for (int k = 0; k < getWorld()->getBoulder(); k++)
			{
				if (!actors[k]->isDead() && actors[k]->getY() == getY() && actors[k]->getX() < getDigger()->getX() && actors[k]->getX() > getX())
					return false;
			}
			for (int d = getX() + 4; d < getDigger()->getX(); d++)
			{
				if (d > 57)
					break;
				for (int j = 0; j < 4; j++)
				{
					if (!(*dirt)[getY() + j][d]->isDead())
						flag = false;
				}
			}
		}
		break;
	}
	return flag;
}
auto Protester::type(ActorType ty)->bool
{
	switch (ty)
	{
	case regular:
		return dynamic_cast<RegularProtester*>(this);
	case hardcore:
		return dynamic_cast<HardcoreProtester*>(this);
	}
	return false;
}
auto Protester::initWalkDistance()->void
{
	m_max_squares = rand() % 55 + 8;
}
auto Protester::initWaitTicks()->void
{
	m_wait_duration = std::max(0, 3 - static_cast<int>(getWorld()->getLevel()) / 4);
}
auto Protester::initStunDuration()->void
{
	m_stun_duration = std::max(50, 100 - static_cast<int>(getWorld()->getLevel()) * 10);
}

auto Protester::leave()->void
{
	if (!updated())
	{
		if (type(regular))
			getWorld()->increaseScore(100);
		else
			getWorld()->increaseScore(250);
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		getWorld()->updateRunGrid(getX(), getY(), 60, 60);
		fillLeaveQueue(getX(), getY());
		m_updated = true;
	}
	if (updated())
	{
		moveOut();
	}
}
auto Protester::moveOut()->void
{
	if (getX() == 60 && getY() == 60)
	{
		setDead(true);
		setVisible(false);
		getWorld()->decProtCount();
		return;
	}
	if (!m_leaveQ.empty())
	{
		switch (m_leaveQ.front())
		{
		case up:
			if(getDirection() != up)
				setDirection(up);
			moveTo(getX(), getY() + 1);
			break;
		case down:
			if (getDirection() != down)
				setDirection(down);
			moveTo(getX(), getY() - 1);
			break;
		case left:
			if (getDirection() != left)
				setDirection(left);
			moveTo(getX() - 1, getY());
			break;
		case right:
			if (getDirection() != right)
				setDirection(right);
			moveTo(getX() + 1, getY());
			break;
		}
		m_leaveQ.pop();
	}
}
auto Protester::fillLeaveQueue(int x, int y)->void
{
	//!(x==60 && y + 1 == 60)
	std::shared_ptr<RunawayBox> currentBox = (*getWorld()->getRunawayGrid())[y][x];
	while (currentBox->getDistance() > 0)
	{
		currentBox = (*getWorld()->getRunawayGrid())[y][x];
		if (y < 60 && (*getWorld()->getRunawayGrid())[y + 1][x]->getDistance() == currentBox->getDistance() - 1)
		{
			m_leaveQ.push(up);
			y++;
			continue;
		}
		if (x > 0 && (*getWorld()->getRunawayGrid())[y][x - 1]->getDistance() == currentBox->getDistance() - 1)
		{
			m_leaveQ.push(left);
			x--;
			continue;
		}
		if (x < 60 && (*getWorld()->getRunawayGrid())[y][x + 1]->getDistance() == currentBox->getDistance() - 1)
		{
			m_leaveQ.push(right);
			x++;
			continue;
		}
		if (y > 0 && (*getWorld()->getRunawayGrid())[y - 1][x]->getDistance() == currentBox->getDistance() - 1)
		{
			m_leaveQ.push(down);
			y--;
			continue;
		}
		
	}
}

auto Protester::stunned()->void
{
	if (m_stun_ticks == 0)
		m_stun_ticks = getWorld()->getTicks();
	if (getWorld()->getTicks() - m_stun_ticks < m_stun_duration)
		return;
	else
	{
		setStunned(false);
		m_stun_ticks = 0;
	}
}
void HardcoreProtester::hunt()
{
	if (!isHunting())
	{
		getWorld()->updateRunGrid(getX(), getY(), getDigger()->getX(), getDigger()->getY());
		fillLeaveQueue(getX(), getY());
		m_isHunting = true;
	}
	else
	{
		if (getHuntSteps() % 5 == 0)
		{
			setHuntSteps(1);
			m_isHunting = false;
			return;
		}
		moveOut();
		incHuntSteps();
	}
}
void HardcoreProtester::initCellDistance()
{
	m_cell_distance = 16 + getWorld()->getLevel() * 2;
}
//***************GOODIES*****************
void Goodie::getPickedUp()
{
	if (getDiggerState())
		setDiggerState(false);
	else
		setProtesterState(false);
	setPickedUp(true);
	setVisible(false);
	setDead(true);
}
void Goodie::setPickedUp(bool flag)
{
	m_pickedUp = flag;
}

void Nugget::doSomething()
{
	if (isDead())
		return;
	else if (getDiggerState())
	{
		if (getDigger()->getSonarFlag())
		{
			if (distanceFromActor(getDigger()) < 12)
			{
				setVisible(true);
			}
		}
		else if (!isPickedUp())
		{
			if (distanceFromActor(getDigger()) < 7)
			{
				setVisible(true);
			}
		}
		if (distanceFromActor(getDigger()) <= 3)
			getPickedUp();
		if (isPickedUp())
		{
			getDigger()->incGold();
			getWorld()->increaseScore(25);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			return;
		}
	}
	if (getProtState())
	{
		for (auto p : getWorld()->getProtesters())
		{
			if (distanceFromActor(p) <= 3)
			{
				getPickedUp();
				getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
				p->incGold();
				p->setStunned(true);
				if (p->type(regular))
				{
					if (p->getGold() > 3)
						p->setLeaveState(true);
				}
			}
		}
	}
}
void Oil::doSomething()
{
	if (isDead())
		return;
	if (getDiggerState())
	{
		if (getDigger()->getSonarFlag())
		{
			if (distanceFromActor(getDigger()) < 12)
				setVisible(true);
		}
		else
		{
			if (distanceFromActor(getDigger()) < 7)
				setVisible(true);
		}
		if (distanceFromActor(getDigger()) <= 3)
			getPickedUp();
		if (isPickedUp())
		{
			getWorld()->decOil();
			getWorld()->playSound(SOUND_FOUND_OIL);
			getWorld()->increaseScore(1000);
		}
	}
}

auto TemporaryGoodie::initLifeTicks()->void
{
	m_life_ticks = std::max(100, (300 - 10 * static_cast<int>(getWorld()->getLevel())));
}
void TemporaryGoodie::doSomething()
{
	if (isDead())
		return;
	if (distanceFromActor(getDigger()) <= 3)
		getPickedUp();
	if (isPickedUp())
	{
		switch (getImgID())
		{
		case IMID_WATER_POOL:
			getDigger()->incWater();
			break;
		case IMID_SONAR:
			getWorld()->increaseScore(75);
			getDigger()->incSonar();
			break;
		}
		getWorld()->playSound(getSoundID());
	}
	incTicks();
	if (getTicks() == getLifeTicks())
	{
		setDead(true);
		setVisible(false);
	}
}