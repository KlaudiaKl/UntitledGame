#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <iostream>
#include <qstack.h>
#include "cube.h"
#include "enemy.h"
#include <QDebug>
#include <math.h>
#include "texturemanager.h"
using namespace std;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::sizeHint() const
{
    return QSize(1000, 800);
}

void GLWidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();

    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.161f,0.714f,0.965f, 1);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/shader.vs");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/shader.fs");
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_viewMatrixLoc = m_program->uniformLocation("viewMatrix");
    m_modelMatrixLoc = m_program->uniformLocation("modelMatrix");
    m_modelColorLoc = m_program->uniformLocation("modelColor");
    m_hasTextureLoc = m_program->uniformLocation("hasTexture");
    m_lightLoc.position = m_program->uniformLocation("light.position");
    m_lightLoc.ambient = m_program->uniformLocation("light.ambient");
    m_lightLoc.diffuse = m_program->uniformLocation("light.diffuse");

    m_program->release();
	

    TextureManager::init();

	m_timer = 0;



    createScene();

    testColisions();
}

void GLWidget::createScene() {
    addObject(&m_player);
    for(int i = 0 ; i < 7 ; i++) {
        // Wskaźnik do Cube'a z tworzeniem obiektu.
        // MUSI BYĆ WSKAŹNIK !!!
        Enemy* cube = new Enemy();
        // Ustawienie pozycji Cube'a
        cube->position.setX(i * 1 - 3);
        cube->position.setY(0);
        cube->position.setZ(-10);
        // Kolor Cube'a. Zwykły gradient.
        cube->material_color.setX(i * 0.2f);
        cube->material_color.setY(0.5f);
        cube->material_color.setZ(0);
        // Wielkość Cube'a.
        // Ustawienie w jednej linijce
        // zamiast osobno dla X, Y i Z.
        //cube->scale = QVector3D(0.3f, 0.3f, 0.3f);

        // Dodanie obiektu do sceny.
        addObject(cube);
    }

     addObject(&terrain);



}


void GLWidget::updateGL()
{
    if(m_keyState[Qt::Key_A])  {
        m_player.speed = -0.1f;
        m_player.position.setX(m_player.position.x() + m_player.speed);
        if(m_player.position.x()<-3){
            m_player.position.setX(-3);

        }
    } else if(m_keyState[Qt::Key_D])  {
        m_player.speed = 0.1f;
        m_player.position.setX(m_player.position.x() + m_player.speed);
        if(m_player.position.x()>3){
            m_player.position.setX(3);

        }

    } else {
        m_player.position.setX(round(m_player.position.x()));
    }

    for(int i = 0 ; i < m_gameObjects.size() ; i++)
    {
        GameObject* obj = m_gameObjects[i];
        obj->update();
    }

    testColisions();
}

void GLWidget::testColisions() {
    for(int i = 1 ; i < m_gameObjects.size() -1 ; i++)
    {
        GameObject* obj = m_gameObjects[i];
        // Porównujemy każdy obiekt z każdym


        GameObject* obj2 = m_gameObjects[0];
        // Liczymy wektor od pozycji jednego obiektu do drugiego
        QVector3D v = obj->position - obj2->position;
        // Długość tego wektora to odległość między środkami obiektów
        float d = v.length();
        // Porównujemy z sumą promieni
        if(d < (obj->m_radius + obj2->m_radius))
        {
            qDebug() << "Kolizja";
            // Reakcja na kolizję!
            onCollision();
        }
    }
}

void GLWidget::onCollision() {
    for(int i = 1 ; i < m_gameObjects.size()-1 ; i++) {
        m_gameObjects[i]->resetPosition();
    }
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QStack<QMatrix4x4> worldMatrixStack;

    m_program->bind();
    m_program->setUniformValue(m_lightLoc.position, QVector3D(0.0f, 5.0f, 15.0f));
    m_program->setUniformValue(m_lightLoc.ambient, QVector3D(0.1f, 0.1f, 0.1f));
    m_program->setUniformValue(m_lightLoc.diffuse, QVector3D(0.9f, 0.9f, 0.9f));

    m_world.setToIdentity();

    m_camera.setToIdentity();
    /*m_camera.lookAt(
    m_player.position - m_camDistance * m_player.direction,
    m_player.position,
    QVector3D(0, 1, 0) );*/

    m_camera.lookAt(QVector3D(0,3,3), QVector3D(0,0,-1), QVector3D(0,1,0));


    /*float timerTime = timer.elapsed() * 0.001f;
    float deltaTime = timerTime - lastUpdateTime;
    if(deltaTime >= (1.0f / FPS)) {
        updateGL();
        lastUpdateTime = timerTime;
    }*/

    updateGL();

    for(int i = 0 ; i < m_gameObjects.size() ; i++) {
        GameObject* obj = m_gameObjects[i];
        m_program->setUniformValue(m_modelColorLoc, obj->material_color);

        if (obj->m_texture != nullptr) {
            m_program->setUniformValue(m_hasTextureLoc, 1);
            obj->m_texture->bind();
        } else {
            m_program->setUniformValue(m_hasTextureLoc, 0);
        }
        worldMatrixStack.push(m_world);
        m_world.translate(obj->position);
        m_world.rotate(obj->rotation.x(), 1, 0, 0);
        m_world.rotate(obj->rotation.y(), 0, 1, 0);
        m_world.rotate(obj->rotation.z(), 0, 0, 1);
        m_world.scale(obj->scale);
        setTransforms();
        obj->render(this);
        m_world = worldMatrixStack.pop();
    }

    m_program->release();
    update();
}


void GLWidget::setTransforms(void)
{
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_viewMatrixLoc, m_camera);
    m_program->setUniformValue(m_modelMatrixLoc, m_world);
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        //setXRotation(m_camXRot + 0.5f * dy);
        //setYRotation(m_camYRot + 0.5f * dx);
    } else if (event->buttons() & Qt::RightButton) {
       // setXRotation(m_camXRot + 0.5f * dy);
       // setZRotation(m_camZRot + 0.5f * dx);
    }
    m_lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        exit(0);
    //else if (e->key() == Qt::Key_Q)
        //exit(0);
    else
        QWidget::keyPressEvent(e);

    if(e->key() >= 0 && e->key() <= 255)
        m_keyState[e->key()] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() >= 0 && e->key() <= 255)
        m_keyState[e->key()] = false;
}

void GLWidget::addObject(GameObject* obj)
{
    obj->init();
    m_gameObjects.push_back(obj);
}

