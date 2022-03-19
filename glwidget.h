#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMap>
#include "cmesh.h"
#include <player.h>
#include <vector>
#include "terrain.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    QSize sizeHint() const override;

    friend CMesh;

public slots:
    void cleanup();

signals:

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void setTransforms(void);
    void updateGL();
    void createScene();
    void testColisions();
    void onCollision();
private:

    struct LightLocStruct
    {
        int position;
        int ambient;
        int diffuse;
    };

    QPoint m_lastPos;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_viewMatrixLoc;
    int m_modelMatrixLoc;
    int m_modelColorLoc;
    int m_hasTextureLoc;
    LightLocStruct m_lightLoc;

    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;

    bool m_keyState[256];

    float m_camDistance = 1.5f;

    int m_timer;

    Player m_player;
    Terrain terrain;

    std::vector<GameObject*> m_gameObjects;
    void addObject(GameObject* obj);

};

#endif

