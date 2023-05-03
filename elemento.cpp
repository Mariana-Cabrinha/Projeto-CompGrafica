#include "elemento.h"

ElementoPonto::ElementoPonto(double x, double y) {
    // Definindo o ponto
    pontos.push_back(Ponto(x, y));

    // Definindo nome, tipo e cor
    nome = "Ponto";
    tipo = 'p';
    cor = Qt::black;
}

Linha::Linha(double x, double y, double comprimento) {
    // Definindo os pontos
    pontos.push_back(Ponto(x, y));
    pontos.push_back(Ponto(x + comprimento, y));

    // Definindo nome, tipo e cor
    nome = "Linha";
    tipo = 'l';
    cor = Qt::black;
}

Retangulo::Retangulo(double x, double y, double largura, double altura) {
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

Circulo::Circulo(double elx, double ely, double raio, int numPontos) {
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
