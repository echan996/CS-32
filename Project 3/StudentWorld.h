#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include "Level.h"
#include <iostream>
class Actor;
class Player;

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		
	}

	virtual int init();
	
	virtual int move();
	
	virtual void cleanUp();

	int numJewels(){ return m_jewels; }
	void decJewels(){ m_jewels--; }
	list<Actor*>& getList(){ return obj; }
	void setFinished(){ isFinished = true; }
	Player* getPlayer(){ return p; }
	void setDisplay();
	int getTick(){ return m_tick; }
	int curTick(){ return cur_tick; }
private:
	Player* p;
	list<Actor*> obj;
	int m_jewels, m_bonus;
	bool isFinished;
	int m_tick, cur_tick;
};

#endif // STUDENTWORLD_H_
