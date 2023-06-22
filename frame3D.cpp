#include "frame3D.h"
#include "pokemon.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <GL/glu.h>

Frame3D::Frame3D(QWidget *parent) : QOpenGLWidget(parent) {
    Charizard = nullptr;
}

void Frame3D::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Configurar a posição da fonte de luz
    lightPosition = QVector3D(0.0f, 5.0f, -10.0f);

    // Carregar e compilar shaders
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../Projeto-CompGrafica/vertexShader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../Projeto-CompGrafica/fragmentShader.glsl");
    shaderProgram.link();
}

void Frame3D::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    // Configurar as fontes de luz
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    GLfloat lightPositionArray[4] = {lightPosition.x(), lightPosition.y(), lightPosition.z(), 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositionArray);

    // Configurar os materiais do cubo
    GLfloat materialAmbient[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat materialDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialShininess = 32.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    // Habilitar o cálculo das sombras
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);

    // Desenhar o cubo
    shaderProgram.bind();
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    shaderProgram.enableAttributeArray(1);

    // Renderizar o cubo com sombras
    glPushMatrix();
    // Cálculo da matriz de sombra
    GLfloat groundPlane[] = {0.0f, 1.0f, 0.0f, 0.0f};
    GLfloat lightPos[] = {lightPosition.x(), lightPosition.y(), lightPosition.z(), 1.0f};
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0.1f, 0.1f, 0.1f); // Cor da sombra
    glPushMatrix();
    glMultMatrixf(groundPlane);
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glScalef(1.0f, 0.0f, 1.0f);

    // Renderizar o cubo com sombra
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();

    // Renderizar o cubo principal
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Criar buffer de vértices
    vertexBuffer.create();
    vertexBuffer.bind();
    float vertices[] = {
        // Face frontal
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        // Face traseira
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Face superior
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Face inferior
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // Face lateral esquerda
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        // Face lateral direita
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f
    };

    QVector<GLfloat> verticesGL;
    for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 3) {
        verticesGL.append(vertices[i]);
        verticesGL.append(vertices[i + 1]);
        verticesGL.append(vertices[i + 2]);
    }
    glVertexPointer(3, GL_FLOAT, 0, verticesGL.data());

    vertexBuffer.allocate(verticesGL.constData(), verticesGL.size() * sizeof(GLfloat));
    Charizard->render();
    vertexBuffer.release();

    shaderProgram.disableAttributeArray(0);
    shaderProgram.disableAttributeArray(1);
    shaderProgram.release();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // Cor da sombra
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}

/*
    glDrawArrays(GL_QUADS, 0, 24);
}*/

void Frame3D::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
}


void Frame3D::ColorToRGB(const QColor &C, float &r, float &g, float &b) const {
    r = normaliza_0_1(C.red(), 1.0f, 255.0f);
    g = normaliza_0_1(C.green(), 1.0f, 255.0f);
    b = normaliza_0_1(C.blue(), 1.0f, 255.0f);
}

float Frame3D::normaliza_0_1(float val, float min, float max) const {
    return (val - min) / (max - min);
}
