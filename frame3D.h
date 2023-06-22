#ifndef FRAME3D_H
#define FRAME3D_H

#include "pokemon.h"
#include <QColor>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class Frame3D : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    Frame3D(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void ColorToRGB(const QColor &C, float &r, float &g, float &b) const;
    float normaliza_0_1(float val, float min, float max) const;

private:
    Pokemon* Charizard;
    QVector3D lightPosition;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
};

#endif // FRAME3D_H
