#include "player.h"
#include <math.h>

Player::Player()
{
    position = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.1f;

}

void Player::init()
{
 //m_mesh.generateMeshFromObjFile("resources/bunny.obj");
//scale = QVector3D(0.1f, 0.1f, 0.1f);
    m_mesh.generateMeshFromObjFile("models/sheep.obj");
    //m_mesh.generateCube(1.0f, 1.0f, 1.0f);
 //m_radius = 0.5f;
m_name = "Player";
}
void Player::render(GLWidget* glwidget)
{
 m_mesh.render(glwidget);
}
void Player::update()
{
rotation.setY(90-atan2(direction.z(), direction.x()) * 180 / 3.14f);
}
