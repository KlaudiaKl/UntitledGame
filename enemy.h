#ifndef ENEMY_H
#define ENEMY_H


#include "gameobject.h"
#include "cmesh.h"
class Enemy : public GameObject
{
public:
 Enemy();
 void init();
 void render(GLWidget* glwidget);
 void update();
 void resetSpeed();
 void resetPosition();
 CMesh m_mesh;
protected:
 double speed;
};

#endif // ENEMY_H
