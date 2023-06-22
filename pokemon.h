#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>

class Pokemon {
public:
    Pokemon(const QString& objFilePath, float posX, float posY, float posZ, float scale, float rotX, float rotY, float rotZ);
    void loadObjFile(const QString& objFilePath, bool calculateNormals);
    void render();
    void calculateFaceNormals(const QVector<QVector3D>& vertices, const QVector<unsigned int>& indices, QVector<QVector3D>& normals);
    QString getObjFilePath() const;
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;
    float getScale() const;
    float getRotationX() const;
    float getRotationY() const;
    float getRotationZ() const;

private:
    QString m_objFilePath;
    float m_positionX;
    float m_positionY;
    float m_positionZ;
    float m_scale;
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer normalBuffer;
    QOpenGLBuffer textureCoordBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLVertexArrayObject vao;
};

#endif // POKEMON_H
