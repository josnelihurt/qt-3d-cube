// Cours de Réalité Virtuelle
// leo.donati@univ-cotedazur.fr
//
// EPU 2019-20
//
#ifndef RVWIDGET_H
#define RVWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QTimer>
#include <QMouseEvent>

class RVWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    float m_angleX,m_angleY,m_angleFov,m_opacity;
    QTimer* m_timer;
    QPoint m_oldPos;
public:
    RVWidget(QWidget *parent = nullptr);
    ~RVWidget() override;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected:
    void initializeBuffer();
    void initializeShaders();

protected:
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

protected slots:
    void update();
    void startAnimation();
    void changeFov(int value);
    void changeOpacity(int value);

};
#endif // RVWIDGET_H
