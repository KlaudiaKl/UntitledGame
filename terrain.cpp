#include "terrain.h"
#include "texturemanager.h"

Terrain::Terrain()
{
m_name = "Terrain";
}

void Terrain::init(){
    m_mesh.generateCube(7.f, 0.1f, 60.0f);
    position.setY(-0.55f);
    position.setZ(-7);
    material_color.setX(0.3407f);
    material_color.setY(0.4779f);
    material_color.setZ(0.1814f);
    m_texture = TextureManager::getTexture("grass");
}


void Terrain::render(GLWidget* glwidget){
    m_mesh.render(glwidget);
}

void Terrain::update(){
    position.setZ(position.z() + 0.1f);
    if (position.z() > 0) {
        position.setZ(-7);
    }
}
