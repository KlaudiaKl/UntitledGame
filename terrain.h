#ifndef TERRAIN_H
#define TERRAIN_H



#include "gameobject.h"
#include "cmesh.h"
class Terrain : public GameObject
{
public:
 Terrain();
 void init();
 void render(GLWidget* glwidget);
 void update();
 CMesh m_mesh;
};

#endif // TERRAIN_H
