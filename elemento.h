#ifndef ELEMENTO_H
#define ELEMENTO_H

#include "matriz.h"
#include <QColor>
#include <QPainter>

class Ponto {
public:
    PontoMatriz matriz;

    Ponto() : matriz() {}
    Ponto(double x, double y) : matriz(x, y) {}
};

class Elemento {
public:
    QString nome;
    QVector<Ponto> pontos;
    char tipo;
    QColor cor;
};

class ElementoPonto : public Elemento {
public:
    ElementoPonto(double x, double y);
};

class Linha : public Elemento {
public:
    Linha(double x, double y, double comprimento);
};

class Retangulo : public Elemento {
public:
    Retangulo(double x, double y, double largura, double altura);
};

class Circulo : public Elemento {
public:
    Circulo(double elx, double ely, double raio, int numPontos);
};

#endif
