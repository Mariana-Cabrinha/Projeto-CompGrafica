#ifndef MATRIZ_H
#define MATRIZ_H

#include <QVector>
#include <cmath>

class Matriz {
public:
    Matriz(int nLinhas, int nColunas); // construtor com número de linhas e colunas
    Matriz operator*(Matriz outraMatriz); // operador de multiplicação de matrizes

    void transalacao(double transX, double transY); // método para aplicar translação na matriz
    void escala(double fatorX, double fatorY); // método para aplicar escala na matriz
    void rotacao(double angulo); // método para aplicar rotação na matriz

private:
    int nLinhas, nColunas;
};

#endif // MATRIZ_H
