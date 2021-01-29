// Cours de Réalité Virtuelle
// leo.donati@univ-cotedazur.fr
//
// EPU 2019-20
//
#include "rvwidget.h"
#include <QMessageBox>

RVWidget::RVWidget(QWidget *parent)
    : QOpenGLWidget(parent), QOpenGLFunctions(),
      m_angleX(30.0f),m_angleY(0),m_angleFov(45), m_opacity(100)
{
    m_timer = new QTimer();
}

RVWidget::~RVWidget()
{
}

void RVWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // background polytech blue R: 53 G: 148 B: 216 -> r,g,b/255 to get float
    glClearColor(0.208f, 0.580f, 0.840f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    initializeBuffer();
    initializeShaders();
    connect(m_timer, SIGNAL(timeout()),this, SLOT(update()));
}

void RVWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    Lignes de code déplacées dans initializeGL()
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    //glDisable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    Lignes de code déplacées dans initializeShader() pour définir le VAO
//
//    m_program.setAttributeBuffer("rv_Position", GL_FLOAT, 0, 3);
//    m_program.enableAttributeArray("rv_Position");
//    m_program.setAttributeBuffer("rv_Color", GL_FLOAT, sizeof(QVector3D)*4, 3);
//    m_program.enableAttributeArray("rv_Color");

    QMatrix4x4 model, proj, view, matrix;

    //Définition de la matrice de projetcion
    proj.perspective(m_angleFov, float(width())/float(height()), 0.1f, 100.0f);

    //Définiion de la matrice de vue
    view  = QMatrix4x4();

    //Définition de la matrice de placement
    model.translate(0, 0, -3);
    model.rotate(m_angleX, 1.0f, 0.0f, 0.0f);
    model.rotate(m_angleY, 0.0f, 1.0f, 0.0f);
    model.translate(-0.5f, -0.5f, -0.5f);
    //model.translate(0.0f, 0.0f, -5.0f);

    //model.rotate(30.0f, 0.0f, 1.0f, 0.0f);

    //Produit des trois matrices (dans ce sens!)
    matrix = proj * view * model;

    m_program.bind();
    m_program.setUniformValue("u_ModelViewProjectionMatrix", matrix);
    m_program.setUniformValue("u_Opacity", m_opacity);

    m_vao.bind();
    for(int i = 0; i < 6; ++i){
        glDrawArrays(GL_TRIANGLE_FAN, i*4, 4);
    }
    m_vao.release();
    m_program.release();

}

void RVWidget::resizeGL(int w, int h)
{
    //transformation de viewport
    glViewport(0, 0, w, h);
}

void RVWidget::mousePressEvent(QMouseEvent *event)
{
    m_timer->stop();
    m_oldPos = event->pos();
}

void RVWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx,dy;
    dx = float(m_oldPos.x()-event->pos().x())/width();
    dy = float(m_oldPos.y()-event->pos().y())/height();

    m_angleX += dx * 180;
    m_angleY += dy * 180;

    m_oldPos = event->pos();

    QOpenGLWidget::update();
}

void RVWidget::initializeBuffer()
{
    //Définition de 6 points
    QVector3D A(0.0f, 0.0f, 1.0f);
    QVector3D B(1.0f, 0.0f, 1.0f);
    QVector3D C(1.0f, 1.0f, 1.0f);
    QVector3D D(0.0f, 1.0f, 1.0f);
    QVector3D E(0.0f, 0.0f, 0.0f);
    QVector3D F(1.0f, 0.0f, 0.0f);
    QVector3D G(1.0f, 1.0f, 0.0f);
    QVector3D H(0.0f, 1.0f, 0.0f);

    QVector3D rouge     (1.0f, 0.0f, 0.0f);
    QVector3D vert      (0.0f, 1.0f, 0.0f);
    QVector3D bleu      (0.0f, 0.0f, 1.0f);
    QVector3D cyan      (0.0f, 1.0f, 1.0f);
    QVector3D magenta   (1.0f, 0.0f, 1.0f);
    QVector3D juane     (1.0f, 1.0f, 0.0f);



    //On prépare le tableau des données
    QVector3D vertexData[] = {
        B,F,E,A,// 6-plane
        A,B,C,D,// 1-plane
        D,C,G,H,// 2-plane
        C,G,F,B,// 3-plane
        A,D,H,E,// 4-plane
        E,F,G,H,// 5-plane
        rouge,  rouge,   rouge,    rouge,
        vert,   vert,   vert,   vert,
        bleu,   bleu,   bleu,   bleu,
        magenta,magenta,magenta,magenta,
        cyan,   cyan,   cyan,   cyan,
        juane,  juane,  juane,  juane

    };

    //Initialisation du Vertex Buffer Object
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //Création du VBO
    m_vbo.create();
    //Lien du VBO avec le contexte de rendu OpenG
    m_vbo.bind();
    //Allocation des données dans le VBO
    m_vbo.allocate(vertexData, sizeof (vertexData));
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    //Libération du VBO
    m_vbo.release();
}

void RVWidget::initializeShaders()
{
    bool resultat;
    m_program.create();
    m_program.bind();

    //Vertex Shader
    resultat = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/VS_simple.vsh");
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Vertex shader");
        msg.setText(m_program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Fragment Shader
    resultat = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/FS_simple.fsh");
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Fragment shader");
        msg.setText(m_program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Link
    resultat = m_program.link();
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Link du shader program");
        msg.setText(m_program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Initialisation du VAO
    m_vao.create();
    m_vao.bind();
    m_vbo.bind();

    //Définition des attributs
    m_program.setAttributeBuffer("rv_Position", GL_FLOAT, 0, 3);
    m_program.enableAttributeArray("rv_Position");

    m_program.setAttributeBuffer("rv_Color", GL_FLOAT, sizeof(QVector3D)*6*4, 3); // now we have 6 planes each of 4 points
    m_program.enableAttributeArray("rv_Color");

    //Libération
    m_vao.release();
    m_program.release();
}
void RVWidget::update()
{
    m_angleY += 5;
    QOpenGLWidget::update();
}

void RVWidget::startAnimation()
{
    m_timer->start(50);
}

void RVWidget::changeFov(int value)
{
    m_angleFov = value;
    QOpenGLWidget::update();
}

void RVWidget::changeOpacity(int value)
{
    m_opacity = float(value)/100.0f;
    qDebug() << "op " << m_opacity ;
    QOpenGLWidget::update();
}

