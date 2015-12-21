#include "Actor.h"
#include <algorithm>
#include "StudentWorld.h"
//HAVE LOOPS CHECK FOR VISIBILITY. USE VISIBILITY AS THE CHECK FOR IF SOMETHING IS ALIVE???
//make hittable functions check if the object is dead or not 
Actor* Actor::isValidMove(Direction d){
	int x = getX(), y = getY();
	deltaDir(x,y,d);
	Actor* p = nullptr;
	for (list<Actor*>::const_iterator it = world()->getList().begin(); it!=world()->getList().end(); it++)		//iterate through list. set item to return to the actor in the spot with highest return priority
		if ((*it)->getX() == x && (*it)->getY() == y){
		if (dynamic_cast<Bullet*>(*it) != nullptr || dynamic_cast<Exit*>(*it) != nullptr){
			continue;
		}
		else if (p == nullptr || p->priority() < (*it)->priority())
			p = (*it);
		}

	return p;
}
Actor* Player::isValidMove(Direction d){
	Actor* a = Actor::isValidMove(d);
	if (a == nullptr ||		 dynamic_cast<Goodie*>(a)!=nullptr)
		return nullptr;
	if (dynamic_cast<Boulder*>(a) != nullptr && a->isValidMove(d)==nullptr) // if the actor base function returns a boulder and the boulder can move, move both
		return nullptr;
	return this;
}	
Actor* Boulder::isValidMove(Direction d){
	Actor* a = Actor::isValidMove(d);
	int x = getX(), y = getY();
	deltaDir(x, y, d);
	if (a == nullptr){
		moveTo(x, y);
		return nullptr;
	}
	if (dynamic_cast<Hole*>(a) != nullptr){		//if can move, move into either a hole or blank space
		moveTo(x, y);
		a->fill();
		setActive(false);
		setVisible(false);
		return nullptr;
	}
	return this;
}
bool Player::hittable() {
	if (!isActive())
		return false;
	m_health -= 2;
	if (m_health != 0)
		world()->playSound(SOUND_PLAYER_IMPACT);	//intuitive
	else{
		world()->playSound(SOUND_PLAYER_DIE);
		setActive(false);
		setVisible(false);
	}
	return true;
}
bool Robot::hittable(){
	if (!isActive())
		return false;
	m_health -= 2;
	if (m_health <= 0){
		setVisible(false);
		setActive(false);					//intuitive
		world()->playSound(SOUND_ROBOT_DIE);
		return true;
	}
	world()->playSound(SOUND_ROBOT_IMPACT);
	return true;

}
bool Snarlbot::hittable(){
	Robot::hittable();
	if (getHealth() <= 0)
		world()->increaseScore(100);
	return true;
}	 
void Player::doSomething(int a){
	if (!isActive())
		return;
	if (m_health <= 0){
		setActive(false);
		setVisible(false);
		return;
	}
	int key;					
	if (!world()->getKey(key))
		return;
	switch (key){		//movement and space
	case KEY_PRESS_DOWN:
		setDirection(down);
		if(isValidMove(getDirection()) == nullptr)
		moveTo(getX(),getY()-1);
		break;
	case KEY_PRESS_UP:
		setDirection(up);
		if (isValidMove(getDirection()) == nullptr)
			moveTo(getX(), getY() + 1);
		break;
	case KEY_PRESS_LEFT:
		setDirection(left);
		if (isValidMove(getDirection()) == nullptr)
			moveTo(getX()-1, getY());
		break;
	case KEY_PRESS_RIGHT:
		setDirection(right);
		if (isValidMove(getDirection()) == nullptr)
			moveTo(getX()+1, getY());
		break;
	case KEY_PRESS_ESCAPE:
		setActive(false);
		world()->decLives();
		break;
	case KEY_PRESS_SPACE:{
		if (ammo > 0){
			ammo--;
			Actor* p = Actor::isValidMove(getDirection());
			int x = getX(), y = getY();
			deltaDir(x, y, getDirection());
			Bullet* b = new Bullet(IID_BULLET, x, y, world(), getDirection());
			world()->getList().push_front(b);
			world()->playSound(SOUND_PLAYER_FIRE);
			if (p != nullptr && p->hittable()){		//if the spot in front is something hittable, then kill the bullet
				b->setActive(false);
				b->setVisible(false);
			}
		}
		return; }
	default:
		break;
	}	
}
void Snarlbot::doSomething(int a){
	int x = getX(), y = getY();
	if (!isActive() || a!=0){
		return;
	}
	if (shoot()){						//if it should shoot, then shoot
		deltaDir(x, y, getDirection());
		world()->playSound(SOUND_ENEMY_FIRE);
		Actor* b = new Bullet(IID_BULLET, x, y, world(), getDirection());
		world()->getList().push_front(b);
		Actor* p = Actor::isValidMove(getDirection());
		if (p != nullptr &&p->hittable()){
			b->setActive(false);
			b->setVisible(false);
		}
		return;
	}
	Actor* p = Actor::isValidMove(getDirection());
	deltaDir(x, y, getDirection());
	if (p == nullptr || dynamic_cast<Exit*>(p) != nullptr || dynamic_cast<Goodie*>(p) != nullptr)	//if it can move, then move
		moveTo(x, y);
	else
		switch (getDirection()){			//turn
		case right:
			setDirection(left);
			break;
		case left:
			setDirection(right);
			break;
		case up:
			setDirection(down);
			break;
		case down:
			setDirection(up);
			break;
		default:
			break;
	}
}
void Goodie::doSomething(int a){
	setActive(false);
	setVisible(false);
	world()->playSound(SOUND_GOT_GOODIE);
}
void Jewel::doSomething(int a){
	if (!isActive())
		return;
	if (world()->getPlayer()->getX()==getX() && world()->getPlayer()->getY() == getY()){
		Goodie::doSomething(a);
		world()->decJewels();
		world()->increaseScore(50);
	}
}	  
void Heart::doSomething(int a){
	if (!isActive())
		return;
	if (world()->getPlayer()->getX() == getX() && world()->getPlayer()->getY() == getY()){
		Goodie::doSomething(a);
		world()->incLives();
		world()->increaseScore(1000);
	}
}	  
void Ammo::doSomething(int a){
	if (!isActive())
		return;
	if (world()->getPlayer()->getX() == getX() && world()->getPlayer()->getY() == getY()){
		Goodie::doSomething(a);
		world()->getPlayer()->addAmmo(20);
		world()->increaseScore(100);
	}
}	 
void Hole::doSomething(int a){
	if (!isActive())
		return;
	if(m_filled){				//if a boulder has filled, then kill it
		setActive(false);
		setVisible(false);
		}
}
void Exit::doSomething(int a){
	if (world()->numJewels() == 0 && !isVisible()){			//should reveal
		setVisible(true);
		world()->playSound(SOUND_REVEAL_EXIT);
	}
	if (isVisible() && getX() == world()->getPlayer()->getX() && getY() == world()->getPlayer()->getY()){		//finish the level
		world()->setFinished();
		world()->increaseScore(2000);
		world()->playSound(SOUND_FINISHED_LEVEL);
		world()->getPlayer()->setActive(false);
	}
}
void Bullet::doSomething(int a){
	list<Actor*>::const_iterator it = world()->getList().begin();
	while (it != world()->getList().end()){									//if the cur spot has a hittable actor, kill the bullet and hit the actor
		if ((*it)->getX() == getX() && (*it)->getY() == getY() && (*it)->hittable()){
			setActive(false);
			setVisible(false);
			return;
		}
		it++;
	}
	Actor* p = isValidMove(getDirection());		
	int x = getX();
	int y = getY();
	deltaDir(x, y, getDirection());			//move and hit new object
	if (p == nullptr){
		moveTo(x, y);
		return;
	}
	if (p->hittable()){
		setActive(false);
		setVisible(false);
	}
}	   
void Factory::doSomething(int a){
	if (rand() % 50 != 0)
		return;
	int count = 0;
	bool occupied = false;
	for (list<Actor*>::const_iterator it = world()->getList().begin(); it != world()->getList().end(); it++)	
		if (dynamic_cast<Kleptobot*>(*it) != nullptr && abs((*it)->getX() - getX()) <= 3 && abs((*it)->getY() - getY()) <= 3){		//count number of bots around and check if its occupied 
		count++;
		if ((*it)->getX() == getX() && (*it)->getY() == getY())
			occupied = true;
		}
	if (count < 3 && !occupied){		//check if it should spawn
		if (angry)
			world()->getList().push_back(new AngryKleptobot(IID_KLEPTOBOT, getX(), getY(), world(), world()->curTick(), right));
		else
			world()->getList().push_back(new Kleptobot(IID_KLEPTOBOT, getX(), getY(), world(), world()->curTick(), right));
		world()->playSound(SOUND_ROBOT_BORN);
	}
}	
void Kleptobot::doSomething(int a){
	if (!isActive() || a!=tickMove())
		return;
	if (m_goodie == nullptr)			//if it has no goodie and its on a goodie, then 10% chance of picking it up
		for (list<Actor*>::const_iterator it = world()->getList().begin(); it != world()->getList().end();it++)		
			if (rand() % 10 == 0 && (*it)->isActive() && (*it)->getX() == getX() && (*it)->getY() == getY() && dynamic_cast<Goodie*>(*it) != nullptr && dynamic_cast<Jewel*>(*it) == nullptr){
				m_goodie = (*it);
				(*it)->setVisible(false);
				(*it)->setActive(false);
				world()->getList().erase(it);
				world()->playSound(SOUND_ROBOT_MUNCH);
				return;
			}
	if (getDistance() > 0){			//if it can still move, then move
		decDistance();
		Actor* p = Actor::isValidMove(getDirection());
		if (p == nullptr || dynamic_cast<Goodie*>(p) != nullptr){
			int x = getX(), y = getY();
			deltaDir(x, y, getDirection());
			moveTo(x, y);
			return;
		}
	}
	int dir = rand() % 4 + 1;
	Direction d;
	switch (dir){			//change its direction
	case 1:
		d = right;
		break;
	case 2:
		d = left;
		break;
	case 3:
		d = up;
		break;
	case 4:
		d = down;
		break;
	default:
		break;
	}	
	Direction q = d;
	for (int i=0 ;i<4;i++){
		switch (dir){
		case 1:
			q = right;
			break;
		case 2:
			q = left;
			break;
		case 3:
			q = up;
			break;
		case 4:
			q = down;
			break;
		default:
			break;
		}
		Actor* p = Actor::isValidMove(q);
		if (p == nullptr || dynamic_cast<Goodie*>(p) != nullptr)	//if its found an empty direction, turn to it
			goto jump;
		dir++;
		if (dir > 4)
			dir = 1;
	}
	jump:
	setDirection(q);
	Actor* p = Actor::isValidMove(getDirection());
	if (p != nullptr && dynamic_cast<Goodie*>(p) == nullptr)	//if the spot is occupied, then dont move
		return;
	int x = getX(), y = getY();
	deltaDir(x, y, getDirection());
	moveTo(x, y);
	setDistance();
}
bool Kleptobot::hittable(){
	Robot::hittable();
	if (getHealth() <= 0 && m_goodie != nullptr){		//if its dead, drop the item
		world()->increaseScore(10);
		m_goodie->moveTo(getX(), getY());
		world()->getList().push_back(m_goodie);
		m_goodie->setActive(true);
		m_goodie->setVisible(true);
		m_goodie = nullptr;
	}
	return true;
}
Actor* Bullet::isValidMove(Direction d){
	Actor* p = Actor::isValidMove(getDirection());
	if (dynamic_cast<Hole*>(p) != nullptr || dynamic_cast<Goodie*>(p) != nullptr)
		return nullptr;
	return p;
}
bool Robot::shoot(){
	switch (getDirection()){		//if the player is in the way and theres nothing blocking it then shoot
	case Actor::right:
		if (world()->getPlayer()->getY() == getY() && world()->getPlayer()->getX() > getX()
			&& obstacleCheck(getX() + 1, getY(), world()->getPlayer()->getX(), world()->getPlayer()->getY(), getDirection()))
			return true;

		break;
	case Actor::left:
		if (world()->getPlayer()->getY() == getY() && world()->getPlayer()->getX() < getX()
			&& obstacleCheck(getX() - 1, getY(), world()->getPlayer()->getX(), world()->getPlayer()->getY(), getDirection()))
			return true;
		break;
	case Actor::up:
		if (world()->getPlayer()->getY() > getY() && world()->getPlayer()->getX() == getX()
			&& obstacleCheck(getX(), getY() + 1, world()->getPlayer()->getX(), world()->getPlayer()->getY(), getDirection()))
			return true;
		break;
	case Actor::down:
		if (world()->getPlayer()->getY() < getY() && world()->getPlayer()->getX() == getX()
			&& obstacleCheck(getX(), getY() - 1, world()->getPlayer()->getX(), world()->getPlayer()->getY(), getDirection()))
			return true;
		break;
	default:
		break;
	}
	return false;
}
bool Robot::obstacleCheck(int rx, int ry, int px, int py, Direction d){
	switch (d){	//check in the direction. if theres anything in the way, then return false
	case Actor::right:
		while (rx != px){
			for (list<Actor*>::const_iterator it = world()->getList().begin(); it != world()->getList().end(); it++){
				if ((*it)->getX() == rx && (*it)->getY() == ry && (dynamic_cast<Robot*>(*it) != nullptr || dynamic_cast<Wall*>(*it) != nullptr || dynamic_cast<Boulder*>(*it) != nullptr || dynamic_cast<Factory*>(*it) != nullptr)){
					return false;
				}
			}
			rx++;
		}
		break;
	case Actor::left:
		while (rx != px){
			for (list<Actor*>::const_iterator it = world()->getList().begin(); it != world()->getList().end(); it++)
				if ((*it)->getX() == rx && (*it)->getY() == ry && (dynamic_cast<Robot*>(*it) != nullptr || dynamic_cast<Wall*>(*it) != nullptr || dynamic_cast<Boulder*>(*it) != nullptr || dynamic_cast<Factory*>(*it) != nullptr))
					return false;
			rx--;
		}
		break;
	case Actor::up:
		while (ry != py){
			for (list<Actor*>::const_iterator it = world()->getList().begin(); it != world()->getList().end(); it++)
				if ((*it)->getX() == rx && (*it)->getY() == ry && (dynamic_cast<Robot*>(*it) != nullptr || dynamic_cast<Wall*>(*it) != nullptr || dynamic_cast<Boulder*>(*it) != nullptr || dynamic_cast<Factory*>(*it) != nullptr))
					return false;
			ry++;
		}
		break;
	case Actor::down:
		while (ry != py){
			for (list<Actor*>::const_iterator it = world()->getList().begin(); it != world()->getList().end(); it++)
				if ((*it)->getX() == rx && (*it)->getY() == ry && (dynamic_cast<Robot*>(*it) != nullptr || dynamic_cast<Wall*>(*it) != nullptr || dynamic_cast<Boulder*>(*it) != nullptr || dynamic_cast<Factory*>(*it) != nullptr))
					return false;
			ry--;
		}
		break;
	default:
		break;
	}
	return true;
}
void AngryKleptobot::doSomething(int a){
	if (!isActive() ||  a != tickMove())
		return;
	int p = getX(), o = getY();
	deltaDir(p, o, getDirection());
	if (shoot()){				//if it should shoot, then shoot. else, move like a kleptobot
		world()->playSound(SOUND_ENEMY_FIRE);
		Actor* b = new Bullet(IID_BULLET, p, o, world(), getDirection());
		world()->getList().push_front(b);
		Actor* p = Actor::isValidMove(getDirection());
		if (p != nullptr &&p->hittable()){
			b->setActive(false);
			b->setVisible(false);
		}
		return;
	}
	Kleptobot::doSomething(a);
}
void RestoreHealth::doSomething(int a){
	if (!isActive())
		return;
	if (world()->getPlayer()->isActive() && world()->getPlayer()->getX() == getX() && world()->getPlayer()->getY() == getY()){
		Goodie::doSomething(a);
		world()->increaseScore(500);
		world()->getPlayer()->setFullHealth();
	}
}
bool AngryKleptobot::hittable(){
	Kleptobot::hittable();
	if (getHealth() <= 0)
		world()->increaseScore(10);
	return true;
}