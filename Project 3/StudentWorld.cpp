#include "StudentWorld.h"
#include <string>
#include "Actor.h"
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::init(){
	isFinished = false;
	m_bonus = 1000; 
	ostringstream str;
	str << "level";					
	if (getLevel() < 10)
		str << "0";
	str << getLevel() <<".dat";
	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(str.str());
	if (result == Level::load_fail_bad_format){				//level loading
		return GWSTATUS_LEVEL_ERROR;
	}
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++){
		Level::MazeEntry ge = lev.getContentsOf(i, j);
		switch (ge)
		{
		case Level::empty:
			break;
		case Level::ammo:
			obj.push_back(new Ammo(IID_AMMO, i, j, this));
			break;
		case Level::extra_life:
			obj.push_back(new Heart(IID_EXTRA_LIFE, i, j, this));
			break;
		case Level::exit:
			obj.push_back(new Exit(IID_EXIT, i, j, this));
			break;
		case Level::player:
			p = new Player(IID_PLAYER, i, j, this, Actor::right);
			obj.push_back(p);
			break;
		case Level::horiz_snarlbot:	
			obj.push_back(new Snarlbot(IID_SNARLBOT, i, j, this, Actor::right));
			break;
		case Level::vert_snarlbot:
			obj.push_back(new Snarlbot(IID_SNARLBOT, i, j, this, Actor::down));
			break;

		case Level::kleptobot_factory:
			obj.push_back(new Factory(IID_ROBOT_FACTORY, i, j, this, false));
			break;
		case Level::angry_kleptobot_factory:
			obj.push_back(new Factory(IID_ROBOT_FACTORY, i, j, this, true));
			break;
		case Level::jewel:
			obj.push_back(new Jewel(IID_JEWEL, i, j, this));
			m_jewels++;
			break;
		case Level::wall:
			obj.push_back(new Wall(IID_WALL, i, j, this));
			break;
		case Level::boulder:
			obj.push_back(new Boulder(IID_BOULDER, i, j, this));
			break;
		case Level::hole:
			obj.push_back(new Hole(IID_HOLE, i, j, this));
			break;
		case Level::restore_health:
			obj.push_back(new RestoreHealth(IID_RESTORE_HEALTH, i, j, this));
			break;
		}
	}
	m_tick = (28 - getLevel()) / 4.0;				//set tickCount
	if (m_tick < 3)
		m_tick = 3;
	cur_tick = 0;
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move(){
	setDisplay();
	for (list<Actor*>::iterator it = obj.begin(); it!=obj.end(); it++){		//iterate through actors to move
		if ((*it)->isActive())
		(*it)->doSomething(cur_tick);
	}
	if (isFinished){															//if the game is finished then go to next level
		increaseScore(m_bonus);
		return GWSTATUS_FINISHED_LEVEL;
	}
	if (!p->isActive())	
		return GWSTATUS_PLAYER_DIED;
	for (list<Actor*>::const_iterator it = obj.begin(); it != obj.end();){		//delete dead actors
		if (!(*it)->isActive()){
			delete (*it);
			it = obj.erase(it);
		}
		else
			it++;
	}
	if (m_bonus > 0)
	m_bonus--;
	cur_tick++;
	if (cur_tick == getTick()){
		cur_tick = 0;
	}
	return GWSTATUS_CONTINUE_GAME;

}

void StudentWorld::cleanUp(){				//clean up clean up 
	while (!obj.empty()){
		delete obj.front();
		obj.pop_front();
	}

}


void StudentWorld::setDisplay(){
	ostringstream os;
	os << "Score: ";
	if (getScore() == 0)
		os << "0000000";
	else if (getScore() < 100)
		os << "00000";
	else if (getScore() < 1000)
		os << "0000";
	else if (getScore() < 10000)
		os << "000";
	else if (getScore() < 100000)
		os << "00";
	else if (getScore()<1000000)
		os << "0";
	os << getScore() << "  " << "Level: ";
	if (getLevel() <= 9)
		os << "0";
	os << getLevel();
	os << "  " << "Lives:  " << getLives();
	os << "  " << "Health:  " << p->getHealth() * 5 << "%";
	os << "  " << "Ammo:  " << p->getAmmo();
	os << "  " << "Bonus:  " << m_bonus;
	setGameStatText(os.str());


}
