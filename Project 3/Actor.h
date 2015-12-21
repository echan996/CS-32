#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, int x, int y, StudentWorld* w, Direction d = none) :GraphObject(imageID, x, y, d){
		m_world = w;
		setVisible(true);
		m_active = true;
	};
	virtual Actor* isValidMove(Direction d);
	virtual ~Actor(){};
	virtual void doSomething(int a) = 0;
	void setActive(bool b){ m_active = b; }
	virtual int priority(){ return 0; }
	virtual	bool isActive(){return m_active;}
	virtual bool hittable(){ return false; }
	StudentWorld* world() { return m_world; }
	void deltaDir(int& x, int& y, Direction d){
		switch (d){
		case right:
			x++;
			break;
		case left:
			x--;
			break;
		case up:
			y++;
			break;
		case down:
			y--;
			break;
		default:
			break;
		}
	}
	virtual void fill(){}
private:
	StudentWorld* m_world;
	bool m_active;
};
class Player: public Actor {
public:
	Player(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){ m_health = 20; ammo = 20; }
	void doSomething(int a);
	bool hittable(); 
	Actor* isValidMove(Direction d);
	virtual ~Player(){};
	void addAmmo(int x){ ammo += x; }
	int getAmmo(){ return ammo; }
	int getHealth(){ return m_health; }
	int priority(){ return 2; }
	void setFullHealth(){ m_health = 20; }
private:
	int m_health, ammo;
};  
class Robot : public Actor{
public:
	Robot(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){}
	virtual ~Robot(){ };
	bool hittable();
	void setHealth(int a){ m_health = a; }
	bool shoot();
	int getHealth(){ return m_health; }
	bool obstacleCheck(int rx, int ry, int px, int py, Direction d);
	int priority(){ return 2; }
private:
	int m_health;	
};
class Snarlbot : public Robot {
public:
	Snarlbot(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Robot(imageID, x, y, w, d){ setHealth(10); }
	void doSomething(int a);
	virtual ~Snarlbot(){};
	bool hittable();
};	 
class Kleptobot: public Robot {
public:
	Kleptobot(int imageID, int x, int y, StudentWorld* w, int tick, Direction d) : Robot(imageID, x, y, w, d){ setHealth(5); setDistance(); m_goodie = nullptr; m_tickMove = tick; }
	void doSomething(int a);
	void decDistance(){ distanceBeforeTurning--; }
	void setDistance(){ distanceBeforeTurning = rand() % 6 + 1; }
	int getDistance(){ return distanceBeforeTurning; }
	virtual ~Kleptobot(){ if (m_goodie != nullptr) delete m_goodie; };
	bool hittable();
	int tickMove(){ return m_tickMove; }
private:
	int distanceBeforeTurning;
	Actor* m_goodie;
	int m_tickMove;	
};  
class AngryKleptobot : public Kleptobot{
public:
	AngryKleptobot(int imageID, int x, int y, StudentWorld* w, int tick, Direction d = none) : Kleptobot(imageID, x, y, w, tick, d){ setHealth(8); }
	void doSomething(int a);
	bool hittable();
	virtual ~AngryKleptobot(){};
};  
class Factory : public Actor{
public:
	Factory(int imageID, int x, int y, StudentWorld* w, bool isAngry, Direction d = none) : Actor(imageID, x, y, w, d){ angry = isAngry; }
	void doSomething(int a);
	int priority(){ return 1; }
	virtual ~Factory(){};
	bool hittable(){ return true; }
private:
	bool angry;
};	
class Bullet : public Actor{
public:
	Bullet(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){}
	Actor* isValidMove(Direction d);
	void doSomething(int a);
	virtual ~Bullet(){};
};  
class Exit : public Actor{
public:
	Exit(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){ setVisible(false); }
	void doSomething(int a);
	virtual ~Exit(){};
};	 
class Wall : public Actor{
public:
	Wall(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){}
	void doSomething(int a){};
	virtual ~Wall(){};
	int priority(){ return 1; }
	bool hittable(){ return true; }
};	 
class Boulder : public Actor{
public:
	Boulder(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){ m_health = 10; }
	void doSomething(int a){}
	virtual ~Boulder(){};
	Actor* isValidMove(Direction d);
	int priority(){ return 2; }
	bool hittable(){ 
		m_health -= 2; 
	if (m_health <= 0){ setActive(false); setVisible(false); }
	return true;
	}
private:
	int m_health;
};	
class Hole : public Actor{
public:
	Hole(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){ m_filled = false; }
	void doSomething(int a);
	void fill(){ m_filled = true; }
	virtual ~Hole(){};
private:
	bool m_filled;
};	 
class Goodie: public Actor{
public:
	Goodie(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Actor(imageID, x, y, w, d){}
	void doSomething(int a);
	virtual ~Goodie(){};
};
class Jewel: public Goodie{
public:
	Jewel(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Goodie(imageID, x, y, w, d){}
	void doSomething(int a);
	virtual ~Jewel(){};
}; 
class RestoreHealth : public Goodie{
public:
	RestoreHealth(int imageID, int x, int y, StudentWorld* w, Direction d = none) :Goodie(imageID, x, y, w, d){}
	void doSomething(int a);
	virtual ~RestoreHealth(){};
};  
class Heart : public Goodie{
public:
	Heart(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Goodie(imageID, x, y, w, d){}
	void doSomething(int a);
	virtual ~Heart(){};
};  
class Ammo : public Goodie{
public:
	Ammo(int imageID, int x, int y, StudentWorld* w, Direction d = none) : Goodie(imageID, x, y, w, d){}
	void doSomething(int a);
	virtual ~Ammo(){};
};


#endif // ACTOR_H_


