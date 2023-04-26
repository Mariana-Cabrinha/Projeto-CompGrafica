#include "meu_frame.h"
#include "matriz.h"
#include <iostream>
#include <QVector>
#include <cmath>
#include <QPainter>
#include <QPolygon>
#include <QRandomGenerator>

// Classe Ponto
class Ponto {
public:
    double x, y;

    Ponto() : x(0), y(0) {}
    Ponto(double x, double y) : x(x), y(y) {}
};

// Classe Elemento
class Elemento {
public:
    QString nome;
    QVector<Ponto> pontos;
    char tipo;
    QColor cor;
};

// Classe Ponto (Elemento)
class ElementoPonto : public Elemento {
public:
    // Como criar um ponto
    ElementoPonto(double x, double y) {
        // Definindo o ponto
        pontos.push_back(Ponto(x, y));

        // Definindo nome, tipo e cor
        nome = "Ponto";
        tipo = 'p';
        cor = Qt::black;
    }
};

// Classe Linha
class Linha : public Elemento {
public:
    // Como criar uma linha
    Linha(double x, double y, double comprimento) {
        // Definindo os pontos
        pontos.push_back(Ponto(x, y));
        pontos.push_back(Ponto(x + comprimento, y));

        // Definindo nome, tipo e cor
        nome = "Linha";
        tipo = 'l';
        cor = Qt::black;
    }
};

// Classe Retângulo
class Retangulo : public Elemento {
public:
    // Como criar um retângulo
    Retangulo(double x, double y, double largura, double altura) {
        // Definindo os pontos
        pontos.push_back(Ponto(x, y));
        pontos.push_back(Ponto(x + largura, y));
        pontos.push_back(Ponto(x + largura, y + altura));
        pontos.push_back(Ponto(x, y + altura));

        // Definindo nome, tipo e cor
        nome = "Retângulo";
        tipo = 'r';
        cor = Qt::black;
    }
};

class Circulo : public Elemento {
public:
    // Como criar um círculo
    Circulo(double elx, double ely, double raio, int numPontos) {
        // Definindo os pontos
        double angulo = 2 * M_PI / numPontos;
        for (int i = 0; i < numPontos; ++i) {
            double x = (raio * std::cos(i * angulo)) + elx;
            double y = (raio * std::sin(i * angulo)) + ely;
            pontos.push_back(Ponto(x, y));
        }

        // Definindo nome, tipo e cor
        nome = "Círculo";
        tipo = 'c';
        cor = Qt::black;
    }
};

QList<Elemento> elementos; //Lista de elementos global

MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(200, 200);

    ElementoPonto P1(75, 75);
    Linha L1(25, 25, 50);
    Retangulo R1(10, 10, 50, 50);
    Circulo C1(50, 50, 50, 10);

    elementos.append(P1);
    elementos.append(L1);
    elementos.append(R1);
    elementos.append(C1);
}

void MyFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);

    painter.setViewport(0, 0, 200, 200);
    painter.setWindow(0, 0, 400, 400);

    for(auto& elemento : elementos) {
        QPen pen(elemento.cor, 2);
        painter.setPen(pen);

        int x, y, w, h, raio;

        switch (elemento.tipo) {
        case 'p':
            painter.drawPoint(elemento.pontos[0].x, elemento.pontos[0].y);
            break;
        case 'l':
            painter.drawLine(elemento.pontos[0].x, elemento.pontos[0].y, elemento.pontos[1].x, elemento.pontos[1].y);
            break;
        case 'r':
            x = std::fmin(elemento.pontos[0].x, elemento.pontos[1].x);
            y = std::fmin(elemento.pontos[0].y, elemento.pontos[1].y);
            w = std::abs(elemento.pontos[0].x - elemento.pontos[1].x);
            h = std::abs(elemento.pontos[0].y - elemento.pontos[2].y);
            painter.drawRect(x, y, w, h);
            break;
        case 'c':
            x = elemento.pontos[0].x;
            y = elemento.pontos[0].y;
            raio = std::sqrt(std::pow(elemento.pontos[1].x - x, 2) + std::pow(elemento.pontos[1].y - y, 2));
            painter.drawEllipse(QPoint(x, y), raio, raio);
            break;
        default:
            std::cout << "Tipo de elemento desconhecido: " << elemento.tipo << std::endl;
            break;
        }
    }
}

void MyFrame::colorir()
{
    for (auto& elemento : elementos) {
        // Gerando valores aleatórios para os componentes RGB
        int r = QRandomGenerator::global()->bounded(256);
        int g = QRandomGenerator::global()->bounded(256);
        int b = QRandomGenerator::global()->bounded(256);

        // Criando uma nova QColor com os valores aleatórios
        QColor cor_aleatoria(r, g, b);

        elemento.cor = cor_aleatoria;
    }

    update();
}

void MyFrame::translacao()
{
    for (auto& elemento : elementos) {
        QVector<Ponto> novos_pontos;
        for (auto& ponto : elemento.pontos) {
            Ponto novo(ponto.x + 3, ponto.y + 3);
            novos_pontos.push_back(novo);
        }
        elemento.pontos = novos_pontos;
    }

    update();
}

void MyFrame::escala()
{
    for (auto& elemento : elementos) {
        QVector<Ponto> novos_pontos;
        for (auto& ponto : elemento.pontos) {
            Ponto novo(ponto.x * 1.1, ponto.y * 1.1);
            novos_pontos.push_back(novo);
        }
        elemento.pontos = novos_pontos;
    }

    update();
}

void MyFrame::rotacao()
{
    for (auto& elemento : elementos) {
        double rad = 10 * M_PI / 180.0;
        double coss = cos(rad);
        double sen = sin(rad);
        QVector<Ponto> novos_pontos;
        for (auto& ponto : elemento.pontos) {
            Ponto novo(ponto.x * coss - ponto.y * sen,
                       ponto.x * sen + ponto.y * coss);
            novos_pontos.push_back(novo);
        }
        elemento.pontos = novos_pontos;
    }

    update();
}
