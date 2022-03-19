#include "enemy.h"

#include <QRandomGenerator>

Enemy::Enemy()
{
m_name = "enemy";
}

void Enemy::init(){

    m_mesh.generateMeshFromObjFile("models/lowpolytree.obj");
    //m_mesh.generateCube(1.0f, 1.0f, 1.0f);
    position.setZ(1000);
    resetSpeed();
}


void Enemy::render(GLWidget* glwidget){
    m_mesh.render(glwidget);
}

void Enemy::resetPosition() {
    position.setZ(-40 - (int)(QRandomGenerator::global()->generateDouble()*5.0f) * 4);
}

void Enemy::resetSpeed() {
   // speed = 0.05f + 0.15f * QRandomGenerator::global()->generateDouble();
    speed = 0.24;
}

void Enemy::update(){
    position.setZ(position.z() + speed);
    if (position.z() > 3) {
        resetPosition();
        resetSpeed();
    }
}
