#include "pokemon.h"
#include <QOpenGLFunctions>
#include <QFile>
#include <QVector3D>
#include <QOpenGLBuffer>

Pokemon::Pokemon(const QString& objFilePath, float posX, float posY, float posZ, float scale, float rotX, float rotY, float rotZ)
    : m_objFilePath(objFilePath), m_positionX(posX), m_positionY(posY), m_positionZ(posZ),
      m_scale(scale), m_rotationX(rotX), m_rotationY(rotY), m_rotationZ(rotZ) {}

void Pokemon::loadObjFile(const QString& objFilePath, bool calculateNormals) {
    QFile file(objFilePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erro ao abrir arquivo OBJ: " << file.errorString();
        return;
    }

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;
    QVector<unsigned int> vertexIndices;
    QVector<unsigned int> normalIndices;
    QVector<unsigned int> textureIndices;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList tokens = line.split(' ');
        QStringList faceIndices;

        if (tokens[0] == "v") {
            float x = tokens[1].toFloat();
            float y = tokens[2].toFloat();
            float z = tokens[3].toFloat();
            vertices.append(QVector3D(x, y, z));
        } else if (tokens[0] == "vn") {
            float nx = tokens[1].toFloat();
            float ny = tokens[2].toFloat();
            float nz = tokens[3].toFloat();
            normals.append(QVector3D(nx, ny, nz));
        } else if (tokens[0] == "vt") {
            float u = tokens[1].toFloat();
            float v = tokens[2].toFloat();
            textureCoords.append(QVector2D(u, v));
        } else if (tokens[0] == "f") {
            for (int i = 1; i <= 3; i++) {
                faceIndices = tokens[i].split('/');
                if (faceIndices.size() < 2) {
                    qDebug() << "Invalid face format: " << tokens[i];
                    continue;
                }
                bool ok;
                unsigned int vertexIndex = faceIndices[0].toUInt(&ok) - 1;
                if (!ok || vertexIndex >= vertices.size()) {
                    qDebug() << "Invalid vertex index: " << faceIndices[0];
                    continue;
                }
                unsigned int textureIndex = faceIndices[1].toUInt(&ok) - 1;
                if (!ok || textureIndex >= textureCoords.size()) {
                    qDebug() << "Invalid texture index: " << faceIndices[1];
                    continue;
                }
                vertexIndices.append(vertexIndex);
                textureIndices.append(textureIndex);
            }
        }
    }

    // Criar e vincular o vertex buffer object (VBO)
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
    qDebug() << "vertices: " << vertexBuffer.size();

    // Criar e vincular o normal buffer object (NBO)
    if (calculateNormals) {
        calculateFaceNormals(vertices, vertexIndices, normals);
    }
    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(normals.constData(), normals.size() * sizeof(QVector3D));

    // Criar e vincular o texture coordinate buffer object (TCBO)
    textureCoordBuffer.create();
    textureCoordBuffer.bind();
    textureCoordBuffer.allocate(textureCoords.constData(), textureCoords.size() * sizeof(QVector2D));

    // Criar e vincular o index buffer object (IBO)
    indexBuffer.create();
    indexBuffer.bind();
    QVector<unsigned int> indices;
    for (int i = 0; i < vertexIndices.size(); i++) {
        indices.append(i);
    }
    indexBuffer.allocate(indices.constData(), indices.size() * sizeof(unsigned int));
}

void Pokemon::render() {
    // Configurar a matriz de transformação para posicionar, escalar e rotacionar o Pokemon
    glPushMatrix();

    // Renderizar o Pokemon usando os buffers OpenGL
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Ativar e vincular os buffers necessários (VBOs e IBO)
    vertexBuffer.bind();
    glVertexPointer(3, GL_FLOAT, 0, nullptr);

    normalBuffer.bind();
    glNormalPointer(GL_FLOAT, 0, nullptr);

    textureCoordBuffer.bind();
    glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

    indexBuffer.bind();

    // Renderizar o objeto usando glDrawElements
    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    // Desativar e desvincular os buffers
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    vertexBuffer.release();
    normalBuffer.release();
    textureCoordBuffer.release();
    indexBuffer.release();

    glPopMatrix();
}

void Pokemon::calculateFaceNormals(const QVector<QVector3D>& vertices, const QVector<unsigned int>& indices, QVector<QVector3D>& normals) {
    normals.resize(vertices.size()); // Inicializar a lista de normais com o tamanho adequado

    for (int i = 0; i < indices.size(); i += 3) {
        unsigned int index1 = indices[i];
        unsigned int index2 = indices[i + 1];
        unsigned int index3 = indices[i + 2];

        const QVector3D& v1 = vertices[index1];
        const QVector3D& v2 = vertices[index2];
        const QVector3D& v3 = vertices[index3];

        QVector3D normal = QVector3D::crossProduct(v2 - v1, v3 - v1).normalized();

        normals[index1] = normal;
        normals[index2] = normal;
        normals[index3] = normal;
    }
}

QString Pokemon::getObjFilePath() const {
    return m_objFilePath;
}

float Pokemon::getPositionX() const {
    return m_positionX;
}

float Pokemon::getPositionY() const {
    return m_positionY;
}

float Pokemon::getPositionZ() const {
    return m_positionZ;
}

float Pokemon::getScale() const {
    return m_scale;
}

float Pokemon::getRotationX() const {
    return m_rotationX;
}

float Pokemon::getRotationY() const {
    return m_rotationY;
}

float Pokemon::getRotationZ() const {
    return m_rotationZ;
}
